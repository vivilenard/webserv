/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 17:34:45 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/20 19:58:07 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLICATIONINTERFACE_HPP
# define APPLICATIONINTERFACE_HPP

# include "socketManager.hpp"
# include "http.hpp"

# define BUFFER_SIZE 1024

class Interface {
	public:
		static void						addExecuter(uint32_t port, http *executer);
		static void						interface(int sock, struct sockData sockData);
		static void						clearExecuters();

	private:
		static int						readFromSocket(int sock, struct sockData data, std::string &request);
		static bool						passRequest(std::string &request, std::string &response, uint32_t port);
		static bool						writeToSocket(int sock, struct sockData data, std::string &response);

		static std::map<uint32_t, http*>				_executerMap;
		static std::map<int, std::string>				_outputBuffer;
};

#endif