#include "client.hpp"

int	main(int argc, char **argv)
{
	int	sockfd = 0;
	SA	serv_addr;
	std::ifstream ifs;
	std::vector<std::string> requests;

	// ! Arg check upon entry
	if (argc < 4)
	{
		std::cout << YEL"Usage = ./client + target IP + port + path to http request file(s)\n"RST;
		return 0;
	}

	// ! Stacking requests
	for (int i = 3; i < argc; i++)
	{
		std::string content;
		std::string line;
		ifs.open(argv[i], std::ifstream::in);
		if (ifs.is_open())
		{
			while (std::getline(ifs, line))
			{
				content += line;
				content += "\n";
			}
		}
		else
		{
			std::cerr << RED"error: invalid file or path " << argv[i] << "\n"RST;
			return 1;
		}
		requests.push_back(content);
		ifs.close();
	}

	if (DEBUG_TEST)
	{
		for (std::vector<std::string>::iterator it = requests.begin(); it != requests.end(); it++)
			std::cout << BLU"\nrequest is : \n"RST << *it << " on address : " << argv[1] << ":" << argv[2] << "\n";
	}

	// ! request loop
	for (std::vector<std::string>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cerr << RED"error: failure to create socket\n"RST;
			return 1;
		}

		// ? CHECK SI IP ET PORT PEUVENT ETRE SET HORS LOOP
		serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
		if (serv_addr.sin_addr.s_addr == INADDR_NONE)
		{
			std::cerr << YEL"error: IP address" << argv[1] << " is invalid\n"RST;
			return 1;
		}

		int port = std::atoi(argv[2]);
		if (port < 0 || port > 65535)
		{
			std::cerr << YEL"error: Port number " << argv[2] << " is invalid\n"RST;
			return 1;
		}
		serv_addr.sin_port = htons(atoi(argv[2])); 
		serv_addr.sin_family = AF_INET;


		// ? A priori connect obligatoire pour chaque nouvelle requete - pas possible hors loop
		if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			std::cerr << RED"error: failure to connect socket"RST << std::endl;
			return -1;
		}
		std::cout << GRE"Connect successful\n"RST;

		if (send(sockfd, it->c_str(), strlen(it->c_str()), 0 ) == -1)
		{
			std::cerr << RED"error: failure to send request\n"RST;
			return 1;
		}

		std::vector<unsigned char> buf(MAXLINE);
		ssize_t n = 0;

		while ( (n = recv(sockfd, buf.data(), MAXLINE - 1, 0)) > 0)
		{
			std::cout << buf.data() << std::endl;
			if (n == 0 || n == EAGAIN)
				break;
		}
		if (n < 0)
		{
			std::cerr << RED"error: failure to receive request\n"RST;
			close(sockfd);
			return 1;
		}

		std::cout << BLU"data received\n"RST;
		close(sockfd);
	}
	return 0;
}