/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 17:34:45 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/26 15:10:57 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLICATIONINTERFACE_HPP
# define APPLICATIONINTERFACE_HPP

# include "socketManager.hpp"
# include "http.hpp"
# include "color.hpp"

// # define BUFFER_SIZE 4194304 // 4MB
// # define BUFFER_SIZE 2097154 // 2MB
# define BUFFER_SIZE 1048576 // 1MB

class Interface {
	public:
		static void						addExecuter(uint32_t port, http *executer);
		static void						interface(int sock, struct sockData data);
		static void						clearExecuters();
		static void						removeExecuter(uint32_t port);

	private:
		static int						readFromSocket(int sock, struct sockData data, std::string &request);
		static bool						passRequest(std::string &request, std::string &response, uint32_t port);
		static int						writeToSocket(int sock, struct sockData data, std::string &response);

		static std::map<uint32_t, http*>				_executerMap;
		static std::map<int, std::string>				_outputBuffer;
		static std::vector<http*>						_deletedExecuters;
};

#endif