/**
 Liste des variables CGI

Variables en rapport avec le serveur
 @param SERVER_SOFTWARE 	: Le nom et la version du serveur HTTP répondant à la requête. (Format : nom/version)
 @param SERVER_NAME 		: Le nom d'hôte, alias DNS ou adresse IP du serveur.
 @param GATEWAY_INTERFACE : La révision de la spécification CGI que le serveur utilise. (Format : CGI/révision)

Variables spécifiques à la requête
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

Variables provenant du client 
Toutes les variables qui sont envoyées par le client sont aussi passées au script CGI, après que le serveur a rajouté le préfixe « HTTP_ ». 
Voici quelques exemples de variables possibles :

 @param HTTP_ACCEPT 			: Les types de données MIME que le client accepte de recevoir.
 @param HTTP_ACCEPT_LANGUAGE 	: Les langues dans lequel le client accepte de recevoir la réponse.
 @param HTTP_USER_AGENT 		: Le navigateur utilisé par le client.
 @param HTTP_COOKIE 			: Les éventuels cookies. Une liste de paires clef=valeur contenant les cookies positionnés par le site, séparés par des points-virgules.
 @param HTTP_REFERER 			: Une adresse absolue ou partielle de la page web à partir de laquelle la requête vers la page courante a été émise.

 */

// http://www.cgi101.com/book/ch3/env.cgi?test1
// CONTEXT_DOCUMENT_ROOT = /home/cgi101/public_html
// CONTEXT_PREFIX =
// DOCUMENT_ROOT = /home/cgi101/public_html
// GATEWAY_INTERFACE = CGI/1.1
// HTTP_ACCEPT = text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
// HTTP_ACCEPT_ENCODING = gzip, deflate
// HTTP_ACCEPT_LANGUAGE = en,fr-FR;q=0.9,fr;q=0.8,en-FR;q=0.7,en-US;q=0.6
// HTTP_CACHE_CONTROL = max-age=0
// HTTP_CONNECTION = keep-alive
// HTTP_HOST = www.cgi101.com
// HTTP_REFERER = http://www.cgi101.com/book/ch3/text.html
// HTTP_UPGRADE_INSECURE_REQUESTS = 1
// HTTP_USER_AGENT = Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36
// PATH = /usr/local/bin:/usr/bin:/bin
// QUERY_STRING = test1
// REMOTE_ADDR = 62.210.32.97
// REMOTE_PORT = 57452
// REQUEST_METHOD = GET
// REQUEST_SCHEME = http
// REQUEST_URI = /book/ch3/env.cgi?test1
// SCRIPT_FILENAME = /home/cgi101/public_html/book/ch3/env.cgi
// SCRIPT_NAME = /book/ch3/env.cgi
// SERVER_ADDR = 45.79.7.121
// SERVER_ADMIN = kira@lightsphere.com
// SERVER_NAME = www.cgi101.com
// SERVER_PORT = 80
// SERVER_PROTOCOL = HTTP/1.1
// SERVER_SIGNATURE =
// Apache/2.4.25 (Debian) Server at www.cgi101.com Port 80

// SERVER_SOFTWARE = Apache/2.4.25 (Debian)
// UNIQUE_ID = Yelk9X8AAQEAACKjB2cAAAAD