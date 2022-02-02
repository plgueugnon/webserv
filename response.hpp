#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class response
{
	public :

	request 	*req;
	t_server 	conf;
	std::string ret;

	// constructors
	response (void);
	response (request *request, t_server config);

	void parse ( void );
	void handleGet ( t_location *loc );
	void handleDelete ( t_location *loc );
	void handlePost (  t_location *loc );
	void setCode ( std::string code, std::string output );
	std::string autoIndex ( t_location *loc );


};
int	receive_request(int client_sock, t_http config);
void	manage_request(int client_sock, request *request, t_server config);
#endif /* RESPONSE_HPP */
