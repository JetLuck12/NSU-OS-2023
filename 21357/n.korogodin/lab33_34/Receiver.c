#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void unframe_data(char* data, char* decoded_data) {
    strncpy(decoded_data, data + 2, strlen(data) - 3);
}

void receive_data(int receiver_socket, char* data) {
    int len;
    while ((len = read(receiver_socket, data, BUFFER_SIZE - 1)) > 0) {
        data[len] = '\0';
        printf("Received data: %s\n", data);
    }
}

int main() {
    int transmitter_socket, receiver_socket;
    struct sockaddr_in transmitter_addr;
    char buffer[BUFFER_SIZE];

    receiver_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (receiver_socket == -1) {
        perror("Error creating receiver socket");
        exit(EXIT_FAILURE);
    }

    transmitter_addr.sin_family = AF_INET;
    transmitter_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    transmitter_addr.sin_port = htons(12345);

    connect(receiver_socket, (struct sockaddr*)&transmitter_addr, sizeof(transmitter_addr));
    printf("Connection established with transmitter\n");

    while (1) {
        receive_data(receiver_socket, buffer);
    }

    close(receiver_socket);

    return 0;
}