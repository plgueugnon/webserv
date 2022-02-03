#include "Aincludes.hpp"

#define NOT_ALLOWED " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					METHOD IS NOT ALLOWED SORRY !</h1> </body> </html>"
#define NOT_IMPLEMENTED " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					METHOD IS NOT IMPLEMENTED SORRY !</h1> </body> </html>"

response::response ( void ) 
{
}

response::response (request *request, t_server config)
{
	req = request;
	conf = config;
	ret = "";
	code = 0;
	// req->printRequest();
}
/*
 check if the request path match a location block
// // if the request path match a location block, 
 check if there is a return directive
	return the code
 // // check if the method is allowed in this location block
// // change root and index and autoindex
 // //if the path is a directory begin and finish with /
 // //return auto index
 // //if not, not found
 check if there is error_pages for this location
 */
std::string response::autoIndex(t_location *loc)
{
	std::vector<std::string> folder;
	std::vector<std::string>::iterator it;
	std::string 	output = "";
	std::string 	fileName = "";
	std::string 	root = "";
	std::string 	tmp = "";
	DIR 			*dir;
	struct dirent 	*ent;

	if (loc->root.size() == 0)
		root += conf.root;
	else
		root += loc->root;

	fileName += root;
	fileName += req->requestLine[request::PATH];
	// std::cout << RED"autoindex index : " << fileName << "\n"RESET;
	if (loc->autoindex.size() == 0)
	{
		if (loc->autoindex.compare("on") != 0 &&
			conf.autoindex.compare("on") != 0)
			return output;
	}
	// std::cout << RED"autoindex index : " << fileName << "\n"RESET;
	if ((dir = opendir(fileName.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL)
		{
			tmp = ent->d_name;
			if (ent->d_type == DT_DIR)
				tmp += "/";
			folder.push_back(tmp);
			// printf("%s\n", ent->d_name);
		}
		closedir(dir);
	}
	else
	{
		/* could not open directory */
		std::cerr << RED"can't open directory\n"RESET;
		return output;
	}
	output += "<html>\n <head><title>Index of ";
	output += req->requestLine[request::PATH];
	output += " folder.\n\n";
	output += " </title></head>\n <body>\n";
	output += "<h1>Index of ";
	output += req->requestLine[request::PATH];
	output += " folder.\n\n</h1><hr><pre>";


	for (it = folder.begin(); it != folder.end(); it++)
	{
		output += "<a href=\"";
		output += *it;
		output += "\">";
		output += *it;
		output += "\n";
	}
	output += "</pre><hr></body>";
	output += "</ html>";
	 return output;
}

void response::setCode(std::string code, std::string output)
{
	ret += code;
	ret += "\r\n\r\n";
	ret += output;
	return ;
}

bool response::isRedirected (std::vector<std::string> vec)
{
	(void)vec;
	return 1;

}

bool response::methodIsAllowed (t_location *loc, std::string method)
{
	std::vector<std::string>::iterator it;

	if (loc->limit_except.size() == 0)
		return (1);
	for (it = loc->limit_except.begin(); it != loc->limit_except.end(); it++)
	{
		if ((*it).compare(method) == 0)
			return (1);
	}
	return (0);
}

// filename = root + request path + index
void response::handleGet(t_location *loc)
{
	std::fstream file;
	std::string fileName = "";
	std::string line = "";
	std::string output = "";

	if (methodIsAllowed(loc, "GET") == 0)
		return setCode(CODE_405, NOT_ALLOWED);
	if (loc->root.size() == 0)
		fileName += conf.root;
	else 
		fileName += loc->root;
	fileName += req->requestLine[request::PATH];
	if (req->requestLine[request::PATH].back() == '/')
	{
		if (loc->index.size() == 0)
			fileName += conf.index;
		else
			fileName += loc->index;
	}
	file.open(fileName.c_str());

	if (file.is_open())
	{
		while (getline(file, line))
				output += line;
		file.close();
	}
	else
		output += autoIndex(loc);
	// std::cout << output << std::endl;
	if (output.size() == 0)
		setCode(CODE_404, output);
	else 
		setCode(CODE_200, output);
	return;
}

// https://www.cplusplus.com/reference/cstdio/remove/
void response::handleDelete ( t_location *loc )
{
	std::fstream file;
	std::string fileName = "";
	std::string output = "";

	if (methodIsAllowed(loc, "DELETE") == 0)
		return setCode(CODE_405, NOT_ALLOWED);
	if (loc->root.size() == 0)
		fileName += conf.root;
	else 
		fileName += loc->root;

	fileName += req->requestLine[request::PATH];
	// if (req->requestLine[request::PATH].back() == '/')
	// {
	// 	if (loc->index.size() == 0)
	// 		fileName += conf.index;
	// 	else
	// 		fileName += loc->index;
	// }
	if (remove(fileName.c_str()) != 0)
	{
		perror("Error deleting file");
		setCode(CODE_404, output);
	}
	else
	{
		setCode(CODE_200, output);
		puts("File successfully deleted");
	}

	return;
}

void response::handlePost ( t_location *loc )
{
	if (methodIsAllowed(loc, "POST") == 0)
		return setCode(CODE_405, NOT_ALLOWED);
	(void) loc;
	// ! CGI env
	cgi cgi;
	// cgi.env[cgi::SERVER_NAME] += conf.server_name;
	// cgi.env[cgi::SERVER_PORT] += conf.listen;
	// cgi.env[cgi::REQUEST_METHOD] += req->requestLine[request::METHOD];
	// cgi.env[cgi::CONTENT_TYPE] += req->requestLine[request::CONTENT_TYPE];
	// cgi.env[cgi::CONTENT_LENGTH] += req->requestLine[request::CONTENT_LENGTH];
	// cgi.env[cgi::HTTP_ACCEPT] += req->requestLine[request::ACCEPT];
	// cgi.env[cgi::HTTP_ACCEPT_LANGUAGE] += req->requestLine[request::ACCEPT_LANGUAGE];
	// cgi.env[cgi::HTTP_ACCEPT] += req->requestLine[request::ACCEPT];
	// cgi.env[cgi::HTTP_USER_AGENT] += req->header[request::USER_AGENT];

	char path[] = "./cgi/php-cgi_vMojave";
	int		fd[2]
	pid_t	pid;
	int cfd = 0;

	pipe(fd);
	pid = fork()
	if (pid == -1)
		std::cerr << RED"error : fork failure\n"RESET;
	else if (pid == 0)
	{
		if (dup2(fd[1], STDOUT) < 0)
		{
			std::cerr << RED"error : dup2 failure\n"RESET;
			return ;
		}
		
	}


	vec_enum(cgi.env);
	cgi.convertToC();
	print_env_c(cgi.c_env);
	return;
}

bool response::methodIsImplemented(void)
{
	if ((req->requestLine[request::METHOD]).compare("GET") != 0 &&
		(req->requestLine[request::METHOD]).compare("POST") != 0 &&
		(req->requestLine[request::METHOD]).compare("DELETE") != 0)
		return (false);
	return (true);
}

void response::parse ( void )
{
	std::vector<t_location>::iterator	loc_it;

	t_location 							tmp;

	if (methodIsImplemented() == 0)
		return (setCode(CODE_501, NOT_IMPLEMENTED));

	// req->printRequest();
	// if all the server requests are redirected
	if (isRedirected(conf.return_dir) == 1)
	// find location path (from server config) that match request path
	for (loc_it = conf.location.begin(); loc_it != conf.location.end(); loc_it++)
	{
		if ( req->requestLine[request::PATH].compare(loc_it->path) == 0 )
			tmp = *loc_it;
	}
	// printLocationConfig
	// printLocation(&tmp);

	if ( (req->requestLine[request::METHOD]).compare("GET") == 0 )
		handleGet(&tmp);
	else if ( (req->requestLine[request::METHOD]).compare("DELETE") == 0 )
		handleDelete(&tmp);
	else if ( (req->requestLine[request::METHOD]).compare("POST") == 0 )
		handlePost(&tmp);
}


void	answer_client(int client_sock, std::string answer)
{
	if (send(client_sock, answer.c_str(), answer.size(), 0) == -1)
	{
		std::cerr << YELLOW"error: failure to send answer\n"RESET;
		close(client_sock);
		return ;
	}
	// ! Par défaut pas de connection keep-alive
	// ? A implementer ?

	// if (VERBOSE)
	// 	std::cout << GREEN"Closing connection with client\n"RESET;
	close(client_sock);
}

// TODO créer parsing complet et remplissage dynamique de la réponse à donner
// ? Problème si le statut disponible en écriture du client pas vérifié ?
void	manage_request(int client_sock, request *request, t_server config)
{
	response 	response(request, config);
	std::string	answer = "";
	(void)config;
	response.parse();
	// if (request->requestLine[request::METHOD].compare(0, 3, "GET") != 0)
	// 	answer.assign("HTTP/1.1 400 Bad Request\r\n\r\n");
	// else
	// 	answer.assign("HTTP/1.1 200 OK\r\n\r\n Wesh ma gueule, bien ou bien !?");
	answer += response.ret;
	answer_client(client_sock, answer);
}


// TODO 1. Check si Bad Request = check method, puis store path et check protocol
// TODO 2. Check header validity et update cgi env
// TODO 2bis pasrse n store request uri
// TODO 3ter si POST method -> check transfer method (MIME or HTML form)
// TODO 3. go to response

// ! Response
// TODO 1. check selon method -> GET -> file/path exist ? -> DELETE = idem ? et POST -> A voir
// TODO 2. Check si appel cgi et storage contenu renvoyé
// TODO 3. Assemblage reponse (entete + body) et send
