/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManagerBase.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 08:04:02 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 08:22:01 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETMANAGERBASE_HPP
#define SOCKETMANAGERBASE_HPP

# include <iostream>
# include <cstdio>
# include <sys/socket.h>
# include <netinet/in.h>
# include <map>
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <cerrno>
# include <csignal>
# include <sstream>
# include <sys/select.h>
# include <sys/types.h>
# include <netdb.h>

#if defined(__LINUX__) || defined(__linux__)
# include <sys/epoll.h>
# define SEPOLL socketManager::socketEpoll
# define SEPOLLREMOVE socketManager::epollRemove
# define READEVENT ready[i].events & EPOLLIN
# define WRITEEVENT ready[i].events & EPOLLOUT
#elif defined(__APPLE__)
# include <sys/event.h>
# define SEPOLL socketManager::socketKqueue
# define SEPOLLREMOVE socketManager::kqueueRemove
# define READEVENT _events[i].filter == EVFILT_READ
# define WRITEEVENT _events[i].filter == EVFILT_WRITE
#else
# define SEPOLL socketManager::socketSelect
# define SEPOLLREMOVE socketManager::selectRemove
#endif



# define TCP		SOCK_STREAM	// 1
# define UDP		SOCK_DGRAM	// 2
# define LOCALHOST	AF_LOCAL
# define IPV4		AF_INET
# define IPV6		AF_INET6
# define IP			0

# define SOCKET			_sockets[fd]
# define SERVERSOCKET	SOCKET.parentSocket == _sockets.end()

# define PRINT_ERROR	std::cout << e.what() << std::endl

struct sockData {
	uint32_t									port;
	bool										read;
	bool										write;
	std::map<int, sockData>::iterator			parentSocket;
	unsigned long								lastActivity;
};

struct sockParameter {
	struct sockaddr*	interfaceAddress;
	int					protocol;
};

typedef void	(*InterfaceFunction)(int sock, sockData Data);

#endif