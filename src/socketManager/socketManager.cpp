/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:05:28 by pharbst           #+#    #+#             */
/*   Updated: 2024/02/13 15:45:55 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"

std::map<int, t_data>		socketManager::_sockets;

void	socketManager::start(InterfaceFunction interfaceFunction) {
	std::cout << "SocketManager started" << std::endl;
	SEPOLL(interfaceFunction);
}

void	socketManager::removeSocket(int fd) {
	SEPOLLREMOVE(fd);
	close(fd);
	_sockets.erase(fd);
}

void	socketManager::addSocket(const std::string &interfaceAddress, uint32_t port, uint32_t ipVersion, uint32_t protocol) {
	(void)protocol;
	if (validateCreationParams(interfaceAddress, port, protocol))
		return ;
	// create socket with error check
	int fd = socket(ipVersion, protocol, 0);
	if (fd == -1) {
		std::cout << "Socket creation failed" << std::endl;
		return ;
	}
	// set socketopt to reuse address and nonblocking with error check
	if (setSocketNonBlocking(fd))
		return ;
	// bind socket with error check
	if (bindSocket(fd, interfaceAddress, port, ipVersion))
		return ;
	// listen on socket with error check
	{
		if (listen(fd, 10) == -1) {
			std::cout << "Socket listening failed" << std::endl;
			return ;
		}
	}
	// set flags
	// add socket to map
	t_data data = {port, protocol, interfaceAddress, false, false, true};
	_sockets.insert(std::pair<int, t_data>(fd, data));
}
