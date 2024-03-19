/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlenard <vlenard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 17:34:45 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/20 17:42:46 by vlenard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLICATIONINTERFACE_HPP
# define APPLICATIONINTERFACE_HPP

# include "socketManager.hpp"

# define BUFFER_SIZE 1024
# define PRINT false

typedef std::string (*protocolFunction)(const std::string &request);

class Interface {
	public:
		static void						addProtocol(uint32_t port, protocolFunction function);
		static void						interface(int sock, sockData sockData);

	private:
		static bool						readFromSocket(int sock, std::string &request);
		static bool						passRequest(std::string &request, std::string &response, uint32_t port);
		static bool						writeToSocket(int sock, std::string &response);

		static std::map<uint32_t, protocolFunction>		_protocolMap;
		static std::map<int, std::string>				_outputBuffer;
};

#endif

