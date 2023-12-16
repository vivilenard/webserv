/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:01:41 by pharbst           #+#    #+#             */
/*   Updated: 2024/01/15 15:52:06 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Interface.hpp"

std::map<std::string, protocolFunction>	Interface::_protocolMap;

void	Interface::interface(int sock, t_data sockData) {
	(void)sockData;
	std::cout << "interface called" << std::endl;
	std::string		request;
	std::string		response;
	if (readFromSocket(sock, request)) {
		std::cout << "readFromSocket failed" << std::endl;
		socketManager::removeSocket(sock);
		return ;	// remove client
	}
	if (passRequest(request, response)) {
		std::cout << "passRequest failed" << std::endl;
		socketManager::removeSocket(sock);
		return ;	// remove client
	}
	if (writeToSocket(sock, response)) {
		std::cout << "writeToSocket failed" << std::endl;
		socketManager::removeSocket(sock);
		return ;	// remove client
	}
}

void	Interface::addProtocol(std::string protocol, protocolFunction function) {
	_protocolMap.insert(std::pair<std::string, protocolFunction>(protocol, function));
}
