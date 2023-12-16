/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 17:34:45 by pharbst           #+#    #+#             */
/*   Updated: 2024/01/13 18:17:17 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLICATIONINTERFACE_HPP
# define APPLICATIONINTERFACE_HPP

# include "socketManager.hpp"

# define BUFFER_SIZE 1024

typedef std::string (*protocolFunction)(const std::string &request);

class Interface {
	public:
		static void						addProtocol(std::string protocol, protocolFunction function);
		static void						interface(int sock, t_data sockData);

	private:
		static bool						readFromSocket(int sock, std::string &request);
		static bool						passRequest(std::string &request, std::string &response);
		static bool						writeToSocket(int sock, std::string &response);

		static std::map<std::string, protocolFunction>	_protocolMap;
};

#endif