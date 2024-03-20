/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:22:29 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/20 20:04:49 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# define RED "\033[1;91m"
# define GREEN "\033[1;92m"
# define ORANGE "\033[1;93m"
# define MAG "\033[1;95m"
# define BLUE "\033[1;96m"
# define NORM "\033[0m"
# define BACK "\033[45m"

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
