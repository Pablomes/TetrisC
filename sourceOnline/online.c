#include "online.h"

WSADATA wsaData;

SOCKET sock;
SOCKET otherSock;
SOCKET discoverySock;

SOCKADDR_IN addr;
SOCKADDR_IN otherAddr;
SOCKADDR_IN discoveryAddr;

void ErrExit(const char* msg) {
    printf("%s\n", msg);
    raise(SIGINT);
}

void InitWinSock() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { ErrExit("WSAStartup() error!"); }
    printf("\nWSAStartup() succeeded.\n");
}

void InitServer(int port, int discoveryPort) {

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) { ErrExit("socket() error!"); }
    printf("socket() succeeded.\n");

    addr.sin_port = htons(port);

    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) { ErrExit("bind() error!"); }
    printf("bind on port %d for comms...\n", port);

    if (listen(sock, 2) == SOCKET_ERROR) { ErrExit("listen() error!"); }
    printf("listen() succeded\n");

    memset(&discoveryAddr, 0, sizeof(discoveryAddr));
    discoveryAddr.sin_family = AF_INET;
    discoveryAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    discoverySock = socket(PF_INET, SOCK_DGRAM, 0);
    if (discoverySock == INVALID_SOCKET) { ErrExit("socket() error!"); }
    printf("socket() succeeded.\n");

    discoveryAddr.sin_port = htons(discoveryPort);

    if (bind(discoverySock, (SOCKADDR*)&discoveryAddr, sizeof(discoveryAddr)) == SOCKET_ERROR) { ErrExit("bind() error!"); }
    printf("bind on port %d for discovery...\n", discoveryPort);
    printf("\nWaiting for other player as client...\n");

}

void ServerListenDiscovery() {
    int addrLen = sizeof(otherAddr);

    char buffer[100]; int discovered = 0;

    while (!discovered) {

        printf("Waiting to receive message...\n");
        int recvLen = recvfrom(discoverySock, buffer, sizeof(buffer) - 1, 0, (SOCKADDR*)&otherAddr, &addrLen);
        printf("Message received!\n");

        if (recvLen > 0) {
            buffer[recvLen] = '\0';

            if (strcmp(buffer, "LETS PLAY TETRIS!") == 0) {
                char response[20];
                sprintf(response, "LETS GO:%d", ntohs(addr.sin_port));
                discovered = 1;
                printf("Sending response\n");

                sendto(discoverySock, response, strlen(response), 0, (SOCKADDR*)&otherAddr, addrLen);
            }
        }
    }
}

void AcceptClient() {
    
    SOCKADDR_IN otherAddr;
    int addrLen = sizeof(otherAddr);
    otherSock = accept(sock, (SOCKADDR*)&otherAddr, &addrLen);
    if (otherSock == INVALID_SOCKET) { ErrExit("accept() error!"); }
    printf("accept() succeed.\n");

}

void CleanServer() {
    closesocket(otherSock);
    closesocket(sock);
    closesocket(discoverySock);
    WSACleanup();
}

void InitClient(int discoveryPort) {
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) { ErrExit("socket() error!"); }
    printf("socket() succeed.\n");

    discoverySock = socket(PF_INET, SOCK_DGRAM, 0);
    if (discoverySock == INVALID_SOCKET) { ErrExit("socket() error!"); }
    printf("socket() succeed.\n");

    BOOL broadcast = TRUE;
    if (setsockopt(discoverySock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) == SOCKET_ERROR) {
        ErrExit("setsockopt() error");
    }

    discoveryAddr.sin_family = AF_INET;
    discoveryAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    discoveryAddr.sin_port = htons(discoveryPort);

}

int DiscoverServer(u_long* servAddr) {
    char msg[] = "LETS PLAY TETRIS!";
    int retryDelay = 2;
    int discovered = 0;

    struct timeval timeout;
    fd_set readfds;

    int addrLen = sizeof(otherAddr);
    char buffer[100];

    while (!discovered) {

        if (sendto(discoverySock, msg, strlen(msg), 0, (SOCKADDR*)&discoveryAddr, sizeof(discoveryAddr)) == SOCKET_ERROR) {
            ErrExit("sendto() error");
        }
        printf("Discovery message sent. Waiting for server response...\n");

        FD_ZERO(&readfds);
        FD_SET(discoverySock, &readfds);

        timeout.tv_sec = retryDelay;
        timeout.tv_usec = 0;

        int activity = select(0, &readfds, NULL, NULL, &timeout);

        if (activity > 0) {
            int recv_len = recvfrom(discoverySock, buffer, sizeof(buffer) - 1, 0, (SOCKADDR*)&otherAddr, &addrLen);

            if (recv_len > 0) {
                buffer[recv_len] = '\0';
                if (strncmp(buffer, "LETS GO:", 8) == 0) {
                    int port;
                    if (sscanf(buffer + 8, "%d", &port) == 1) {
                        printf("Server discovered on port %d\n", port);
                        *servAddr = otherAddr.sin_addr.s_addr;
                        return port;
                   }
               }
            }
        } else {
            printf("No server found. Trying again...\n");
        }
    }
}

void ConnectToServer(u_long addr, int port) {
    memset(&otherAddr, 0, sizeof(otherAddr));
    otherAddr.sin_family = AF_INET;
    otherAddr.sin_addr.s_addr = addr;
    otherAddr.sin_port = htons(port);

    if (connect(sock, (SOCKADDR*)&otherAddr, sizeof(otherAddr)) == SOCKET_ERROR) {
        ErrExit("connect() error!");
    }
    printf("connect() succeed.\n");
}

void CleanClient() {
    closesocket(sock);
    closesocket(discoverySock);
    WSACleanup();
}

void SendGameState(int** board, int score, int linesCleared, int level, int nextPiece, int holdPiece, int rubbish, int sentRubbish, int running, int isServer, int length, int width) {
    char state[1024];
    int stateLen = 0;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            char temp[10]; itoa(board[i][j], temp, 10);
            state[stateLen] = board[i][j] == -1 ? '9' : temp[0];
            stateLen++;
        }
    }

    char hex[10];

    itoa(score, hex, 16);
    for (int i = 0; i < 8 - (int)strlen(hex); i++) {
        state[stateLen] = '0';
        stateLen++;
    }
    for (int i = 0; i < (int)strlen(hex); i++) {
        state[stateLen] = hex[i];
        stateLen++;
    }

    itoa(linesCleared, hex, 16);
    for (int i = 0; i < 8 - (int)strlen(hex); i++) {
        state[stateLen] = '0';
        stateLen++;
    }
    for (int i = 0; i < (int)strlen(hex); i++) {
        state[stateLen] = hex[i];
        stateLen++;
    }

    itoa(level, hex, 16);
    for (int i = 0; i < 8 - (int)strlen(hex); i++) {
        state[stateLen] = '0';
        stateLen++;
    }
    for (int i = 0; i < (int)strlen(hex); i++) {
        state[stateLen] = hex[i];
        stateLen++;
    }

    itoa(nextPiece, hex, 16);
    for (int i = 0; i < 8 - (int)strlen(hex); i++) {
        state[stateLen] = '0';
        stateLen++;
    }
    for (int i = 0; i < (int)strlen(hex); i++) {
        state[stateLen] = hex[i];
        stateLen++;
    }

    itoa((holdPiece == -1) ? 9 : holdPiece, hex, 16);
    for (int i = 0; i < 8 - (int)strlen(hex); i++) {
        state[stateLen] = '0';
        stateLen++;
    }
    for (int i = 0; i < (int)strlen(hex); i++) {
        state[stateLen] = hex[i];
        stateLen++;
    }

    itoa(rubbish, hex, 16);
    for (int i = 0; i < 8 - (int)strlen(hex); i++) {
        state[stateLen] = '0';
        stateLen++;
    }
    for (int i = 0; i < (int)strlen(hex); i++) {
        state[stateLen] = hex[i];
        stateLen++;
    }

    itoa(sentRubbish, hex, 16);
    for (int i = 0; i < 8 - (int)strlen(hex); i++) {
        state[stateLen] = '0';
        stateLen++;
    }
    for (int i = 0; i < (int)strlen(hex); i++) {
        state[stateLen] = hex[i];
        stateLen++;
    }

    state[stateLen] = running ? '1' : '0';
    stateLen++;

    state[stateLen] = '\0';

    //sendto(sock, state, strlen(state), 0, (SOCKADDR*)&otherAddr, sizeof(otherAddr));
    if (isServer) { send(otherSock, state, strlen(state) + 1, 0); }
    else { send(sock, state, strlen(state) + 1, 0); }
}

int ReceiveGameState(int** board, int* score, int* linesCleared, int* level, int* nextPiece, int* holdPiece, int* rubbish, int* receivedRubbish, int isServer, int length, int width) {

    char state[1024]; int addrLen = sizeof(otherAddr);
    int recvLen = 0; 

    //int recvLen = recvfrom(sock, state, sizeof(state) - 1, 0, (SOCKADDR*)&otherAddr, &addrLen);
    if (isServer) { recvLen = recv(otherSock, state, sizeof(state) - 1, 0); }
    else { recvLen = recv(sock, state, sizeof(state) - 1, 0); }

    if (recvLen <= 0) { ErrExit("Nobody connected."); }

    state[recvLen] = '\0';
    int idx = 0; 

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            char temp[2]; temp[1] = '\0'; temp[0] = state[idx];
            int cell = (int)strtol(temp, NULL, 16);
            
            board[i][j] = cell == 9 ? -1 : cell;
            idx++;
        }
    }

    char hex[9]; hex[8] = '\0';

    for (int i = 0; i < 8; i++) {
        hex[i] = state[idx];
        idx++;
    }
    *score = (int)strtol(hex, NULL, 16);

    for (int i = 0; i < 8; i++) {
        hex[i] = state[idx];
        idx++;
    }
    *linesCleared = (int)strtol(hex, NULL, 16);

    for (int i = 0; i < 8; i++) {
        hex[i] = state[idx];
        idx++;
    }
    *level = (int)strtol(hex, NULL, 16);

    for (int i = 0; i < 8; i++) {
        hex[i] = state[idx];
        idx++;
    }
    *nextPiece = (int)strtol(hex, NULL, 16);

    for (int i = 0; i < 8; i++) {
        hex[i] = state[idx];
        idx++;
    }
    *holdPiece = (int)strtol(hex, NULL, 16);
    if (*holdPiece == 9) { *holdPiece = -1; }

    for (int i = 0; i < 8; i++) {
        hex[i] = state[idx];
        idx++;
    }
    *rubbish = (int)strtol(hex, NULL, 16);

    for (int i = 0; i < 8; i++) {
        hex[i] = state[idx];
        idx++;
    }
    *receivedRubbish = (int)strtol(hex, NULL, 16);

    return state[idx] == '0' ? 0 : 1;
}

void ConnectAsServer() {
    InitWinSock();

    int uPort = 0;

    while (uPort < 4000 || uPort > 9999) {

        printf("\n\nPlease choose a port for the game, it should be from 4001 - 9999. Refrain from using port 8888. If in doubt, use 9090: ");
        scanf("%d", &uPort);

        if (uPort < 4000 || uPort > 9999) {printf("\nNot a valid port, please try again.\n"); }
    }

    InitServer(uPort, 4000);

    ServerListenDiscovery();

    AcceptClient();
}

void ConnectAsClient() {
    InitWinSock();

    InitClient(4000);

    u_long servAddr;

    int foundPort = DiscoverServer(&servAddr);

    ConnectToServer(servAddr, foundPort);
}