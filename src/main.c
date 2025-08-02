#include<winsock2.h>
#include<stdio.h>
#include<string.h>

int main(){
    WSADATA wsaData;
    int check = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(check != 0) {
        printf("WSAStartup failed with error: %d\n", check);
        return 1;
    }
    else{
        printf("WSAStartup succeeded.\n");
    }

    SOCKET socketDescriptor;
    socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketDescriptor == INVALID_SOCKET){
        printf("Socket creation failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else{
        printf("Socket created Successfully.\n");
    }

    struct sockaddr_in  serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    int bindResult = bind(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if(bindResult == SOCKET_ERROR){
        printf("Binding failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else{
        printf("Binding successful.\n");
    }

    int listenResult = listen(socketDescriptor, 5);
    if(listenResult == SOCKET_ERROR){
        printf("Listening failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else{
        printf("Server is listening.\n");
    }

    while(1){
        SOCKET clientSocket;
        struct sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        clientSocket = accept(socketDescriptor, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if(clientSocket == INVALID_SOCKET){
            printf("Accept failed with error: %d\n", WSAGetLastError());
            continue;
        }
        else{
            printf("Accept successful.\n");
        }

        char requestBuffer[2048];
        int bytesReceived = recv(clientSocket, requestBuffer, sizeof(requestBuffer) - 1, 0);
        if(bytesReceived > 0){
            requestBuffer[bytesReceived] = '\0';
        }
        printf("Received request:\n%s\n", requestBuffer);

        char method[16], path[1024], protocol[16];
        sscanf(requestBuffer, "%s %s %s", method, path, protocol);
        printf("Requested path: %s\n", path);

        if(strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0){
            char filePath[] = "static/index.html";
            printf("Trying to open file: %s\n", filePath);
            FILE *indexFile = fopen(filePath, "rb");
            if(indexFile == NULL){
                printf("Failed to open index.html!\n");
                char notFoundBody[] = "<html><body><h1>404 Not Found</h1></body></html>";
                char notFoundResponse[256];
                sprintf(notFoundResponse,
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %zu\r\n"
                "\r\n%s",
                strlen(notFoundBody), notFoundBody);
                send(clientSocket, notFoundResponse, strlen(notFoundResponse), 0);
                closesocket(clientSocket);
                continue;
            } else {
                fseek(indexFile, 0, SEEK_END);
                long fileSize = ftell(indexFile);
                printf("index.html size: %ld\n", fileSize);
                rewind(indexFile);
                char *fileBuffer = malloc(fileSize);
                if(fileBuffer == NULL){
                    printf("Failed to allocate buffer!\n");
                    fclose(indexFile);
                    closesocket(clientSocket);
                    continue;
                }
                if(fileSize <= 0){
                    fclose(indexFile);
                    char notFoundBody[] = "<html><body><h1>404 Not Found</h1></body></html>";
                    char notFoundResponse[256];
                    sprintf(notFoundResponse,
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %zu\r\n"
                        "\r\n%s",
                        strlen(notFoundBody), notFoundBody);
                    send(clientSocket, notFoundResponse, strlen(notFoundResponse), 0);
                    free(fileBuffer);
                    closesocket(clientSocket);
                    continue;
                }
                size_t bytesRead = fread(fileBuffer, 1, fileSize, indexFile);
                if(bytesRead != fileSize){
                    fclose(indexFile);
                    char notFoundBody[] = "<html><body><h1>404 Not Found</h1></body></html>";
                    char notFoundResponse[256];
                    sprintf(notFoundResponse,
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %zu\r\n"
                        "\r\n%s",
                        strlen(notFoundBody), notFoundBody);
                    send(clientSocket, notFoundResponse, strlen(notFoundResponse), 0);
                    free(fileBuffer); 
                    closesocket(clientSocket);
                    continue;
                }
                else{
                    fclose(indexFile);
                    char header[128];
                    sprintf(header,
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %ld\r\n"
                        "\r\n", fileSize);
                    send(clientSocket, header, strlen(header), 0);
                    send(clientSocket, fileBuffer, fileSize, 0);
                    free(fileBuffer);
                    closesocket(clientSocket);
                }
            }
        }
    }
    WSACleanup();
    return 0;
}