#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class response
{
	public :

	response (void);


};
void	receive_request(int client_sock, t_http config);
void	manage_request(int client_sock, request *request, t_http config);
#endif /* RESPONSE_HPP */
