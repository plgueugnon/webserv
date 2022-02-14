#include "Aincludes.hpp"

#define NOT_ALLOWED " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					NOT ALLOWED SORRY !</h1> </body> </html>"
#define NOT_IMPLEMENTED " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					METHOD IS NOT IMPLEMENTED SORRY !</h1> </body> </html>"
#define PAYLOAD_TOO_LARGE " <!DOCTYPE html> <html> <body><h1> \
					<h1 style=\"color:red;\"> \
					Payload is too large. Modify client_max_body_size \
					 !</h1> </body> </html>"

#define NOT_FOUND "<html><body><h1>Not found.</h1></body></html>"
#define CANT_DELETE "<html><body><h1>Couln't delete file.</h1></body></html>"
#define FILE_CREATED "<html><body><h1>File successfuly uploaded.</h1></body></html>"
#define URI_TOO_LONG "<html><body><h1>URI is too long.</h1></body></html>"
#define HEADERS_TOO_LONG "<html><body><h1>Header field is too long.</h1></body></html>"

#define CGI_BIN "./cgi/darwin_phpcgi"

#define URI_SIZE_LIMIT 256
#define HEADERS_SIZE_LIMIT 1096


response::response ( void )
{
}

response::response (request request, t_server config)
{
	req = request;
	conf = config;
	code = 0;
	buffer = "";
	output = "";
	ret = "";
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

void response::setCGIfd (int client_read_fd[2], int client_write_fd[2])
{
	this->read_fd[0] = client_read_fd[0];
	this->read_fd[1] = client_read_fd[1];
	this->write_fd[0] = client_write_fd[0];
	this->write_fd[1] = client_write_fd[1];
}

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
		}
		closedir(dir);
	}
	else
		return "";
	data += "<html>\n <head><title>Index of ";
	data += path;
	data += " folder.\n\n </title></head>\n <body>\n<h1>Index of ";
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
	std::string data 		= "";
	std::string fileName 	= root;

	int 		errorCode;

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
		// ! add 201 create when upload file
		case 201 :
		ret = CODE_201;
		if (output.size() == 0)
			output = FILE_CREATED;
		break;
		case 204 :
		ret = CODE_204;
		if (output.size() == 0)
			output = CANT_DELETE;
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
			output = NOT_FOUND;
		break;
		case 405 :
		ret = CODE_405;
		if (output.size() == 0)
			output = NOT_ALLOWED;
		break ;
		case 413 :
		ret = CODE_413;
		// if (output.size() == 0)
		output = PAYLOAD_TOO_LARGE;
		break ;
		case 414 :
		ret = CODE_414;
		// if (output.size() == 0)
		output = URI_TOO_LONG;
		break ;
		case 431 :
		ret = CODE_431;
		// if (output.size() == 0)
		output = HEADERS_TOO_LONG;
		break ;
		case 500 : 
		ret = CODE_500;
		break ;
		case 501 :
		ret = CODE_501;
		if (output.size() == 0)
			output = NOT_IMPLEMENTED;
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

// filename = root + request path + index
void response::handleGet( void )
{

	if (isRedirected(&conf.return_dir) == true)
		return(redirectRequest(&conf.return_dir));
	if (isRedirected(&loc.return_dir) == true)
		return(redirectRequest(&loc.return_dir));
	if (isMethodAllowed("GET") == false)
		return setCode(405);
	// if the last character is a /, it's a folder, so add index.
	if (req.requestLine[request::PATH].back() == '/')
		output = getDataFromFile(root + path + index);
	else
		output = getDataFromFile(root + path);
	if (output.size() == 0)
		output = getAutoIndex(root + path);
	// if fileName can't be open, return empty string -> size 0 = no index.
	// size = 0 means that autoindex couln't be generated
	// it means that the request is trying to get a regular file(not a folder)
	if (output.size() == 0)
		setCode(404);
	else
		setCode(200);
	return;
}

// https://www.cplusplus.com/reference/cstdio/remove/
// if the file doesn't exist or can't be removed code 204
// if file successfully deleted code 200
void response::handleDelete ( void )
{
	if (isMethodAllowed("DELETE") == false)
		return setCode(405);

	if (remove((root + path).c_str()) != 0)
		setCode(204);
	else
		setCode(200);
	return;
}


// * https://www.unix.com/programming/58138-c-how-use-pipe-fork-stdin-stdout-another-program.html
void response::handlePost ( void )
{
	if (isMethodAllowed("POST") == false)
		return setCode(405);
	if (isBodyTooLarge() == true)
		return setCode(413);

	// ! CGI env
	cgi cgi;
	// config
	cgi.env[cgi::SERVER_NAME] 			+= conf.server_name;
	cgi.env[cgi::SERVER_PORT] 			+= conf.listen;

	//request Line
	cgi.env[cgi::REQUEST_METHOD] 		+= req.requestLine[request::METHOD];
	cgi.env[cgi::QUERY_STRING] 			+= req.requestLine[request::QUERY];

	// headers
	cgi.env[cgi::CONTENT_LENGTH] 		+= req.header[request::CONTENT_LENGTH];
	cgi.env[cgi::CONTENT_TYPE] 			+= req.header[request::CONTENT_TYPE];
	cgi.env[cgi::HTTP_ACCEPT] 			+= req.header[request::ACCEPT];
	cgi.env[cgi::HTTP_ACCEPT_LANGUAGE] 	+= req.header[request::ACCEPT_LANGUAGE];
	cgi.env[cgi::HTTP_USER_AGENT] 		+= req.header[request::USER_AGENT];
	
	// tmp dir to upload a file whith php script
	cgi.env[cgi::TMP_DIR] 				+= getenv("PWD") + (std::string)"/";
	cgi.env[cgi::TMP_DIR] 				+= root + (std::string)"/tmp/";


	// script to be executed by CGI
	cgi.env[cgi::SCRIPT_FILENAME] += getenv("PWD") + (std::string)"/";
	cgi.env[cgi::SCRIPT_FILENAME] += root + path;
	// if (req.requestLine[request::PATH].back() == '/')
	// 	cgi.env[cgi::SCRIPT_FILENAME] += index;

	// std::cout << RED"scriptFilename :" << cgi.env[cgi::SCRIPT_FILENAME] << "\n"RESET;

	cgi.convertToC();
	print_env_c(cgi.c_env);
	pid_t	pid;
	char	*argv[3];
	char	buffer[PIPE_BUFFER_SIZE];
	int		r = 0;
	int		w = 0;

	if ((pid = fork()) == -1 )
	{
		std::cerr << RED"error : fork failure\n"RESET;
		close(write_fd[1]);
		close(read_fd[0]);
		close(write_fd[0]);
		close(read_fd[1]);
		setCode(500);
		return ;
	}
	argv[0] = strdup(CGI_BIN);
	argv[1] = strdup(cgi.env[cgi::SCRIPT_FILENAME].c_str());
	argv[2] = NULL;
	if (pid == 0)
	{
		close(write_fd[1]);
		close(read_fd[0]);
		if (dup2(read_fd[1], STDOUT_FILENO) < 0)
		{
			std::cerr << RED"error : dup2 failure\n"RESET;
			close(write_fd[0]);
			close(read_fd[1]);
			return ;
		}
		if (dup2(write_fd[0], STDIN_FILENO) < 0)
		{
			std::cerr << RED"error : dup2 failure\n"RESET;
			close(write_fd[0]);
			close(read_fd[1]);
			return ;
		}
		if (execve(argv[0], argv, cgi.c_env) < 0)
		{
			std::cerr << RED"error : execve failure\n"RESET;
			close(write_fd[0]);
			close(read_fd[1]);
			kill(pid, SIGTERM);
		}
	}
	else
	{
		close(write_fd[0]);
		close(read_fd[1]);
		waitpid(pid, NULL, WNOHANG);

		w = write(write_fd[1], req.body.c_str(), req.body.size());
		switch (w)
		{
			case -1:
				std::cerr << RED"error : cgi write failure\n"RESET;
				break;
			case 0:
				if (VERBOSE)
					std::cout << YELLOW"warning: no data input sent to cgi\n"RESET;
				break;
			default:
				break;
		}
		close(write_fd[1]);

		while((r = read(read_fd[0], buffer, sizeof(buffer))) > 0)
		{
			buffer[r] = 0;
			output += buffer;
			bzero(buffer, sizeof(buffer));
		}
		if (r == -1)
			std::cerr << RED"error : cgi read failure\n"RESET;
		close(read_fd[0]);
	}
	free(argv[0]);
	free(argv[1]);
	if (w < 0 || r < 0)
		setCode(500);
	else if (output.size() == 0)
		setCode(404);
	else
		setCode(200);

	return;
}

// ! add meilleur parsing d'erreur pour redirect only code 30x et 2 args args
// okay dans le parsing fichier de config normalement
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
	ret += CRLF;
	return ;

}

// find location path (from server config) that match request path
void response::setLocation ( void )
{
	for (	loc_it = conf.location.begin();
			loc_it != conf.location.end();
			loc_it++)
		if ( req.requestLine[request::PATH].compare(0, loc_it->path.size(), loc_it->path) == 0 )
			loc = *loc_it;
}

// extract request path
void response::setPath ( void )
{
	path = req.requestLine[request::PATH];
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

// ! BOOL
bool response::isMethodAllowed ( std::string method)
{
	if (loc.limit_except.size() == 0)
		return (true);
	for (it = loc.limit_except.begin(); it != loc.limit_except.end(); it++)
		if ((*it).compare(method) == 0)
			return (true);
	return (false);
}

bool response::isRedirected (std::vector<std::string> *vec)
{
	if (vec->size() == 0)
		return (false);
	return (true);
}

bool response::isMethodImplemented(void)
{
	if ((req.requestLine[request::METHOD]).compare(0, 3, "GET") != 0 &&
		(req.requestLine[request::METHOD]).compare(0, 4, "POST") != 0 &&
		(req.requestLine[request::METHOD]).compare(0, 6, "DELETE") != 0)
		return (false);
	else
		return (true);
}

bool response::isBodyTooLarge(void)
{
	size_t limitSize = loc.client_max_body_size;
	// std::cout << RED"limit size: " << limitSize << "\n"RESET;
	// std::cout << RED"body size: " << req.body.size() << "\n"RESET;
	if ( limitSize > req.body.size() )
		return (false);
	return (true);
}

bool response::isChunked(void)
{
	if (req.header[request::TRANSFER_ENCODING].compare(0, 7, "chunked") == 0)
		return (true);
	return (false);
}

bool response::isURITooLong(void)
{
	if (path.size() > URI_SIZE_LIMIT)
		return (true);
	return (false);
}

void response::parse ( void )
{
	// if all the server requests are redirected
	if (isMethodImplemented() == false)
		return (setCode(501));
	if (isChunked() == true)
		return (setCode(501));
	if (isURITooLong() == true)
		return (setCode(414));
	if (req.headerSize > HEADERS_SIZE_LIMIT)
		return (setCode(431));

	if ( (req.requestLine[request::METHOD]).compare("GET") == 0 )
		handleGet();
	else if ( (req.requestLine[request::METHOD]).compare("DELETE") == 0 )
		handleDelete();
	else if ( (req.requestLine[request::METHOD]).compare("POST") == 0 )
		handlePost();
}
