/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManagerUtils.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 20:31:39 by pharbst           #+#    #+#             */
/*   Updated: 2024/02/13 15:25:32 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketManager.hpp"
#include <cstdio>

bool	socketManager::setSocketNonBlocking(int fd) {
	if (fd < 0)
		return (true);
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		std::cout << "Socketopt getting failed" << std::endl;
		return (true);
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cout << "Socketopt setting failed" << std::endl;
		return (true);
	}
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cout << "Socketopt setting failed" << std::endl;
		return (true);
	}
	return (false);
}

void	socketManager::printMap() {
	std::cout << "╔══════Socket Map══════╗" << std::endl;
	std::cout << "║ fd ║ port ║  server  ║" << std::endl;
	for (std::map<int, t_data>::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
		printf("║ %2d ║ %4d ║ %8d ║\n", it->first, it->second.port, it->second.server);
	}
	std::cout << "╚══════════════════════╝" << std::endl;
}
