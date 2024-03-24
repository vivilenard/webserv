#!/bin/bash
echo "Starting Webserv"
cd /Webserv
make re
valgrind --leak-check=full ./Webserv_linux example.conf