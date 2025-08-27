//obs.: usar threads nos for das funçoes (insertClient)

//TODO LIST:
//criptografia https
//envia paginas html

#include "../include/main.hpp"

int main(){

    Server server;
    std::string cmd;

    if(startServer(&server) == -1){

        std::cerr << "startServer()\n";

        close(server.server_fd);

        return -1;
    }

    std::cout << "SERVER STARTED\n" << "IP:PORT (" << IP_SERVER << ":" << PORT_SERVER << ")\n";

    //creating thread to accept new users
    std::thread connection_thread(waitConnection, &server);
    connection_thread.detach();

    //server commands
    while(server.run){

		std::getline(std::cin,cmd);

        if(cmds(cmd,&server) == -1){

            std::cout << "COMMAND NOT FOUND\n";
        }
		
		cmd.clear();
	}

    std::cout << "SERVER OFFLINE\n";

    return 0;
}