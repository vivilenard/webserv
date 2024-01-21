
//169.245.240.99
// struct sockaddr
// {
//     unsigned short integer sa_family;     /* address family */
//     char sa_data[14];      /*  up to 14 bytes of direct address */
// };
// struct in_addr
// {
//         unsigned long integer s_addr;
// };
// struct sockaddr_in
// {
//         short   sin_family;   //=IPv4 or IPv6 ...
//         unsigned short integer sin_port;
//         struct  in_addr sin_addr;
//         char    sin_zero[8];
// };
//ssize_t read(int fd, void *buf, size_t count);


// const char* forbidden =
//         "HTTP/1.1 403 Forbidden\n"
//         "Content-Type: text/html\n"
//         "Content-Length: 0\n"
//         "Keep - Alive: timeout=1, max=1\n"
//         "Accept-Ranges: bytes\n"
//         "Connection: close\r\n\r\n";

// const char* ok =
//         "HTTP/1.1 200 OK\n"
//         "Content-Type: text/html\n"
//         "Content-Length: 0\n"
//         "Keep - Alive: timeout=1, max=1\n"
//         "Accept-Ranges: bytes\n"
//         "Connection: close\r\n\r\n";


	//send(clientSocket, reply, strlen(reply), 0);
