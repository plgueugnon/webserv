#ifndef CGI_HPP
#define CGI_HPP

class cgi
{
	public :
	enum index
	{
		SERVER_SOFTWARE,
		SERVER_NAME,
		GATEWAY_INTERFACE,
		SERVER_PROTOCOL,
		SERVER_PORT,
		REQUEST_METHOD,
		PATH_INFO,
		PATH_TRANSLATED,
		SCRIPT_NAME,
		QUERY_STRING,
		REMOTE_HOST,
		REMOTE_ADDR,
		AUTH_TYPE,
		REMOTE_USER,
		REMOTE_IDENT,
		CONTENT_TYPE,
		CONTENT_LENGTH,
		HTTP_ACCEPT,
		HTTP_ACCEPT_LANGUAGE,
		HTTP_USER_AGENT,
		HTTP_COOKIE,
		HTTP_REFERER,
		REDIRECT_STATUS,
		SCRIPT_FILENAME,
		TMP_DIR,
		NB_CGI_VAR,
	};
	
	 std::vector<std::string> 	env;
	 char 				*c_env[NB_CGI_VAR + 1];
	 cgi ( void );
	 void convertToC ( void );
};
/**
 Liste des variables CGI

//! Variables en rapport avec le serveur
 @param SERVER_SOFTWARE 	: Le nom et la version du serveur HTTP répondant à la requête. (Format : nom/version)
 @param SERVER_NAME 		: Le nom d'hôte, alias DNS ou adresse IP du serveur.
 @param GATEWAY_INTERFACE 	: La révision de la spécification CGI que le serveur utilise. (Format : CGI/révision)

//! Variables spécifiques à la requête
 @param SERVER_PROTOCOL 	: Le nom et la révision du protocole dans lequel la requête a été faite (Format : protocole/révision)
 @param SERVER_PORT 		: Le numéro de port sur lequel la requête a été envoyée.
 @param REQUEST_METHOD		: La méthode utilisée pour faire la requête. Pour HTTP, elle contient généralement « GET » ou « POST ».
 @param PATH_INFO 			: Le chemin supplémentaire du script tel que donné par le client. Par exemple, si le serveur héberge le script « /cgi-bin/monscript.cgi » et que le client demande l'url « http://serveur.org/cgi-bin/monscript.cgi/marecherche », alors PATH_INFO contiendra « marecherche ».
 @param PATH_TRANSLATED		: Contient le chemin demandé par le client après que les conversions virtuel → physique ont été faites par le serveur.
 @param SCRIPT_NAME 		: Le chemin virtuel vers le script étant exécuté. Exemple : « /cgi-bin/script.cgi »
 @param QUERY_STRING 		: Contient tout ce qui suit le « ? » dans l'URL envoyée par le client. Toutes les variables provenant d'un formulaire envoyé avec la méthode « GET » seront contenues dans le QUERY_STRING sous la forme « var1=val1&var2=val2&... ».
 @param REMOTE_HOST 		: Le nom d'hôte du client. Si le serveur ne possède pas cette information (par exemple, lorsque la résolution DNS inverse est désactivée), REMOTE_HOST sera vide.
 @param REMOTE_ADDR 		: L'adresse IP du client.
 @param AUTH_TYPE 			: Le type d'identification utilisé pour protéger le script (s’il est protégé et si le serveur supporte l'identification).
 @param REMOTE_USER 		: Le nom d'utilisateur du client, si le script est protégé et si le serveur supporte l'identification.
 @param REMOTE_IDENT 		: Nom d'utilisateur (distant) du client faisant la requête. Le serveur doit supporter l'identification RFC 931. Cette variable devrait être utilisée à des fins de journaux seulement.
 @param CONTENT_TYPE 		: Le type de contenu attaché à la requête, si des données sont attachées (comme lorsqu'un formulaire est envoyé avec la méthode « POST »).
 @param CONTENT_LENGTH 		: La longueur du contenu envoyé par le client.  

//! Variables provenant du client 
Toutes les variables qui sont envoyées par le client sont aussi passées au script CGI, après que le serveur a rajouté le préfixe « HTTP_ ». 
Voici quelques exemples de variables possibles :

 @param HTTP_ACCEPT 			: Les types de données MIME que le client accepte de recevoir.
 @param HTTP_ACCEPT_LANGUAGE 	: Les langues dans lequel le client accepte de recevoir la réponse.
 @param HTTP_USER_AGENT 		: Le navigateur utilisé par le client.
 @param HTTP_COOKIE 			: Les éventuels cookies. Une liste de paires clef=valeur contenant les cookies positionnés par le site, séparés par des points-virgules.
 @param HTTP_REFERER 			: Une adresse absolue ou partielle de la page web à partir de laquelle la requête vers la page courante a été émise.
 // * explication de la variable REDIRECT_STATUS : https://stackoverflow.com/questions/24378472/what-is-php-serverredirect-status/31870773
 @param REDIRECT_STATUS			: Permet au cgi php d'accepter les requêtes qui lui sont directement adressées
 */

#endif /* CGI_HPP */
