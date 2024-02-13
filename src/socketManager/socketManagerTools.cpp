/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManagerTools.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlenard <vlenard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:12:07 by pharbst           #+#    #+#             */
/*   Updated: 2024/01/22 15:46:11 by pharbst          ###   ########.fr       */
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

