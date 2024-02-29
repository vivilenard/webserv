/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:05:28 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 01:31:06 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

std::map<int, s_sockData>		socketManager::_sockets;

void	socketManager::start(InterfaceFunction interfaceFunction) {
	std::cout << "SocketManager started" << std::endl;
	SEPOLL(interfaceFunction);
}

void	socketManager::removeSocket(int fd) {
	SEPOLLREMOVE(fd);
	close(fd);
	_sockets.erase(fd);
}

void	socketManager::addSocket(const sockParameter &parameter) {
	int fd = socket(parameter.interfaceAddress->sa_family, parameter.protocol, 0);
	setSocketNonBlocking(fd);
	bindSocket(fd, parameter.interfaceAddress);
	if (listen(fd, SOMAXCONN) == -1) {
		throw std::runtime_error("Socket listening failed");
	}
	s_sockData data; {
		data.parentSocket = _sockets.end();
		data.port = extractPort(parameter.interfaceAddress);
		data.read = false;
		data.write = false;
	}
	_sockets.insert(std::pair<int, s_sockData>(fd, data));
}
