/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:05:15 by pharbst           #+#    #+#             */
/*   Updated: 2024/01/17 14:46:03 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEW_SOCKETMANAGER_HPP
# define NEW_SOCKETMANAGER_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <map>
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <cerrno>
# include <csignal>
# include <sstream>

#if defined(__LINUX__) || defined(__linux__)
# include <sys/epoll.h>
# define SEPOLL socketManager::socketEpoll
# define SEPOLLREMOVE socketManager::epollRemove
#elif defined(__APPLE__)
# include <sys/event.h>
# define SEPOLL socketManager::socketKqueue
# define SEPOLLREMOVE socketManager::kqueueRemove
#else
# include <sys/select.h>
# define SEPOLL socketManager::socketSelect
# define SEPOLLREMOVE socketManager::selectRemove
#endif



# define TCP		SOCK_STREAM
# define UDP		SOCK_DGRAM
# define LOCALHOST	AF_LOCAL
# define IPV4		AF_INET
# define IPV6		AF_INET6
# define IP			0

typedef struct s_data {
	uint32_t			port;
	uint32_t			protocol;
	const std::string	interfaceAddress;
	bool				server;
}	t_data;

typedef void	(*InterfaceFunction)(int sock, t_data sockData);

class socketManager {
	public:
		static void							addSocket(const std::string &interfaceAddress, uint32_t port, uint32_t ipVersion, uint32_t protocol);
		static void							removeSocket(int fd);
		static void							printMap();
		static void							start(InterfaceFunction interfaceFunction);
	private:
		static std::map<int, t_data>		_sockets;

	#if defined(__LINUX__) || defined(__linux__)
		static int							_epollfd;
	#elif defined(__APPLE__)
		static int							_kq;
		static struct kevent				_changes[2];
		static struct kevent				_events[2];
	#else
		static fd_set						_interest;
		static int							_maxfd;
	#endif

		static bool							bindSocket(int fd, const std::string &interfaceAddress, uint32_t port, uint32_t ipVersion);
		static bool							validateCreationParams(const std::string &interfaceAddress, uint32_t port, uint32_t protocol);
	#if defined(__LINUX__) || defined(__linux__)
		static void							socketEpoll(InterfaceFunction interfaceFunction);
		static void							epollRemove(int fd);
		static void							epollAccept(int fd);
	#elif defined(__APPLE__)
		static void							socketKqueue(InterfaceFunction interfaceFunction);
		static void							kqueueRemove(int fd);
		static void							kqueueAccept(int fd);
	#else
		static void							socketSelect(InterfaceFunction interfaceFunction);
		static void							selectRemove(int fd);
		static void							selectAccept(int fd);
	#endif
};

#endif