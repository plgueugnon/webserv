std::map<SOCKET, Client> clients;
std::vector<pollfd> clientsFds;
for (;;)
{
	{
		pollfd pollServerFd;
		pollServerFd.fd = server;
		pollServerFd.events = POLLIN;
		int pollReady = poll(&pollServerFd, 1, 0);
		if (pollReady == -1)
		{
			std::cout << "Erreur poll pour accept : " << Sockets::GetError() << std::endl;
			break;
		}
		if (pollReady > 0)
		{
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
				clients[newClientSocket] = newClient;
				pollfd newClientPollFd;
				newClientPollFd.fd = newClientSocket;
				newClientPollFd.events = POLLIN | POLLOUT;
				clientsFds.push_back(newClientPollFd);
			}
		}
	}
	if (!clients.empty())
	{
		int pollResult = poll(clientsFds.data(), static_cast<nfds_t>(clientsFds.size()), 0);
		if (pollResult == -1)
		{
			std::cout << "Erreur poll pour clients : " << Sockets::GetError() << std::endl;
			break;
		}
		else if (pollResult > 0)
		{
			auto itPollResult = clientsFds.cbegin();
			while (itPollResult != clientsFds.cend())
			{
				const auto clientIt = clients.find(itPollResult->fd);
				if (clientIt == clients.cend())
				{
					itPollResult = clientsFds.erase(itPollResult);
					continue;
				}
				const auto& client = clientIt->second;
				const std::string clientAddress = Sockets::GetAddress(client.addr);
				const unsigned short clientPort = ntohs(client.addr.sin_port);
				bool disconnect = false;
				if (itPollResult->revents & POLLERR)
				{
					socklen_t err;
					int errsize = sizeof(err);
					if (getsockopt(client.sckt, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err), &errsize) != 0)
					{
						std::cout << "Impossible de determiner l'erreur : " << Sockets::GetError() << std::endl;
					}
					if (err != 0)
						std::cout << "Erreur : " << err << std::endl;
					disconnect = true;
				}
				else if (itPollResult->revents & (POLLHUP | POLLNVAL))
				{
					disconnect = true;
				}
				else if (itPollResult->revents & POLLIN)
				{
					char buffer[200] = { 0 };
					int ret = recv(client.sckt, buffer, 199, 0);
					if (ret == 0)
					{
						std::cout << "Connexion terminee" << std::endl;
						disconnect = true;
					}
					else if (ret == SOCKET_ERROR)
					{
						std::cout << "Erreur reception : " << Sockets::GetError() << std::endl;
						disconnect = true;
					}
					else
					{
						std::cout << "[" << clientAddress << ":" << clientPort << "]" << buffer << std::endl;
						if (itPollResult->revents & POLLOUT)
						{
							ret = send(client.sckt, buffer, ret, 0);
							if (ret == 0 || ret == SOCKET_ERROR)
							{
								std::cout << "Erreur envoi : " << Sockets::GetError() << std::endl;
								disconnect = true;
							}
						}
					}
				}
				if (disconnect)
				{
					std::cout << "Deconnexion de " << "[" << clientAddress << ":" << clientPort << "]" << std::endl;
					itPollResult = clientsFds.erase(itPollResult);
					clients.erase(clientIt);
				}
				else
				{
					++itPollResult;
				}
			}
		}
	}
}