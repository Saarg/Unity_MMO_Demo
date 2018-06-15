#include <iostream>

#include "login.hpp"
#include "game.hpp"
#include "player.hpp"
#include "server.hpp"

std::vector<int> clients;
std::map<int, Server*> servers;
std::map<int, Player> players;

int main(int argc, char const *argv[])
{
    std::cout << "\033[1;37m";
    if (argc < 2) {
        std::cout << "Usage: Server port oherServerPort" << std::endl;
        return 0;
    }

    Game game(atoi(argv[1]), clients, servers, players);
    Login login(atoi(argv[1]), game);
    
    login.Run();
    game.Run();

    for (int i = 2; i < argc; i++) {
        std::cout << "Trying to join server on " << atoi(argv[2]) << ": ";

        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == 0) {
            perror("Failed to create socket");
            exit(EXIT_FAILURE);
        } else {
            struct sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(atoi(argv[i]));

            if(connect(sockfd, (struct sockaddr *) &address, sizeof(address)) == 0)		
            {
                int id = atoi(argv[i]);
                int curId = atoi(argv[1]);

                int socketType[2] = {1, curId};
                send(sockfd, &socketType, 2*sizeof(int), 0);
                
                servers[id] = new Server(id, sockfd, &game);

                std::cout << "\033[1;32m SUCCESS \033[1;37m" << std::endl;
            } else {
                close(sockfd);
                std::cout << "\033[1;33m FAIL \033[1;37m" << std::endl;                
            }
        }
    }

    std::cout << "Press [ENTER] to quit" << std::endl;
    while (std::cin.get() != '\n') {
        std::cout << "Press [ENTER] to quit" << std::endl;
    }

    std::cout << "Shuting down all sockets";    
    for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
        close(*it);
        std::cout  << ".";            
    }

    std::cout  << "." << std::endl;            
    

    for (std::map<int, Server*>::iterator it=servers.begin(); it!=servers.end(); it++) {
        std::cout  << ".";  
        delete it->second;
    }

    std::cout  << "." << std::endl;   
}