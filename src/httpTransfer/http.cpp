/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlenard <vlenard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:22:20 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/23 13:28:22 by vlenard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

http::http(configServer &config) : _config(config) {}

http::~http() {}

http::http() {}

std::string	http::executer(const std::string &request) {
		cout << MAG << "-------request----->>" << NORM << endl;
		Request		httpRequest(request, _config);
		if (!httpRequest.boundary.empty() || httpRequest.getRequest().empty())
			return "";
		cout << RED << request << NORM << endl;
		Response	httpResponse(httpRequest, _config);
		std::string response = httpResponse.getResponse();
		cout << "RESPONSE: " << endl;
		cout << BLUE << response << NORM << endl;
		cout << MAG << "<<-----response-------" << NORM << endl;
		return response;
}
