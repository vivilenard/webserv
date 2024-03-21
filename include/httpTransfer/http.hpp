/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:22:29 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/24 00:32:00 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include "configFile.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Status.hpp"

class http {
	public:
		http(configServer &config);
		~http();
		std::string	executer(const std::string &request);
	private:
		configServer	_config;
		http();
};

#endif
