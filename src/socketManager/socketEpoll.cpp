/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketEpoll.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 20:44:00 by pharbst           #+#    #+#             */
/*   Updated: 2024/01/22 15:23:40 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

#if defined(__LINUX__) || defined(__linux__)
int								socketManager::_epollfd = -1;
void							socketManager::socketEpoll(InterfaceFunction interfaceFunction) {
	const int MAX_EVENTS = 10;
	struct epoll_event ready[MAX_EVENTS];
	if (initEpoll())
		return ;
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
		if (epollAdd(newClient, fd))
			return ;
	}
}

bool							socketManager::epollAdd(int newClient, int serverSocket) {
	struct epoll_event newClientEvent;
	newClientEvent.events = EPOLLIN | EPOLLOUT;
	newClientEvent.data.fd = newClient;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, newClient, &newClientEvent) == -1) {
		std::cerr << "Error adding file descriptor to epoll" << std::endl;
		return true;
	}
	t_data data = _sockets[serverSocket];
	data.server = false;
	_sockets.insert(std::pair<int, t_data>(newClient, data));
	return (false);
}

void							socketManager::epollRemove(int fd) {
	if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		std::cerr << "Error removing file descriptor from epoll" << std::endl;
		return ;
	}
}

bool	socketManager::initEpoll() {
	_epollfd = epoll_create1(0);
	if (_epollfd == -1) {
		std::cerr << "Error creating epoll file descriptor" << std::endl;
		return (true);
	}
	struct epoll_event interest;
	for (std::map<int, t_data>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		interest.events = EPOLLIN | EPOLLOUT;
		interest.data.fd = pair->first;
		if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, pair->first, &interest) == -1) {
			std::cerr << "Error adding file descriptor to epoll" << std::endl;
			return (true);
		}
	}
	return (false);
}

#endif
