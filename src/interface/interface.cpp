/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:01:41 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/20 20:02:40 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Interface.hpp"

std::map<uint32_t, http*>				Interface::_executerMap;
std::map<int, std::string>				Interface::_outputBuffer;

void	Interface::interface(int sock, struct sockData data) {
	if (data.READ) {
		socketManager::detectActivity(sock);
		std::string		request;
		std::string		response;
		int status = readFromSocket(sock, data, request);
		if (status == -1) {
			std::cout << "readFromSocket failed" << std::endl;
			socketManager::removeSocket(sock);
			return ;
		}
		else if (status == 0)
			return ;
		if (passRequest(request, response, data.info.port)) {
			std::cout << "passRequest failed" << std::endl;
			socketManager::removeSocket(sock);
			return ;
		}
		std::cout << "response added to buffer for socket: " << sock << std::endl;
		_outputBuffer.insert(std::pair<int, std::string>(sock, response));
	}
	else if (data.WRITE && _outputBuffer.find(sock) != _outputBuffer.end()) {
		socketManager::detectActivity(sock);
		std::cout << "writing to socket: " << sock << std::endl;
		std::string	response = _outputBuffer[sock];
		if (writeToSocket(sock, data, response)) {
			std::cout << "writeToSocket failed" << std::endl;
			socketManager::removeSocket(sock);
			_outputBuffer.erase(sock);
			return ;
		}
		_outputBuffer.erase(sock);
	}
}

void	Interface::clearExecuters() {
	for (std::map<uint32_t, http*>::iterator it = _executerMap.begin(); it != _executerMap.end(); it++) {
		delete it->second;
	}
	_executerMap.clear();
}

void	Interface::addExecuter(uint32_t port, http *executer) {
	_executerMap.insert(std::pair<uint32_t, http*>(port, executer));
}
