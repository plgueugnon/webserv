#include "Aincludes.hpp"

#define NOT_ALLOWED " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					METHOD IS NOT ALLOWED SORRY !</h1> </body> </html>"
#define NOT_IMPLEMENTED " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					METHOD IS NOT IMPLEMENTED SORRY !</h1> </body> </html>"
				
#define CRLF "\r\n\r\n"


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
	std::string data = "";

	if (loc.autoindex.size() == 0)
	{
		if (loc.autoindex.compare("on") != 0 &&
			conf.autoindex.compare("on") != 0)
			return "";
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
		return "";
	}
	data += "<html>\n <head><title>Index of ";
	data += path;
	data += " folder.\n\n";
	data += " </title></head>\n <body>\n";
	data += "<h1>Index of ";
	data += path;
	data += " folder.\n\n</h1><hr><pre>";


	for (it = folder.begin(); it != folder.end(); it++)
	{
		data += "<a href=\"";
		data += *it;
		data += "\">";
		data += *it;
		data += "\n";
	}
	data += "</pre><hr></body>";
	data += "</ html>";
	 return data;
}

std::string response::getErrorPage ( std::vector<std::string> vec )
{
	// fileName = "";
	std::string data = "";
	std::string fileName = root;
	int errorCode;

	if (vec.size() == 0)
		return data;
	for (it = vec.begin(); it != vec.end() - 1; it++)
	{
		errorCode = atoi(it->c_str());
		if (code == errorCode)
		{
			fileName += *(vec.end() - 1);
			break;
		}
	}
	data = getDataFromFile(fileName);
	return (data);
}

void response::setCode(int code)
{
	this->code = code;
	if (output.size() == 0)
		output = getErrorPage(loc.error_page);
	if (output.size() == 0)
		output = getErrorPage(conf.error_page);
	switch (code)
	{
		case 200 :
		ret = CODE_200;
		break;
		case 204 :
		ret = CODE_204;
		if (output.size() == 0)
			output = "<html><body><h1>Couln't delete file.</h1></body></html>";
		break;
		case 400 : 
		ret = CODE_400;
		break ;
		case 401 : 
		ret = CODE_401;
		break ;
		case 402 : 
		ret = CODE_402;
		break ;
		case 403 : 
		ret = CODE_403;
		break ;
		case 404 : 
		ret = CODE_404;
		if (output.size() == 0)
			output = "<html><body><h1>Not found.</h1></body></html>";
		break;
		case 405 :
		ret = CODE_405;
		if (output.size() == 0)
			output = NOT_ALLOWED;
		break ;
		case 406 : 
		ret = CODE_406;
		break ;
		case 407 : 
		ret = CODE_407;
		break ;
		case 408 : 
		ret = CODE_408;
		break ;
		case 409 : 
		ret = CODE_409;
		break ;
		case 410 : 
		ret = CODE_410;
		break ;
		case 411 : 
		ret = CODE_411;
		break ;
		case 412 : 
		ret = CODE_412;
		break ;
		case 413 : 
		ret = CODE_413;
		break ;
		case 414 : 
		ret = CODE_414;
		break ;
		case 415 : 
		ret = CODE_415;
		break ;
		case 416 : 
		ret = CODE_416;
		break ;
		case 417 : 
		ret = CODE_417;
		break ;
		case 418 : 
		ret = CODE_418;
		break ;
		case 421 : 
		ret = CODE_421;
		break ;
		case 422 : 
		ret = CODE_422;
		break ;
		case 423 : 
		ret = CODE_423;
		break ;
		case 424 : 
		ret = CODE_424;
		break ;
		case 425 : 
		ret = CODE_425;
		break ;
		case 426 : 
		ret = CODE_426;
		break ;
		case 428 : 
		ret = CODE_428;
		break ;
		case 429 : 
		ret = CODE_429;
		break ;
		case 431 : 
		ret = CODE_431;
		break ;
		case 451 : 
		ret = CODE_451;
		break ;
		case 500 : 
		ret = CODE_500;
		break ;
		case 501 : 
		ret = CODE_501;
		if (output.size() == 0)
			output = NOT_IMPLEMENTED;
		break ;
		case 502 : 
		ret = CODE_502;
		break ;
		case 503 : 
		ret = CODE_503;
		break ;
		case 504 : 
		ret = CODE_504;
		break ;
		case 505 : 
		ret = CODE_505;
		break ;
		case 506 : 
		ret = CODE_506;
		break ;
		case 507 : 
		ret = CODE_507;
		break ;
		case 508 : 
		ret = CODE_508;
		break ;
		case 510 : 
		ret = CODE_510;
		break ;
		case 511 : 
		ret = CODE_511;
		break ;
	}
	ret += CRLF;
	ret += output;
	return ;
}

std::string response::getDataFromFile(std::string fileName)
{
	buffer = "";
	std::string data = "";

	file.open(fileName.c_str());

	if (file.is_open())
	{
		while (getline(file, buffer))
				data += buffer;
		file.close();
	}
	return data;
}

// void response::buildResponse( int code )
// {

// }

// filename = root + request path + index
void response::handleGet( void )
{

	if (isRedirected(&loc.return_dir) == true)
		return(redirectRequest(&loc.return_dir));
	if (isMethodAllowed("GET") == 0)
		return setCode(405);
	// if the last character is a /, it's a folder, so add index.
	if (req->requestLine[request::PATH].back() == '/')
		output = getDataFromFile(root + path + index);
	else
		output = getDataFromFile(root + path);
	if (output.size() == 0)
		output = getAutoIndex(root + path);
	// if fileName can't be open, return empty string -> size 0 = no index.
	// size = 0 means that autoindex couln't be generated
	// it means that the request is trying to get a regular file(not a folder)
	if (output.size() == 0)
		// code = 404;
		setCode(404);
	else 
		// code = 200;
		setCode(200);
	// if (code > 399 && code < 511)
	// 	output = getErrorPage(&loc.error_page);
	// if (code == 404)
	// 	setCode(404, CODE_404, output);
	return;
}

// https://www.cplusplus.com/reference/cstdio/remove/
void response::handleDelete ( void )
{
	if (isMethodAllowed("DELETE") == 0)
		return setCode(405);

	// if the file doesn't exist or can't be removed code 204
	if (remove((root + path).c_str()) != 0)
		setCode(204);
	// if file successfully deleted code 200
	else
		setCode(200);
	return;
}

void response::handlePost ( void )
{
	if (isMethodAllowed("POST") == 0)
		return setCode(405);
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

bool response::isMethodAllowed ( std::string method)
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
	// ret += CRLF;
	return ;

}

// extract request path
void response::setPath ( void )
{
	path = req->requestLine[request::PATH];
}

// set the index file according to the request path
void response::setIndex ( void )
{
	if (loc.index.size() == 0)
		index = conf.index;
	else
		index = loc.index;
	return ;
}

// set the root directory according to the request path root
void response::setRoot ( void )
{
	if (loc.root.size() == 0)
		root = conf.root;
	else
		root = loc.root;
	return ;
}

// find location path (from server config) that match request path
void response::setLocation ( void )
{
	for (	loc_it = conf.location.begin(); 
			loc_it != conf.location.end();
			loc_it++)
		if ( req->requestLine[request::PATH].compare(loc_it->path) == 0 )
			loc = *loc_it;
}

void response::parse ( void )
{

	// if all the server requests are redirected
	if (isRedirected(&conf.return_dir) == true)
		return(redirectRequest(&conf.return_dir));
	if (isMethodImplemented() == 0)
		return (setCode(501));

	setLocation();
	if (isRedirected(&loc.return_dir) == true)
		return(redirectRequest(&loc.return_dir));
	setRoot();
	setPath();
	setIndex();

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
