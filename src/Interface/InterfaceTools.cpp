/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceTools.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlenard <vlenard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:02:48 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/20 17:44:15 by vlenard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Interface.hpp"

int	Interface::readFromSocket(int sock, std::string &request) {
	int	n = BUFFER_SIZE;
	char buffer[BUFFER_SIZE];
	while (n == BUFFER_SIZE) {
		n = recv(sock, buffer, BUFFER_SIZE, 0);
		if (n < 0) {
			std::cout << "recv failed" << std::endl;
			return n;
		}
		else if (n == 0 && request.empty())
			return n;
		request.append(buffer, n);
	}
	return 1;
}

bool	Interface::passRequest(std::string &request, std::string &response, uint32_t port) {
	if (_protocolMap.find(port) != _protocolMap.end()) {
		response = _protocolMap[port](request);
		return (false);
	}
	std::cout << "unknown protocol" << std::endl;
	return (true);
}

bool	Interface::writeToSocket(int sock, std::string &response) {
	if (response.empty())
	{
		// std::cout << "response is empty" << std::endl;
		return false;
	}
	// std::cout << "just sent: " << std::endl;
	// std::cout << response.c_str() << std::endl;
	_outputBuffer.erase(sock);
	int i = send(sock, response.c_str(), response.length(), 0);
	if (i < 0)
		return (true);
	return (false);
}
