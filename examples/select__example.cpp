fd_set set;
timeval timeout = { 0 };
FD_ZERO(&set);
FD_SET(server, &set);
int selectReady = select(server + 1, &set, nullptr, nullptr, &timeout);
if (selectReady == -1)
{
	std::cout << "Erreur select pour accept : " << Sockets::GetError() << std::endl;
	break;
}
else if (selectReady > 0) 
{
	// notre socket server est prêt à être lu
	sockaddr_in from = { 0 };
	socklen_t addrlen = sizeof(from);
	SOCKET newClientSocket = accept(server, (SOCKADDR*)(&from), &addrlen);
	if (newClientSocket != INVALID_SOCKET)
	{
		Client newClient;
		newClient.sckt = newClientSocket;
		newClient.addr = from;
		const std::string clientAddress = Sockets::GetAddress(from);
		const unsigned short clientPort = ntohs(from.sin_port);
		std::cout << "Connexion de " << clientAddress.c_str() << ":" << clientPort << std::endl;
	}
}

fd_set setReads;
fd_set setWrite;
fd_set setErrors;
int highestFd = 0;
timeval timeout = { 0 };
for (auto& client : clients)
{
	FD_SET(client.sckt, &setReads);
	FD_SET(client.sckt, &setWrite);
	FD_SET(client.sckt, &setErrors);
	if (client.sckt > highestFd)
		highestFd = client.sckt;
}
int selectResult = select(highestFd + 1, &setReads, &setWrite, &setErrors, &timeout);
if (selectResult == -1)
	// erreur
else if (selectResult > 0)
	// au moins 1 client a une action à exécuter

auto itClient = clients.begin();
while (itClient != clients.end())
{
	const std::string clientAddress = Sockets::GetAddress(itClient->addr);
	const unsigned short clientPort = ntohs(itClient->addr.sin_port);

	bool hasError = false;
	if (FD_ISSET(itClient->sckt, &setErrors))
	{
		std::cout << "Erreur" << std::endl;
		hasError = true;
	}
	else if (FD_ISSET(itClient->sckt, &setReads))
	{
		char buffer[200] = { 0 };
		int ret = recv(itClient->sckt, buffer, 199, 0);
		if (ret == 0 || ret == SOCKET_ERROR)
		{
			std::cout << "Erreur reception" << std::endl;
			hasError = true;
		}
		else
		{
			std::cout << "[" << clientAddress << ":" << clientPort << "]" << buffer << std::endl;
			if (FD_ISSET(itClient->sckt, &setWrite))
			{
				ret = send(itClient->sckt, buffer, ret, 0);
				if (ret == 0 || ret == SOCKET_ERROR)
				{
					std::cout << "Erreur envo" << std::endl;
					hasError = true;
				}
			}
		}
	}
	if (hasError)
	{
		//!< Déconnecté
		std::cout << "Deconnexion de [" << clientAddress << ":" << clientPort << "]" << std::endl;
		itClient = clients.erase(itClient);
	}
	else
	{
		++itClient;
	}
}