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
#include "strutils.h"

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
            fprintf(stderr, "createserver: setsockopt\n"); // This is noteworthy, I think
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
        close(listensock);
        return -1;
    }

    freeaddrinfo(ai); // Done with this

    error = listen(listensock, 10);
    if (error) {
        fprintf(stderr, "createserver: listen\n");
        close(listensock);
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
        fprintf(stderr, "acceptnewconn: accept");
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
        fprintf(stderr, "Error prompting new user for username.\n");
        return -1;
    }

    printf("\tAccepted new connection on socket %d\n", newfd);
    return newfd;
}

int readall(int sock)
{
    unsigned char buffer[BUFFER_LEN];
    size_t read = 0;
    user *user = getuserbysock(users, sock);

    read = recv(sock, buffer, BUFFER_LEN - user->alloctotal - 1, 0);
    if (read <= 0) {
        // Connection closed, delete user
        deluser(&users, user);
        return -1;
    }

    // Check for telnet command and discard
    // TODO: Do real handling of telnet commands
    if (buffer[0] == 255) return 0;

    if (BUFFER_LEN - user->alloctotal - read > 0) {
        strncpy(user->allocptr, buffer, read);

        user->allocptr += read;
        char *end = strchr(user->allocbuf, '\r');
        if (end && (buffer[read - 1] == '\0' || *(end + 1) == '\n')) {
            parse(sock, user->allocbuf);
            resetuserbuffer(user);
        }
    } else {
        sendtouser(user, "Buffer overflow");
        resetuserbuffer(user);
    }

    return 0;
}

void sendtoall(char *buffer)
{
    int size = strlen(buffer);
    for (int s = 0; s <= maxfd; s++) {
        if (FD_ISSET(s, &master) && s != listensock) {
            char output[BUFFER_LEN];
            sprintf(output, "%s%s", buffer, TELNET_EOL);
            size = strlen(output);
            send(s, output, size, 0);
        }
    }
}

void sendtouser(user *u, char *msg) {
    char output[BUFFER_LEN];
    sprintf(output, "%s%s", msg, TELNET_EOL);
    send(u->sock, output, strlen(output), 0);
}

void senderror(user *u, char *msg) {
    char error_msg[100];
    sprintf(error_msg, "error: %s%s", msg, TELNET_EOL);
    send(u->sock, error_msg, strlen(error_msg), 0);
}
