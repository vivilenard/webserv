/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketSEPOLL.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 07:57:28 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 11:07:39 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

#if defined(__LINUX__) || defined(__linux__)

int								socketManager::_epollfd = -1;

void							socketManager::socketEpoll(InterfaceFunction interfaceFunction) {
	const int MAX_EVENTS = 10;
	struct epoll_event ready[MAX_EVENTS];
	initEpoll();
	int	errorCounter = 0;
	while (true) {
		int numEvents = epoll_wait(_epollfd, ready, MAX_EVENTS, 1000);
		if (numEvents == -1) {
			std::cout << "socketManager::socketEpoll:	Error in epoll_wait" << std::endl;
			errorCounter++;
			if (errorCounter > 0XFFFF)
				throw std::runtime_error("socketManager::socketEpoll:	Critical error: too many errors in epoll_wait");
		}
		for (int i = 0; i < numEvents; ++i) {
			int fd = ready[i].data.fd;
			if (SERVERSOCKET) {
				try {
					epollAccept(fd);
					printMap();
				}
				catch (std::exception &e) {
					PRINT_ERROR;
				}
			}
			else {
				struct sockData data = _sockets[fd];
				if (READEVENT)
					data.read = true;
				else
					data.read = false;
				if (WRITEEVENT)
					data.write = true;
				else
					data.write = false;
				interfaceFunction(fd, data);
			}
		}
		checkTimeouts();
	}
	close(_epollfd);
	_epollfd = -1;
}

void							socketManager::epollAccept(int fd) {
	int newClient = accept(fd, NULL, NULL);
	if (newClient == -1)
		throw std::runtime_error("socketManager::epollAccept:	Error accepting new client");
	epollAdd(newClient, fd);
}

void							socketManager::epollAdd(int newClient, int serverSocket) {
	struct epoll_event newClientEvent;
	newClientEvent.events = EPOLLIN | EPOLLOUT;
	newClientEvent.data.fd = newClient;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, newClient, &newClientEvent) == -1) {
		close(newClient);
		throw std::runtime_error("socketManager::epollAdd:	Error adding file descriptor to epoll");
	}
	struct sockData data = _sockets[serverSocket];
	data.parentSocket = _sockets.find(serverSocket);
	data.lastActivity = getCurrentTime();
	_sockets.insert(std::pair<int, struct sockData>(newClient, data));
}

void							socketManager::epollRemove(int fd) {
	if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
		std::cout << "socketManager::epollRemove:	Error removing file descriptor from epoll" << std::endl;
}

void							socketManager::initEpoll() {
	_epollfd = epoll_create1(0);
	if (_epollfd == -1)
		throw std::runtime_error("socketManager::initEpoll:	Critical error: epoll_create1() failed");
	struct epoll_event interest;
	for (std::map<int, struct sockData>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		interest.events = EPOLLIN | EPOLLOUT;
		interest.data.fd = pair->first;
		for (int i = 0; i < 10; i++) {
			if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, pair->first, &interest) != -1)
				break ;
			if (i == 9) {
				std::cout << "socketManager::initEpoll:	unable to add " << pair->first << " to the epoll instance deleting socket and continue" << std::endl;
				_sockets.erase(pair->first);
			}
		}
	}
}

#elif defined(__APPLE__)

int								socketManager::_kq = -1;
struct kevent					socketManager::_changes[2];
struct kevent					socketManager::_events[2];

void							socketManager::socketKqueue(InterfaceFunction interfaceFunction) {
	initKqueue();
	int errorCounter = 0;
	while (true) {
		int numEvents = kevent(_kq, NULL, 0, _events, 2, NULL);
		if (numEvents == -1) {
			std::cout << "socketManager::socketKqueue:	Error in kevent" << std::endl;
			errorCounter++;
			if (errorCounter > 0XFFFF)
				throw std::runtime_error("socketManager::socketKqueue:	Critical error: too many errors in kevent");
		}
		for (int i = 0; i < numEvents; i++) {
			int fd = _events[i].ident;
			if (SERVERSOCKET) {
				try {
					kqueueAccept(fd);
					printMap();
				}
				catch (std::exception &e) {
					PRINT_ERROR;
				}
			}
			else {
				struct sockData data = _sockets[fd];
				if (READEVENT)
					data.read = true;
				else
					data.read = false;
				if (WRITEEVENT)
					data.write = true;
				else
					data.write = false;
				interfaceFunction(fd, data);
			}
		}
		checkTimeouts();
	}
	close(_kq);
	_kq = -1;
}

void						socketManager::kqueueAccept(int fd) {
	int newClient = accept(fd, NULL, NULL);
	if (newClient == -1)
		throw std::runtime_error("socketManager::kqueueAccept:	Error accepting new client");
	kqueueAdd(newClient, fd);
}

void						socketManager::kqueueAdd(int newClient, int serverSocket) {
	EV_SET(&_changes[0], newClient, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
		close(newClient);
		throw std::runtime_error("socketManager::kqueueAdd:	Error adding read event for file descriptor to kqueue");
	}
	EV_SET(&_changes[0], newClient, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1) {
		close(newClient);
		throw std::runtime_error("socketManager::kqueueAdd:	Error adding write event for file descriptor to kqueue");
	}
	struct sockData data = _sockets[serverSocket];
	data.parentSocket = _sockets.find(serverSocket);
	if (_keepAlive)
		data.lastActivity = getCurrentTime();
	_sockets.insert(std::pair<int, struct sockData>(newClient, data));
}

void						socketManager::kqueueRemove(int fd) {
	EV_SET(&_changes[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) == -1)
		std::cout << "socketManager::kqueueRemove:	Error removing file descriptor from kqueue" << std::endl;
}

void		socketManager::initKqueue() {
	_kq = kqueue();
	if (_kq == -1)
		throw std::runtime_error("socketManager::initKqueue:	Critical error: kqueue() failed");
	for (std::map<int, struct sockData>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		EV_SET(&_changes[0], pair->first, EVFILT_READ, EV_ADD, 0, 0, NULL);
		for (int i = 0; i < 10; i++) {
			if (kevent(_kq, &_changes[0], 1, NULL, 0, NULL) != -1)
				break ;
			if (i == 9) {
				std::cout << "socketManager::initKqueue:	unable to add " << pair->first << " to the kqueue instance deleting socket and continue" << std::endl;
				_sockets.erase(pair->first);
				return ;
			}
		}
		EV_SET(&_changes[1], pair->first, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		for (int i = 0; i < 10; i++) {
			if (kevent(_kq, &_changes[1], 1, NULL, 0, NULL) == -1) {
				std::cout << "socketManager::initKqueue:	unable to add " << pair->first << " to the kqueue instance deleting socket and continue" << std::endl;
				_sockets.erase(pair->first);
				return ;
			}
		}
	}
}

#else

fd_set							socketManager::_interest;
int								socketManager::_maxfd = -1;

void							socketManager::socketSelect(InterfaceFunction interfaceFunction) {
	initSelect();
	int errorCounter = 0;
	while (true) {
		fd_set readList = _interest;
		fd_set writeList = _interest;
		int numEvents = select(_maxfd + 1, &readList, &writeList, NULL, NULL);
		if (numEvents == -1) {
			std::cout << "socketManager::socketSelect:	Error in select" << std::endl;
			errorCounter++;
			if (errorCounter > 0XFFFF)
				throw std::runtime_error("socketManager::socketSelect:	Critical error: too many errors in select");
		}
		for (std::map<int, struct sockData>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
			int fd = it->first;
			struct sockData data = it->second;
			if (FD_ISSET(fd, &readList)) {
				if (data.parentSocket == _sockets.end()) {
					try {
						selectAccept(fd);
						printMap();
						continue ;
					}
					catch (std::exception &e) {
						PRINT_ERROR;
						continue ;
					}
				}
				data.read = true;
			}
			else if (FD_ISSET(fd, &writeList)) {
				if (data.parentSocket == _sockets.end())
					continue;
				data.write = true;
			}
			interfaceFunction(fd, data);
		}
		checkTimeouts();
	}
}

void							socketManager::selectAccept(int fd) {
	int newClient = accept(fd, NULL, NULL);
	if (newClient == -1)
		throw std::runtime_error("socketManager::selectAccept:	Error accepting new client");
	selectAdd(newClient, fd);
}

void							socketManager::selectAdd(int newClient, int serverSocket) {
	FD_SET(newClient, &_interest);
	if (newClient > _maxfd)
		_maxfd = newClient;
	struct sockData data = _sockets[serverSocket];
	data.parentSocket = _sockets.find(serverSocket);
	_sockets.insert(std::pair<int, struct sockData>(newClient, data));
}

void							socketManager::selectRemove(int fd) {
	FD_CLR(fd, &_interest);
	if (fd == _maxfd) {
		for (std::map<int, struct sockData>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
			if (it->first > _maxfd)
				_maxfd = it->first;
		}
	}
}

void							socketManager::initSelect() {
	FD_ZERO(&_interest);
	for (std::map<int, struct sockData>::iterator pair = _sockets.begin(); pair != _sockets.end(); pair++) {
		int fd = pair->first;
		FD_SET(fd, &_interest);
		if (fd > _maxfd)
			_maxfd = fd;
	}
}

#endif
