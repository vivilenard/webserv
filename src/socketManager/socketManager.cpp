/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:05:28 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 07:46:30 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

std::map<int, struct sockData>		socketManager::_sockets;

void	socketManager::start(InterfaceFunction interfaceFunction) {
	std::cout << "SocketManager:	Starting..." << std::endl;
	SEPOLL(interfaceFunction);
}

void	socketManager::removeSocket(int fd) {
	SEPOLLREMOVE(fd);
	close(fd);
	_sockets.erase(fd);
}

void	socketManager::addSocket(struct sockParameter &parameter) {
	int proto;
	struct sockData data;
	if (!parameter.protocol)
		proto = TCP;
	else if (parameter.protocol != TCP && parameter.protocol != UDP) {
		delete parameter.interfaceAddress;
		throw std::runtime_error("socketManager:	addSocket:	Invalid protocol");
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
		throw std::runtime_error("socketManager:	addSocket:	Socket listening failed");
	}
	data.parentSocket = _sockets.end();
	data.read = false;
	data.write = false;
	_sockets.insert(std::pair<int, struct sockData>(fd, data));
}
