#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class response
{
	public :

	// conf
	request 		*req;
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

	// iterator
	std::vector<std::string>::iterator 	it;
	std::vector<t_location>::iterator	loc_it;
	// std::vector<std::string>::iterator end;

	// constructors
	response (void);
	response (request *request, t_server config);

	void setRoot ( void );
	void setPath ( void );
	void setIndex ( void );
	void setFile ( void );
	void setCode ( int code, std::string codeMessage, std::string output );

	void parse ( void );
	void handleGet ( void );
	void handleDelete ( void );
	void handlePost (  void );

	std::string getAutoIndex ( std::string fileName );
	std::string getErrorPage (std::vector<std::string> *vec );
	std::string getDataFromFile(std::string fileName);

	void redirectRequest (std::vector<std::string> *vec);

	bool isMethodAllowed (t_location loc, std::string method);
	bool isMethodImplemented ( void );
	bool isRedirected (std::vector<std::string> *vec);
};
int	receive_request(int client_sock, t_http config);
void	manage_request(int client_sock, request *request, t_server config);
#endif /* RESPONSE_HPP */
