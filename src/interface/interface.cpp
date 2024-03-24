/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:01:41 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/24 07:38:53 by pharbst          ###   ########.fr       */
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
		std::cout << FYellow << "recieved string from sock: " << sock << std::endl;
		std::cout << "==================================================================================================================================================================================================" << std::endl;
		std::cout << request << std::endl;
		std::cout << "==================================================================================================================================================================================================" << std::endl;
		if (status == -1) {
			socketManager::removeSocket(sock);
			return ;
		}
		else if (status == 0)
			return ;
		if (passRequest(request, response, data.info.port)) {
			std::cout << FRed << "Oh no something went wrong" << NORMAL << std::endl;
			socketManager::removeSocket(sock);
			return ;
		}
		std::cout << std::endl << std::endl << std::endl << std::endl;
		std::cout << NORMAL <<std::endl;
		std::cout << NORMAL <<std::endl;
		std::cout << Yellow << "socket=" << sock << std::endl << std::endl << "response=" << std::endl << response << NORMAL << std::endl;
		_outputBuffer.insert(std::pair<int, std::string>(sock, response));
		std::cout << FCyan << "here some important information" << std::endl;
		std::cout << std::endl << std::endl << std::endl << std::endl;
		for (std::map<int, std::string>::iterator it = _outputBuffer.begin(); it != _outputBuffer.end(); it++) {
			std::cout << FGreen << "output buffer: " << it->first << std::endl;
			std::cout << FCyan << it->second << NORMAL << std::endl;
		}
		std::cout << FCyan << "end of important information" << std::endl;
		std::cout << FGreen << "response added to output buffer for sock: " << sock << NORMAL << std::endl;
	}
	else if (data.WRITE && _outputBuffer.find(sock) != _outputBuffer.end()) {
		socketManager::detectActivity(sock);
		std::cout << FGreen << "writing to socket: " << sock << NORMAL << std::endl;
		std::string	response = _outputBuffer[sock];
		std::cout << "here is the important stuff" << std::endl;
		std::cout << Yellow << "socket=" << sock << std::endl << "response=" << response << NORMAL << std::endl;
		if (writeToSocket(sock, data, response)) {
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

void	Interface::removeExecuter(uint32_t port) {
	if (_executerMap.find(port) == _executerMap.end())
		return ;
	delete _executerMap[port];
	_executerMap.erase(port);
}

void	Interface::addExecuter(uint32_t port, http *executer) {
	_executerMap.insert(std::pair<uint32_t, http*>(port, executer));
}
