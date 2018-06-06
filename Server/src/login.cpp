#include "login.hpp"

Login::Login(int const port, std::vector<int>& clients, std::map<int, Player>& players): clients(clients), players(players) {
    /* 
     * AF_INET => IPv4 || AF_INET6 => IPv6
     * SOCK_STREAM: TCP || SOCK_DGRAM: UDP
     * man protocols
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    } else {
        std::cout << "Server socket created." << std::endl;
    }
    
    /*
     * Setup port with reuse
     */
    int opt = 1; // Black magic
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Failed to attached socket to port");
        exit(EXIT_FAILURE);
    }
    
    /*
     * Setup address, port etc 
     */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    // Bin socket to address and port
    if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Failed to bin socket to port");
        exit(EXIT_FAILURE);
    } else {
        std::cout << "Server binded to port " << port << "." << std::endl;
    }

    /*
     *  Socket listen with up to 10 clients waiting
     */
    if (listen(sockfd, 10) < 0) {
        perror("Failed to listen to socket");
        exit(EXIT_FAILURE);
    } else {
        std::cout << "Server waiting for clients." << std::endl;
    }
}

void Login::Run() {
    loop = new std::thread(&Login::Loop, this);
}

void Login::Loop() {
    // Main loop
    while (true) {
        /*
         *  Wait for a new client to accept
         *  addrlen ends up with the clients address length
         */
        socklen_t addrlen = sizeof(address);
        int clientSocket = accept(sockfd, (struct sockaddr *)&address, &addrlen);
        if (clientSocket < 0) {
            perror("Failed to get client socket");
            exit(EXIT_FAILURE);
        } else {
            /*
            *  Read buffer send by client
            *  Send "hello"
            */
            char buffer[1024] = {0};
            read(clientSocket, buffer, 1024);
            printf("%s\n", buffer);
            send(clientSocket , &clientSocket, sizeof(int), 0 );

            clients.push_back(clientSocket);
            players[clientSocket] = Player();

            PlayerThread pthread(clientSocket, players[clientSocket]);
            pthread.Run();

            printf("New client added with id %d \n", clientSocket);

            // Sending spawn message to all clients including myself
            short offset = 0;
            char* spawBuffer = new char[1 + 3*sizeof(int)];

            (*(int*)(spawBuffer + offset)) = 2*sizeof(int);
            offset += sizeof(int);

            spawBuffer[offset++] = 2;

            // player prefab id is 0
            (*(int*)(spawBuffer + offset)) = 0;
            offset += sizeof(int);
            // client id
            (*(int*)(spawBuffer + offset)) = clientSocket;
            offset += sizeof(int);   

            for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
                // offset now has the value of the buffer's size
                send(*it, spawBuffer, offset, 0);

                printf("Spawn message sent to client %d \n", *it);   
            }         
        }
    }
}