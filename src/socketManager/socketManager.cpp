/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlenard <vlenard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/03/20 17:58:50 by vlenard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "socketManager.hpp"

std::map<int, struct sockData>		socketManager::_sockets;
unsigned long						socketManager::_keepAlive = 0; //change here if needed
unsigned long						socketManager::_keepAlive = 0; //change here if needed

void	socketManager::start(InterfaceFunction interfaceFunction) {
	std::cout << "socketManager::Starting..." << std::endl;
	std::cout << "socketManager::KeepAlive: " << _keepAlive << "ms" << std::endl;
	printMap();
	SEPOLL(interfaceFunction);
}

void	socketManager::removeSocket(int fd) {
	SEPOLLREMOVE(fd);
	_sockets.erase(fd);
	close(fd);
}

void	socketManager::addSocket(struct sockParameter &parameter) {
	int proto;
	struct sockData data;
	if (!parameter.protocol)
		proto = TCP;
	else if (parameter.protocol != TCP && parameter.protocol != UDP) {
		delete parameter.interfaceAddress;
		throw std::runtime_error("socketManager::addSocket:	Invalid protocol");
	}
	else
		proto = parameter.protocol;
	int fd = socket(parameter.interfaceAddress->sa_family, proto, IP);
	try {
		setSocketNonBlocking(fd);
		bindSocket(fd, parameter.interfaceAddress);
		data.port = extractPort(parameter.interfaceAddress);
		delete parameter.interfaceAddress;
	}
	catch (std::exception &e) {
		delete parameter.interfaceAddress;
		throw e;
	}
	if (listen(fd, SOMAXCONN) == -1) {
		throw std::runtime_error("socketManager::addSocket:	Socket listening failed");
	}
	data.parentSocket = _sockets.end();
	data.read = false;
	data.write = false;
	data.lastActivity = 0;
	_sockets.insert(std::pair<int, struct sockData>(fd, data));
}

