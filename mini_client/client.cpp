#include "client.hpp"

int	main(int argc, char **argv)
{
	int	sockfd = 0;
	SA	serv_addr;

	if (argc != 3)
	{
		std::cout << "usage = ./client + ip + port" << std::endl;
		return 0;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "error: failure to create socket" << std::endl;
		return -1;
	}

	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	if (serv_addr.sin_addr.s_addr == INADDR_NONE)
	{
		std::cerr << "error: failure to convert IP address with inet_addr" << std::endl;
		return -1;
	}

	serv_addr.sin_port = htons(atoi(argv[2])); 
	serv_addr.sin_family = AF_INET;

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "error: failure to connect socket" << std::endl;
		return -1;
	}
	std::cout << BLU"Connect successful\n"RST;

	char request[MAXLINE] = "GET / HTTP/1.1\nHost: example.com\r\n\r\n";
	if (send(sockfd, request, std::strlen(request), 0 ) == -1)
	{
		std::cerr << "error: failure to send request\n";
		return -1;
	}

	std::vector<unsigned char> buf(MAXLINE);
	// fcntl(sockfd, F_SETFL, O_NONBLOCK);
	ssize_t n = 0;

	// faire un ping sur example.com
	while ( (n = recv(sockfd, buf.data(), MAXLINE - 1, 0)) > 0)
	{
		std::cout << buf.data() << std::endl;
		if (n == 0 || n == EAGAIN)
			break;
		// std::cout << RED"check n = " << n << RST << std::endl;
		// break; // necessary has google will not stop the connection // will either block or loop forever if non blocking
	}
	if (n < 0)
	{
		std::cerr << "error: failure to receive request\n";
		close(sockfd);
		return -1;
	}

	// while(n)
	// {
	// 	n = recv(sockfd, buf.data(), buf.size() - 1, 0);
	// 	if (n < 0)
	// 	{
	// 		std::cerr << "error: failure to send request\n";
	// 		close(sockfd);
	// 		return -1;
	// 	}
	// 	// else if (n >= buf.capacity())
	// 	// 	buf.resize(n + 2);
	// 	std::cout << buf.data() << std::endl;
		// std::cout << "check n = " << n << std::endl; 
		// switch (n)
		// {
		// 	case -1:
		// 		std::cerr << "error: failure to send request\n";
		// 		return -1;
		// 	case 0:
		// 		break;
		// 	default:
		// 		if (n >= buf.size())
		// 		{
		// 			std::cout << "check n = " << n << " size = " << buf.size() << " cap = " << buf.capacity() << "\n";
		// 			// buf.reserve(n);
		// 			buf.resize(n * 2);
		// 		}
		// 		std::cout << "check n = " << n << " size = " << buf.size() << " cap = " << buf.capacity() << "\n";
		// 		std::cout << buf.data() << std::endl;
		// 		break;
		// }
	// }

	std::cout << "data received\n";
	close(sockfd);
	return 1;
}