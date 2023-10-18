#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
private:
    int clientSocket;
    std::vector<Client*> clients;

	// request line information
	std::string method;
	std::string uri;
	std::string protocol;

	// header information
	std::map <std::string, std::string> headerMap; 
	// to find a value in the map:
	/* if (headerMap.find("Host") != headerMap.end())
       	std::cout << "found, value is " << headerMap["Host"] << std::endl; */
public:
    Client();
    Client(int client_socket);
    ~Client();
    Client(Client const &copy);
    Client &operator=(Client const &copy);

    void	saveClientRequest(char* buffer, int client_socket);
	void	parseRequest(char* buffer, Client* client);

	// setters and getters
	Client* getClientListIndex(int index) { return clients[index]; };
	int		getClientSocket() { return clientSocket; };
	std::string getMethod() { return method; };
	// void setMethod(std::string new_method){method = new_method;};
};


#endif