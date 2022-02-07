#include "Aincludes.hpp"

#define NOT_ALLOWED " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					METHOD IS NOT ALLOWED SORRY !</h1> </body> </html>"
#define NOT_IMPLEMENTED " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					METHOD IS NOT IMPLEMENTED SORRY !</h1> </body> </html>"
				
// #define CRLF "\r\n\r\n"

// TODO add file deleted message with Code 200 et 204
// <html>
//   <body>
//     <h1>File deleted.</h1>
//   </body>
// </html>


response::response ( void ) 
{
}

response::response (request *request, t_server config)
{
	req = request;
	conf = config;
	// loc = 0;
	code = 0;
	// file = {0};
	// fileName = "";
	buffer = "";
	output = "";
	ret = "";
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
std::string response::getAutoIndex( std::string fileName )
{
	std::vector<std::string> folder;

	DIR 			*dir;
	struct dirent 	*ent;

	// fileName = "";
	// if (loc.root.size() == 0)
	// 	fileName += conf.root;
	// else
	// 	fileName += loc.root;
	std::cout << "data filename: " << fileName << "\n";

	// fileName += req->requestLine[request::PATH];
	if (loc.autoindex.size() == 0)
	{
		if (loc.autoindex.compare("on") != 0 &&
			conf.autoindex.compare("on") != 0)
			return output;
	}
	if ((dir = opendir(fileName.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			buffer = ent->d_name;
			// add / if it's a directory
			if (ent->d_type == DT_DIR)
				buffer += "/";
			folder.push_back(buffer);
			// printf("%s\n", ent->d_name);
		}
		closedir(dir);
	}
	else
	{
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

std::string response::getErrorPage ( std::vector<std::string> *vec )
{
	// fileName = "";
	std::string data = "";
	std::string fileName = root;
	int errorCode;

	// if (loc.root.size() == 0)
	// 	fileName += conf.root;
	// else 
	// 	fileName += loc.root;

	for (it = vec->begin(); it != vec->end() - 1; it++)
	{
		errorCode = atoi(it->c_str());
		if (code == errorCode)
		{
			fileName += *(vec->end() - 1);
			break;
		}
			// std::cout << "error code\n";
	}
	std::cout << "data filename" << fileName << "\n";
	std::cout << "file name :" << fileName<< '\n';
	data = getDataFromFile(fileName);
	return (data);
}

void response::setCode(int code, std::string codeMessage, std::string output)
{
	this->code = code;
	ret += codeMessage;
	// ret += CRLF;
	ret += "\r\n\r\n";
	ret += output;
	return ;
}

std::string response::getDataFromFile(std::string fileName)
{
	buffer = "";
	std::string data = "";

	file.open(fileName.c_str());
	std::cout << "data filename: " << fileName << "\n";

	if (file.is_open())
	{
		while (getline(file, buffer))
				data += buffer;
		file.close();
	}
	return data;
}

// filename = root + request path + index
void response::handleGet( void )
{

	if (isRedirected(&loc.return_dir) == true)
		return(redirectRequest(&loc.return_dir));
	if (isMethodAllowed(loc, "GET") == 0)
		return setCode(405, CODE_405, NOT_ALLOWED);
	// fileName += req->requestLine[request::PATH];
	// if the last character is a /, it's a folder, so add index.
	if (req->requestLine[request::PATH].back() == '/')
		output = getDataFromFile(root + path + index);
	else
		output = getDataFromFile(root + path);
	if (output.size() == 0)
		output = getAutoIndex(root + path);
		// setIndex();
	// if fileName can't be open, return empty string -> size 0 = no index.
	// output = getDataFromFile(fileName);
	// size = 0 means that autoindex couln't be generated
	// it means that the request is trying to get a regular file(not a folder)
	// if (output.size() == 0)
	// 	output = getAutoIndex();
	if (output.size() == 0)
		// couln't find file nor autoindex -> error 404
		setCode(404, CODE_404, output);
	else 
		// regular file successfuly open -> code 200
		setCode(200, CODE_200, output);
	// if (code > 399 && code < 511)
	// 	output = getErrorPage(&loc.error_page);
	// if (code == 404)
	// 	setCode(404, CODE_404, output);
	return;
}

// https://www.cplusplus.com/reference/cstdio/remove/
void response::handleDelete ( void )
{
	if (isMethodAllowed(loc, "DELETE") == 0)
		return setCode(405, CODE_405, NOT_ALLOWED);
	// fileName += req->requestLine[request::PATH];

	if (remove((root + path).c_str()) != 0)
		setCode(204, CODE_204, output);
	else
		setCode(200, CODE_200, output);
	return;
}

void response::handlePost ( void )
{
	if (isMethodAllowed(loc, "POST") == 0)
		return setCode(405, CODE_405, NOT_ALLOWED);
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

	// char path[] = "./cgi/php-cgi_vMojave";
	// int		fd[2];
	// pid_t	pid;
	// int cfd = 0;

	// pipe(fd);
	// pid = fork();
	// if (pid == -1)
	// 	std::cerr << RED"error : fork failure\n"RESET;
	// else if (pid == 0)
	// {
	// 	if (dup2(fd[1], STDOUT) < 0)
	// 	{
// 		std::cerr << RED"error : dup2 failure\n"RESET;
// 		return ;
// 	}
	
// }


// vec_enum(cgi.env);
// cgi.convertToC();
// print_env_c(cgi.c_env);
return;
}

bool response::isRedirected (std::vector<std::string> *vec)
{
	if (vec->size() == 0)
		return (false);
	return (true);
}

bool response::isMethodAllowed (t_location loc, std::string method)
{
	if (loc.limit_except.size() == 0)
		return (true);
	for (it = loc.limit_except.begin(); it != loc.limit_except.end(); it++)
		if ((*it).compare(method) == 0)
			return (true);
	return (false);
}


bool response::isMethodImplemented(void)
{
	if ((req->requestLine[request::METHOD]).compare("GET") != 0 &&
		(req->requestLine[request::METHOD]).compare("POST") != 0 &&
		(req->requestLine[request::METHOD]).compare("DELETE") != 0)
		return (false);
	return (true);
}

// ! add meilleur parsing d'erreur pour redirect only code 30x et 2 args args
void response::redirectRequest (std::vector<std::string> *vec)
{
	 it = vec->begin();

	code = atoi(it->c_str());
	it++;
	std::string  redirectUrl = *it;
	if (code == 300)
		ret += CODE_300;
	else if (code == 301)
		ret += CODE_301;
	else if (code == 302)
		ret += CODE_302;
	else if (code == 303)
		ret += CODE_303;
	else if (code == 304)
		ret += CODE_304;
	else if (code == 305)
		ret += CODE_305;
	else if (code == 306)
		ret += CODE_306;
	else if (code == 307)
		ret += CODE_307;
	else if (code == 308)
		ret += CODE_308;
	ret += "\nLocation: ";
	ret += redirectUrl;
	ret += "\r\n\r\n";
	return ;

}

void response::setPath ( void )
{
	path = req->requestLine[request::PATH];
}

void response::setIndex ( void )
{
	if (loc.index.size() == 0)
		index = conf.index;
	else
		index = loc.index;
	return ;
}

void response::setRoot ( void )
{
	std::cout << "root :" << loc.root.size() << "\n";
	if (loc.root.size() == 0)
		root = conf.root;
	else
		root = loc.root;
	return ;
}

void response::parse ( void )
{
	// if all the server requests are redirected
	if (isRedirected(&conf.return_dir) == true)
		return(redirectRequest(&conf.return_dir));
	if (isMethodImplemented() == 0)
		return (setCode(501, CODE_501, NOT_IMPLEMENTED));

	// find location path (from server config) that match request path
	for (loc_it = conf.location.begin(); loc_it != conf.location.end(); loc_it++)
	{
		if ( req->requestLine[request::PATH].compare(loc_it->path) == 0 )
			loc = *loc_it;
	}
	// printLocationConfig
	// printLocation(&tmp);
	setRoot();
	setPath();
	setIndex();
	// set filename


	if ( (req->requestLine[request::METHOD]).compare("GET") == 0 )
		handleGet();
	else if ( (req->requestLine[request::METHOD]).compare("DELETE") == 0 )
		handleDelete();
	else if ( (req->requestLine[request::METHOD]).compare("POST") == 0 )
		handlePost();
	// if (output.size() == 0)
	// 	output = getErrorPage(&conf.error_page);
	// return response;
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
	answer += response.ret;
	std::cout << "mdr\n";
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
