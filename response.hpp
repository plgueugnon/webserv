#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#define PIPE_BUFFER_SIZE 10000

class response
{
	public :

	// conf
	request 		req;
	t_server 		conf;
	t_location 		loc;

	// response
	int				code;
	std::fstream	file;
	// std::string 	fileName;
	std::string 	root;
	std::string 	path;
	std::string 	index;
	std::string 	buffer;
	std::string 	output;
	std::string 	ret;
	int				read_fd[2];
	int				write_fd[2];

	// iterator
	std::vector<std::string>::iterator 	it;
	std::vector<t_location>::iterator	loc_it;
	// std::vector<std::string>::iterator end;

	// constructors
	response (void);
	response (request request, t_server config);

	void setCGIfd (int client_read_fd[2], int client_write_fd[2]);
	void closeAllfd ( void );

	void setRoot ( void );
	void setPath ( void );
	void setIndex ( void );
	void setFile ( void );
	void setLocation ( void );
	void setCode ( int code);

	void parse ( void );
	void handleGet ( void );
	void handleDelete ( void );
	void handlePost (  void );

	std::string getAutoIndex ( std::string fileName );
	std::string getErrorPage (std::vector<std::string> vec );
	std::string getDataFromFile(std::string fileName);

	void redirectRequest (std::vector<std::string> *vec);

	bool isMethodAllowed (std::string method);
	bool isMethodImplemented ( void );
	bool isRedirected (std::vector<std::string> *vec);
	bool isBodyTooLarge ( void );
	bool isChunked ( void );
	bool isURITooLong(void);
};

#endif /* RESPONSE_HPP */
