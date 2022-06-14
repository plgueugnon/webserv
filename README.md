# webserv : 42 Cursus project [![pgueugno's 42 webserv Score](https://badge42.vercel.app/api/v2/cl4cqx7ks006409jn1s8sg534/project/2429207)](https://github.com/JaeSeoKim/badge42)
<p align="left"> <a href="https://www.w3schools.com/cpp/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/> </a> <a href="https://www.php.net" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/php/php-original.svg" alt="php" width="40" height="40"/> </a> </p>

### Overview
- Write a http server in c++98
- Base file configuration should follow nginx model
- Server should work as per RFC standard for HTTP/1.1
- A request to the server should never hang forever
- Server must be compatible with the web browser of our choice (mozilla and chrome)
- HTTP response status codes must be accurate
- Server must have default error pages if none are provided
- Server must be able to serve a fully static website
- Clients must be able to upload files (binary files are not managed)
- GET, POST, and DELETE methods are required
- Server must stay available at all cost (stress test using siege)
- Implement multiplexed socket management using either select, poll, epoll or kqueue (we used kqueue)

### Warning
Has we used kqueue to implement our server which is less portable than select, program might not compile depending on the OS you are using (project was built and tested on MacOS).

### Credit
@[pgueugno](pgueugno@student.42.fr) @[ygeslin](ygeslin@student.42.fr)
