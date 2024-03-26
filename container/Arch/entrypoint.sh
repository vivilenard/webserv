#!/bin/bash
cd /Webserv
make
valgrind --leak-check=full ./Webserv_linux example.conf