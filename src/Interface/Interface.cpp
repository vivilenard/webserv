/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlenard <vlenard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:01:41 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/20 14:13:52 by vlenard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Interface.hpp"

std::map<uint32_t, protocolFunction>	Interface::_protocolMap;
std::map<int, std::string>				Interface::_outputBuffer;

void	Interface::interface(int sock, struct sockData data) {
	if (data.read) {
		socketManager::detectActivity(sock);
		std::string		request;
		std::string		response;
		int status = readFromSocket(sock, request);
		// std::cout << request << std::endl;
		if (status == -1) {
			std::cout << "readFromSocket failed" << std::endl;
			socketManager::removeSocket(sock);
			return ;	// remove client
		}
		else if (status == 0)
			return ;
		if (passRequest(request, response, data.port)) {
			std::cout << "passRequest failed" << std::endl;
			socketManager::removeSocket(sock);
			return ;	// remove client
		}
		if (PRINT)
			{ std::cout << "response added to buffer for socket: " << sock << std::endl; }
		_outputBuffer.insert(std::pair<int, std::string>(sock, response));
	}
	else if (data.write && _outputBuffer.find(sock) != _outputBuffer.end()) {
		socketManager::detectActivity(sock);
		if (PRINT)
			{ std::cout << "writing to socket: " << sock << std::endl; }
		std::string		response = _outputBuffer[sock];
		if (writeToSocket(sock, response)) {
			std::cout << "writeToSocket failed" << std::endl;
			socketManager::removeSocket(sock);
			_outputBuffer.erase(sock);
			return ;	// remove client
		}
		_outputBuffer.erase(sock);
	}
}

void	Interface::addProtocol(uint32_t port, protocolFunction function) {
	_protocolMap.insert(std::pair<uint32_t, protocolFunction>(port, function));
}
