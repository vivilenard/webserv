/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManagerTools.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:12:07 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 01:31:19 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

void	socketManager::bindSocket(int fd, struct sockaddr* address) {
	if (bind(fd, address, sizeof(*address)) == -1)
		throw std::runtime_error("binding failed");
}

uint32_t	socketManager::extractPort(struct sockaddr* address) {
	if (address->sa_family == AF_INET)
		return (ntohs(((struct sockaddr_in*)address)->sin_port));
	else if (address->sa_family == AF_INET6)
		return (ntohs(((struct sockaddr_in6*)address)->sin6_port));
	else
		throw std::runtime_error("extractPort: unknown address family");
}

void	socketManager::setSocketNonBlocking(int fd) {
	if (fd < 0)
		throw std::runtime_error("Invalid file descriptor: likely due to socket function failure");
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("Socketopt getting failed");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("Socketopt setting failed");
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("Socketopt setting failed");
}

void	socketManager::printMap() {
	std::cout << "╔══════Socket Map══════╗" << std::endl;
	std::cout << "║ fd ║ port ║  server  ║" << std::endl;
	for (std::map<int, s_sockData>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
		printf("║ %2d ║ %4d ║ %8d ║\n", it->first, it->second.port, (it->second.parentSocket == _sockets.end()) ? 1 : 0);
	}
	std::cout << "╚══════════════════════╝" << std::endl;
}
