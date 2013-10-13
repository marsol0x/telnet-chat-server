#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>

#include "sock.h"
#include "parse.h"
#include "user.h"

#include "globals.h"

int listensock = -1; // The server's listening socket
int maxfd = -1;      // Max socket descriptor
fd_set master;       // Master socket set
fd_set readfds;      // Socket set for available reading
userlist *users;     // Initialize global user list

int createserver(char *port)
{
    struct addrinfo hints, *ai, *p; // Structures for 
    int error; // Returned errors, if any

    // Initialize our socket sets
    FD_ZERO(&master);
    FD_ZERO(&readfds);

    // Initialize our user list
    users = (userlist *) malloc(sizeof(userlist));
    memset(users, 0, sizeof(userlist));
    
    // Get our local settings for creating a server socket
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_family   = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    error = getaddrinfo(NULL, port, &hints, &ai);

    if (error) {
        fprintf(stderr, "createserver: getaddrinfo error -- %s\n", gai_strerror(error));
        return -1;
    }

    int yes = 1; // For setsockopt to allow for reusing addresses
    for (p = ai; p != NULL; p = p->ai_next) {
        listensock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listensock == -1) {
            continue; // Nope, not this one
        }

        error = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(listensock));
        if (error == -1) {
            perror("createserver: setsockopt"); // This is noteworthy, I think
            close(listensock);
            continue;
        }

        error = bind(listensock, p->ai_addr, p->ai_addrlen);
        if (error) {
            close(listensock);
            continue;
        }

        break;
    }

    // We never got a successful bind
    if (p == NULL) {
        fprintf(stderr, "createserver: Failed to bind port %s\n", port);
        return -1;
    }

    freeaddrinfo(ai); // Done with this

    error = listen(listensock, 10);
    if (error) {
        perror("createserver: listen");
        return -1;
    }

    maxfd = listensock; // The one and only
    FD_SET(listensock, &master);
    
    return listensock;
}

void pollconns()
{
    struct timeval timeout;
    memset(&timeout, 0, sizeof(timeout));
    
    readfds = master;
    if (select(maxfd + 1, &readfds, NULL, NULL, NULL) != -1) {
        for (int s = 0; s <= maxfd; s++) {
            if (FD_ISSET(s, &readfds)) {
                // New connection
                if (s == listensock) {
                    acceptnewconn();
                    continue;
                }

                if (readall(s)) {
                    close(s);
                    FD_CLR(s, &master);
                    continue;
                }
            }
        } 
    } // Select
}

int acceptnewconn()
{
    int newfd;
    struct sockaddr_storage remote;
    socklen_t addrlen = sizeof(remote);
    
    newfd = accept(listensock, (struct sockaddr *) &remote, &addrlen);
    if (newfd == -1) {
        perror("acceptnewconn: accept");
        return -1;
    }

    FD_SET(newfd, &master); // Add new connection to master set
    if (maxfd < newfd) {
        maxfd = newfd;
    }

    // Create new user object
    user *new = newuser(users);
    new->sock = newfd;

    char *prompt = "What is your name?\r\n";
    if (send(newfd, prompt, strlen(prompt), 0) == -1) {
        perror("Error prompting new user for username.");
        return -1;
    }

    printf("\tAccepted new connection on socket %d\n", newfd);
    return newfd;
}

int readall(int sock)
{
    char buffer[256];
    char *tok;
    ssize_t read = 0;
    user *user;

    do {
        read = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (read <= 0) {
            // Connection closed, delete user
            user = getuserbysock(users, sock);
            deluser(&users, user);
            return -1;
        }
        buffer[read] = '\0'; // null terminate
        user = getuserbysock(users, sock);
        if (user == NULL) {
            perror("Error locating user for current socket.");
            return -1;
        }
        if (user->name == NULL) {
            int len = strlen(buffer);
            user->name = (char *) malloc(sizeof(buffer));
            strncpy(user->name, buffer, len);

            // Replace the first instance of a character in TELNET_EOL with the
            // null-terminator, we only need the first one since all subsequent
            // will be auto-nulled by the placement.
            tok = strtok(user->name, TELNET_EOL);
            tok = '\0';

            printf("New user: %s\n", user->name);
        } else {
            printf("%s: %s", user->name, buffer);
            sendtoall(user->name, buffer, read);
        }
        tok = strtok(buffer, TELNET_EOL);
        if (tok != NULL) break; // Input ends at TELNET_EOL
    } while (read != 0);

    return 0;
}

int sendall(int sock, char *buffer, int size)
{
    ssize_t sent = 0;
    int total = 0;

    do {
        sent = send(sock, buffer, size, 0);
        if (sent == -1) {
            // Error sending
            return -1;
        }
        total += sent;
    } while (sent > 0 && total < size);

    return total;
}

void sendtoall(char *user, char *buffer, int size)
{
    char leader[10];
    sprintf(leader, "%s> ", user);
    for (int s = 0; s <= maxfd; s++) {
        if (FD_ISSET(s, &master) && s != listensock) {
            sendall(s, leader, strlen(leader));
            sendall(s, buffer, size);
        }
    }
}
