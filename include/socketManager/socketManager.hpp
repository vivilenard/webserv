/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 15:05:15 by pharbst           #+#    #+#             */
/*   Updated: 2024/03/11 11:23:01 by pharbst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEW_SOCKETMANAGER_HPP
# define NEW_SOCKETMANAGER_HPP

# include "socketManagerBase.hpp"

class socketManager {
	public:
		static void							addSocket(struct sockParameter &parameter);
		static void							start(InterfaceFunction interfaceFunction);
		static void							removeSocket(int fd);
		static void							printMap();
		static void							detectActivity(int fd);

	private:
	// private attributes
		static std::map<int, sockData>		_sockets;

		static unsigned long				_keepAlive;
	
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

	// private methods
		static void							setSocketNonBlocking(int fd);
		static void							bindSocket(int fd, struct sockaddr* address);
		static uint32_t						extractPort(struct sockaddr* address);
		static void							checkTimeouts();
		static unsigned long				getCurrentTime();

	// private socket methods
	#if defined(__LINUX__) || defined(__linux__)
		static void							socketEpoll(InterfaceFunction interfaceFunction);
		static void							initEpoll();
		static void							epollAdd(int newClient, int serverSocket);
		static void							epollRemove(int fd);
		static void							epollAccept(int fd);
	#elif defined(__APPLE__)
		static void							socketKqueue(InterfaceFunction interfaceFunction);
		static void							initKqueue();
		static void							kqueueAdd(int newClient, int serverSocket);
		static void							kqueueRemove(int fd);
		static void							kqueueAccept(int fd);
	#else
		static void							socketSelect(InterfaceFunction interfaceFunction);
		static void							initSelect();
		static void							selectAdd(int newClient, int serverSocket);
		static void							selectRemove(int fd);
		static void							selectAccept(int fd);
	#endif
};

#endif