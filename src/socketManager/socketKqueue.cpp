/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketKqueue.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:02:01 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 06:14:56 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

#if defined(__APPLE__)
int								socketManager::_kq = -1;
struct kevent					socketManager::_changes[2];
struct kevent					socketManager::_events[2];

void							socketManager::socketKqueue(InterfaceFunction interfaceFunction) {
	if (initKqueue())
		return ;
	while (true) {
		int numEvents = kevent(_kq, NULL, 0, _events, 2, NULL);
		if (numEvents == -1) {
			std::cerr << "Error in kevent" << std::endl;
			return ;
		}
		for (int i = 0; i < numEvents; i++) {
			int fd = _events[i].ident;
			if (_sockets[fd].server)
				kqueueAccept(fd);
			else {
				sockData data = _sockets[fd];
				if (_events[i].filter == EVFILT_READ)
					data.read = true;
				else
					data.read = false;
				if (_events[i].filter == EVFILT_WRITE)
					data.write = true;
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
		if (newClient == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			std::cout << "Error accepting connection" << std::endl;
			continue;
		}
		if (kqueueAdd(newClient, fd))
			continue;
	}
}

bool						socketManager::kqueueAdd(int newClient, int serverSocket) {
	EV_SET(&_changes[0], newClient, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
		std::cerr << "Error adding file descriptor to kqueue" << std::endl;
		return (true);
	}
	EV_SET(&_changes[0], newClient, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
		std::cerr << "Error adding file descriptor to kqueue" << std::endl;
		return (true);
	}
	struct sockData data = _sockets[serverSocket];
	data.parentSocket = _sockets.find(serverSocket);
	_sockets.insert(std::pair<int, struct sockData>(newClient, data));
	std::cout << "New client connected with:" << std::endl;
	std::cout << "\tfd: " << newClient << std::endl;
	return (false);
}

void						socketManager::kqueueRemove(int fd) {
	EV_SET(&_changes[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
		std::cerr << "Error removing file descriptor from kqueue" << std::endl;
		return ;
	}
}

bool		socketManager::initKqueue() {
	_kq = kqueue();
	if (_kq == -1) {
		std::cerr << "Error creating kqueue" << std::endl;
		return (true);
	}
	for (std::map<int, struct sockData>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		EV_SET(&_changes[0], pair->first, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
			std::cerr << "Error adding file descriptor to kqueue" << std::endl;
			return (true);
		}
		EV_SET(&_changes[1], pair->first, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		if (kevent(_kq, &_changes[1], 1, NULL, 0, NULL) == -1) {
			std::cerr << "Error adding write event to kqueue" << std::endl;
			return (true);
		}
	}
	return (false);
}
#endif