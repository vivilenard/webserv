/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlenard <vlenard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:22:20 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/25 11:57:24 by vlenard          ###   ########.fr       */
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
		Response	httpResponse(httpRequest, _config);
		std::string response = httpResponse.getResponse();
		cout << MAG << "<<-----response-------" << NORM << endl;
		return response;
}
