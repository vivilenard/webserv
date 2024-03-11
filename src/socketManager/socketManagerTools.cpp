/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManagerTools.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peter <peter@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:12:07 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 11:36:33 by peter            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

void				socketManager::bindSocket(int fd, struct sockaddr* address) {
	if (bind(fd, address, sizeof(*address)) == -1)
		throw std::runtime_error("socketManager::bindSocket:	binding failed");
}

uint32_t			socketManager::extractPort(struct sockaddr* address) {
	if (address->sa_family == AF_INET)
		return (ntohs(((struct sockaddr_in*)address)->sin_port));
	else if (address->sa_family == AF_INET6)
		return (ntohs(((struct sockaddr_in6*)address)->sin6_port));
	else
		throw std::runtime_error("socketManager::extractPort:	unknown address family");
}

void				socketManager::setSocketNonBlocking(int fd) {
	if (fd < 0)
		throw std::runtime_error("socketManager::setSocketNonBlocking:	Invalid file descriptor: likely due to socket function failure");
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("socketManager::setSocketNonBlocking:	Socketopt getting failed");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("socketManager::setSocketNonBlocking:	Socketopt setting failed");
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("socketManager::setSocketNonBlocking:	Socketopt setting failed");
}

void				socketManager::detectActivity(int fd) {
	if (_keepAlive == 0)
		return ;
	SOCKET.lastActivity = getCurrentTime();
}

unsigned long		socketManager::getCurrentTime() {
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);
    return static_cast<unsigned long>(currentTime.tv_sec) * 1000 + currentTime.tv_nsec / 1000000;
}

void				socketManager::checkTimeouts() {
	static unsigned long lastCheck = 0;
	if (_keepAlive != 0 && getCurrentTime() - lastCheck > _keepAlive / 4) {
		unsigned long currentTime = getCurrentTime();
		for (std::map<int, struct sockData>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
			if (it->second.parentSocket != _sockets.end() && currentTime - it->second.lastActivity > _keepAlive) {
				int fd = it->first;
				it--;
				std::cout << "socketManager::checkTimeouts:	timeout (socket " << fd << ")" << std::endl;
				removeSocket(fd);
			}
		}
		lastCheck = getCurrentTime();
	}
	static unsigned long lastPrint = 0;
	if (getCurrentTime() - lastPrint > 10000) {
		printMap();
		lastPrint = getCurrentTime();
	}
}

void				socketManager::printMap() {
	unsigned long currentTime = getCurrentTime();
	std::cout << "╔════════════Socket Map════════════╗" << std::endl;
	std::cout << "║ fd ║ port ║  server  ║  Timeout  ║" << std::endl;
	for (std::map<int, struct sockData>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
		printf("║ %2d ║ %4d ║ %8d ║ %9ld ║\n", it->first, it->second.port, (it->second.parentSocket == _sockets.end()) ? 1 : 0, (it->second.parentSocket == _sockets.end()) ? 0 : currentTime - it->second.lastActivity);
	}
	std::cout << "╚══════════════════════════════════╝" << std::endl;
}
