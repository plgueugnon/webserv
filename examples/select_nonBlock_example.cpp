	std::vector<Client> clients;
	for (;;)
	{
		{
			sockaddr_in from = { 0 };
			socklen_t addrlen = sizeof(from);
			SOCKET newClientSocket = accept(server, (SOCKADDR*)(&from), &addrlen);
			if (newClientSocket != INVALID_SOCKET)
			{
				if (!Sockets::SetNonBlocking(newClientSocket))
				{
					std::cout << "Erreur settings nouveau socket non bloquant : " << Sockets::GetError() << std::endl;
					Sockets::CloseSocket(newClientSocket);
					continue;
				}
				Client newClient;
				newClient.sckt = newClientSocket;
				newClient.addr = from;
				const std::string clientAddress = Sockets::GetAddress(from);
				const unsigned short clientPort = ntohs(from.sin_port);
				std::cout << "Connexion de " << clientAddress.c_str() << ":" << clientPort << std::endl;
				clients.push_back(newClient);
			}
		}
		{
			auto itClient = clients.begin();
			while ( itClient != clients.end() )
			{
				const std::string clientAddress = Sockets::GetAddress(itClient->addr);
				const unsigned short clientPort = ntohs(itClient->addr.sin_port);
				char buffer[200] = { 0 };
				bool disconnect = false;
				int ret = recv(itClient->sckt, buffer, 199, 0);
				if (ret == 0)
				{
					//!< Déconnecté
					disconnect = true;
				}
				if (ret == SOCKET_ERROR)
				{
					int error = Sockets::GetError();
					if (error != static_cast<int>(Sockets::Errors::WOULDBLOCK))
					{
						disconnect = true;
					}
					//!< il n'y avait juste rien à recevoir
				}
				std::cout << "[" << clientAddress << ":" << clientPort << "]" << buffer << std::endl;
				ret = send(itClient->sckt, buffer, ret, 0);
				if (ret == 0 || ret == SOCKET_ERROR)
				{
					disconnect = true;
				}
				if (disconnect)
				{
					std::cout << "Deconnexion de [" << clientAddress << ":" << clientPort << "]" << std::endl;
					itClient = clients.erase(itClient);
				}
				else
					++itClient;
			}
		}
	}