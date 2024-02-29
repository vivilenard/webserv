/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceTools.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:02:48 by pharbst           #+#    #+#             */
/*   Updated: 2024/02/29 20:39:58 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Interface.hpp"

bool	Interface::readFromSocket(int sock, std::string &request) {
	int	n = BUFFER_SIZE;
	char buffer[BUFFER_SIZE];
	while (n == BUFFER_SIZE) {
		n = recv(sock, buffer, BUFFER_SIZE, 0);
		if (n < 0) {
			std::cout << "recv failed" << std::endl;
			return (true);
		}
		if (n == 0 && request.empty()) {
			std::cout << "client disconnected" << std::endl;
			return (true);
		}
		request.append(buffer, n);
	}
	return (false);
}

bool	Interface::passRequest(std::string &request, std::string &response, uint32_t port) {
	if (request.empty())
		return (true);
	if (_protocolMap.find(port) != _protocolMap.end()) {
		response = _protocolMap[port](request);
		return (false);
	}
	std::cout << "unknown protocol" << std::endl;
	return (false);
}

bool	Interface::writeToSocket(int sock, std::string &response) {
	int i = send(sock, response.c_str(), response.length(), 0);
	if (i < 0 || i == 0)
		return (true);
	return (false);
}
