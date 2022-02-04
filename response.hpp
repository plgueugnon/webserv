#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class response
{
	public :

	// conf
	request 		*req;
	t_server 		conf;

	// response
	int				code;
	std::fstream	file;
	std::string 	fileName;
	std::string 	buffer;
	std::string 	output;
	std::string 	ret;

	// iterator
	std::vector<std::string>::iterator it;
	// std::vector<std::string>::iterator end;

	// constructors
	response (void);
	response (request *request, t_server config);

	void setRoot ( t_location *loc);
	void setIndex ( t_location *loc);
	void setFile ( t_location *loc);
	void setCode ( int code, std::string codeMessage, std::string output );

	void parse ( void );
	void handleGet ( t_location *loc );
	void handleDelete ( t_location *loc );
	void handlePost (  t_location *loc );

	std::string getAutoIndex ( t_location *loc );
	std::string getErrorPage ( std::vector<std::string> *vec);
	std::string getDataFromFile(std::string fileName);

	void redirectRequest (std::vector<std::string> *vec);

	bool isMethodAllowed (t_location *loc, std::string method);
	bool isMethodImplemented ( void );
	bool isRedirected (std::vector<std::string> *vec);
};
int	receive_request(int client_sock, t_http config);
void	manage_request(int client_sock, request *request, t_server config);
#endif /* RESPONSE_HPP */
