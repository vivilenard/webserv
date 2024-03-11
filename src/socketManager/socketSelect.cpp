/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketSelect.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:11:48 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 06:14:56 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

fd_set							socketManager::_interest;
int								socketManager::_maxfd = -1;

void							socketManager::socketSelect(InterfaceFunction interfaceFunction) {
	if (initSelect())
		return ;
	while (true) {
		fd_set readList = _interest;
		fd_set writeList = _interest;
		int numEvents = select(_maxfd + 1, &readList, &writeList, NULL, NULL);
		if (numEvents == -1) {
			std::cerr << "Error in select" << std::endl;
			return ;
		}
		for (std::map<int, struct sockData>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
			struct sockData data = it->second;
			if (FD_ISSET(it->first, &readList)) {
				if (it->second.parentSocket == _sockets.end()) {
					selectAccept(it->first);
					continue;
				}
				data.read = true;
			}
			if (FD_ISSET(it->first, &writeList)) {
				if (it->second.parentSocket == _sockets.end())
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
		struct sockData data = _sockets[fd];
		data.parentSocket = _sockets.find(fd);
		_sockets.insert(std::pair<int, struct sockData>(newClient, data));
		std::cout << "New client connected" << std::endl;
		std::cout << "\tfd: " << newClient << std::endl;
	}
}
void							socketManager::selectRemove(int fd) {
	std::cout << "Removing fd " << fd << std::endl;
	FD_CLR(fd, &_interest);
	if (fd == _maxfd) {
		for (std::map<int, struct sockData>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
			if (it->first > _maxfd)
				_maxfd = it->first;
		}
	}
}

bool							socketManager::initSelect() {
	FD_ZERO(&_interest);
	for (std::map<int, struct sockData>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		FD_SET(pair->first, &_interest);
		if (pair->first > _maxfd)
			_maxfd = pair->first;
	}
	return (false);
}
