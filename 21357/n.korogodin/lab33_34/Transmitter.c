#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void frame_data(int connection_id, char* data, char* frame) {
    sprintf(frame, "\x7E%c%s\x7E", connection_id, data);
}

void transmit_data(int transmitter_socket, int connection_id, char* data) {
    char frame[BUFFER_SIZE];
    frame_data(connection_id, data, frame);
    write(transmitter_socket, frame, strlen(frame));
}

int main() {
    int transmitter_socket, receiver_socket;
    struct sockaddr_in transmitter_addr, receiver_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];

    transmitter_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (transmitter_socket == -1) {
        perror("Error creating transmitter socket");
        exit(EXIT_FAILURE);
    }

    transmitter_addr.sin_family = AF_INET;
    transmitter_addr.sin_addr.s_addr = INADDR_ANY;
    transmitter_addr.sin_port = htons(12345);

    bind(transmitter_socket, (struct sockaddr*)&transmitter_addr, sizeof(transmitter_addr));
    listen(transmitter_socket, 1);

    printf("Transmitter listening on 127.0.0.1:12345\n");

    receiver_socket = accept(transmitter_socket, (struct sockaddr*)&receiver_addr, &addr_size);
    printf("Connection established with receiver\n");

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        transmit_data(receiver_socket, 1, buffer);
    }

    close(transmitter_socket);
    close(receiver_socket);

    return 0;
}