/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:22:20 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/20 20:07:47 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

http::http(configServer &config) : _config(config) {}

http::~http() {}

http::http() {}

static uint32_t	extractPort(struct sockaddr* address) 
{
	if (address->sa_family == AF_INET)
		return (ntohs(((struct sockaddr_in*)address)->sin_port));
	else if (address->sa_family == AF_INET6)
		return (ntohs(((struct sockaddr_in6*)address)->sin6_port));
	else
		throw std::runtime_error("extractPort: unknown address family");
}

std::string	http::executer(const std::string &request) {
		std::cout << "executer for port: " << extractPort(_config._socketAddress.interfaceAddress) << std::endl;
		cout << MAG << "-------request----->>" << NORM << endl;
		Request		httpRequest(request, _config);
		if (!httpRequest.boundary.empty())
			return "";
		Response	httpResponse(httpRequest, _config);
		std::string response = httpResponse.getResponse();
		cout << MAG << "<<------------------------" << NORM << endl;
		return response;
}
