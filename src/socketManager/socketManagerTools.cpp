/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManagerTools.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:12:07 by pharbst           #+#    #+#             */
/*   Updated: 2024/01/20 17:23:39 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"
#include <iostream>
#include <cstdio>

bool	socketManager::validateCreationParams(const std::string &interfaceAddress, uint32_t port, uint32_t protocol) {
	// validate port
	{
		if (port > 0XFFFF) {
			std::cout << "Port is out of range" << std::endl;
			return true;
		}
	}
	// validate protocol
	{
		switch (protocol)
		{
		case TCP:
			break;
		case UDP:
			break;
		default:
			return true;
		}
	}
	// check if socket already exists
	{
		for (std::map<int, t_data>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
			if (it->second.port == port && it->second.protocol == protocol && it->second.interfaceAddress == interfaceAddress) {
				std::cout << "Socket already exists" << std::endl;
				return true;
			}
		}
	}
	return false;
}

bool	socketManager::bindSocket(int fd, const std::string &interfaceAddress, uint32_t port, uint32_t ipVersion) {
		if (ipVersion == IPV4) {
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			if (inet_pton(AF_INET, interfaceAddress.c_str(), &addr.sin_addr) != 1) {
				std::cout << "Invalid interface address" << std::endl;
				return true;
			}
			if (bind(fd, (sockaddr *)&addr, sizeof(addr)) == -1) {
				std::cout << "Socket binding failed" << std::endl;
				return true;
			}
		}
		else if (ipVersion == IPV6) {
			sockaddr_in6 addr;
			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
			if (inet_pton(AF_INET6, interfaceAddress.c_str(), &addr.sin6_addr) != 1) {
				std::cout << "Invalid interface address" << std::endl;
				return true;
			}
			if (bind(fd, (sockaddr *)&addr, sizeof(addr)) == -1) {
				std::cout << "Socket binding failed" << std::endl;
				return true;
			}
		}
		else {
			std::cout << "Invalid ipVersion" << std::endl;
			return true;
		}
	return false;
}

void	socketManager::printMap() {
	std::cout << "╔══════Socket Map══════╗" << std::endl;
	std::cout << "║ fd ║ port ║  server  ║" << std::endl;
	for (std::map<int, t_data>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
		printf("║ %2d ║ %4d ║ %8d ║\n", it->first, it->second.port, it->second.server);
	}
	std::cout << "╚══════════════════════╝" << std::endl;
}

#if defined(__LINUX__) || defined(__linux__)
int								socketManager::_epollfd = -1;
void							socketManager::socketEpoll(InterfaceFunction interfaceFunction) {
	const int MAX_EVENTS = 10;
	std::map<int, uint32_t> eventsMap;

	_epollfd = epoll_create1(0);
	if (_epollfd == -1) {
		std::cerr << "Error creating epoll file descriptor" << std::endl;
		return ;
	}

	struct epoll_event interest, ready[MAX_EVENTS];
	for (std::map<int, t_data>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		interest.events = EPOLLIN | EPOLLOUT;
		interest.data.fd = pair->first;
		if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, pair->first, &interest) == -1) {
			std::cerr << "Error adding file descriptor to epoll" << std::endl;
			return ;
		}
	}

	while (true) {
		// std::cout << "waiting for events" << std::endl;
		int numEvents = epoll_wait(_epollfd, ready, MAX_EVENTS, -1);
		if (numEvents == -1) {
			std::cerr << "Error in epoll_wait" << std::endl;
			return ;
		}
		// std::cout << "event detected" << std::endl;
		for (int i = 0; i < numEvents; ++i) {
			int fd = ready[i].data.fd;
			if (_sockets[fd].server)
				epollAccept(fd);
			else {
				t_data data = _sockets[fd];
				if (ready[i].events & EPOLLIN)
					data.read = true;
				else
					data.read = false;
				if (ready[i].events & EPOLLOUT)
					data.write = true;
				else
					data.write = false;
				interfaceFunction(fd, data);
			}
		}
	}
	close(_epollfd);
	_epollfd = -1;
}
void							socketManager::epollAccept(int fd) {
	while (true) {
		int newClient = accept(fd, NULL, NULL);
		if (newClient == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			std::cout << "Error accepting connection" << std::endl;
			continue;
		}
		struct epoll_event ev;
		ev.events = EPOLLIN | EPOLLOUT;
		ev.data.fd = newClient;
		if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, newClient, &ev) == -1) {
			std::cerr << "Error adding file descriptor to epoll" << std::endl;
			return ;
		}
		t_data data = _sockets[fd];
		data.server = false;
		_sockets.insert(std::pair<int, t_data>(newClient, data));
		std::cout << "New client connected" << std::endl;
		std::cout << "\tfd: " << newClient << std::endl;
	}
}
void							socketManager::epollRemove(int fd) {
	if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		std::cerr << "Error removing file descriptor from epoll" << std::endl;
		return ;
	}
}
#elif defined(__APPLE__)
int								socketManager::_kq = -1;
struct kevent					socketManager::_changes[2];
struct kevent					socketManager::_events[2];

void							socketManager::socketKqueue(InterfaceFunction interfaceFunction) {
	_kq = kqueue();
	if (_kq == -1) {
		std::cerr << "Error creating kqueue" << std::endl;
		return ;
	}
	for (std::map<int, t_data>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		EV_SET(&_changes[0], pair->first, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
			std::cerr << "Error adding file descriptor to kqueue" << std::endl;
			return ;
		}
		EV_SET(&_changes[1], pair->first, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		if (kevent(_kq, &_changes[1], 1, NULL, 0, NULL) == -1) {
			std::cerr << "Error adding write event to kqueue" << std::endl;
			return;
		}
	}
	while (true) {
		// std::cout << "waiting for events" << std::endl;
		printMap();
		int numEvents = kevent(_kq, NULL, 0, _events, 2, NULL);
		if (numEvents == -1) {
			std::cerr << "Error in kevent" << std::endl;
			return ;
		}
		// std::cout << "event detected" << std::endl;
		for (int i = 0; i < numEvents; i++) {
			int fd = _events[i].ident;
			if (_sockets[fd].server)
				kqueueAccept(fd);
			else {
				t_data data = _sockets[fd];
				if (_events[i].filter == EVFILT_READ) {
					data.read = true;
					std::cout << "read event detected" << std::endl;
				}
				else
					data.read = false;
				if (_events[i].filter == EVFILT_WRITE) {
					data.write = true;
					std::cout << "write event detected" << std::endl;
				}
				else
					data.write = false;
				interfaceFunction(fd, data);
			}
		}
	}
	close(_kq);
	_kq = -1;
}

void						socketManager::kqueueAccept(int fd) {
	std::cout << "kqueue accept called" << std::endl;
		int newClient;
	while (true) {
		newClient = accept(fd, NULL, NULL);
		// std::cout << "debug: fd = " << newClient << std::endl;
		if (newClient == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			std::cout << "Error accepting connection" << std::endl;
			continue;
		}
		EV_SET(&_changes[0], newClient, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
			std::cerr << "Error adding file descriptor to kqueue" << std::endl;
			return ;
		}
		EV_SET(&_changes[0], newClient, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
			std::cerr << "Error adding file descriptor to kqueue" << std::endl;
			return ;
		}
		t_data data = _sockets[fd];
		data.server = false;
		_sockets.insert(std::pair<int, t_data>(newClient, data));
		std::cout << "New client connected with:" << std::endl;
		std::cout << "\tfd: " << newClient << std::endl;
	}
}

void						socketManager::kqueueRemove(int fd) {
	EV_SET(&_changes[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
		std::cerr << "Error removing file descriptor from kqueue" << std::endl;
		return ;
	}
}
#else
fd_set							socketManager::_interest;
int								socketManager::_maxfd = -1;

void							socketManager::socketSelect(InterfaceFunction interfaceFunction) {
	FD_ZERO(&_interest);
	int maxfd = 0;
	for (std::map<int, t_data>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		FD_SET(pair->first, &_interest);
		if (pair->first > maxfd)
			maxfd = pair->first;
	}
	while (true) {
		// std::cout << "waiting for events" << std::endl;
		fd_set readList = _interest;
		fd_set writeList = _interest;
		int numEvents = select(maxfd + 1, &readList, &writeList, NULL, NULL);
		if (numEvents == -1) {
			std::cerr << "Error in select" << std::endl;
			return ;
		}
		// std::cout << "event detected" << std::endl;
		for (std::map<int, t_data>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
			t_data data = it->second;
			if (FD_ISSET(it->first, &readList)) {
				if (it->second.server) {
					selectAccept(it->first);
					continue;
				}
				data.read = true;
			}
			if (FD_ISSET(it->first, &writeList)) {
				if (it->second.server)
					continue;
				data.write = true;
			}
			interfaceFunction(it->first, data);
		}
	}
}
void							socketManager::selectAccept(int fd) {
	while (true) {
		int newClient = accept(fd, NULL, NULL);
		if (newClient == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			std::cout << "Error accepting connection" << std::endl;
			continue;
		}
		FD_SET(newClient, &_interest);
		if (newClient > _maxfd)
			_maxfd = newClient;
		t_data data = _sockets[fd];
		data.server = false;
		_sockets.insert(std::pair<int, t_data>(newClient, data));
		std::cout << "New client connected" << std::endl;
		std::cout << "\tfd: " << newClient << std::endl;
	}
}
void							socketManager::selectRemove(int fd) {
	std::cout << "Removing fd " << fd << std::endl;
	FD_CLR(fd, &_interest);
	if (fd == _maxfd) {
		for (std::map<int, t_data>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
			if (it->first > _maxfd)
				_maxfd = it->first;
		}
	}
}
#endif
