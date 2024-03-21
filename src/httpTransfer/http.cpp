/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:22:20 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/21 12:24:42 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

http::http(configServer &config) : _config(config) {}

http::~http() {}

http::http() {}

std::string	http::executer(const std::string &request) {
		cout << MAG << "-------request----->>" << NORM << endl;
		Request		httpRequest(request, _config);
		if (!httpRequest.boundary.empty())
			return "";
		Response	httpResponse(httpRequest, _config);
		std::string response = httpResponse.getResponse();
		cout << MAG << "<<------------------------" << NORM << endl;
		return response;
}
