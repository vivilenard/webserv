# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/17 12:55:54 by pharbst           #+#    #+#              #
#    Updated: 2024/03/20 14:09:28 by pharbst          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# openssl req -new -newkey rsa:2048 -nodes -keyout test.key -subj "/C=DE/ST=Baden-Württemberg /L=Heilbronn /O=42/CN=42heilbronn.com" -x509 -days 365 -out test.crt && cat test.key test.crt >> test.pem

include color.mk

ifeq ($(UNAME), Darwin)
PRINT	= printf
PRONAME = Webserv
else ifeq ($(UNAME), Linux)
PRINT	= printf
PRONAME = Webserv_linux
endif
ifeq ($(OS_LIKE), Debian)
PRINT	= echo
endif

# Submodules
SOCKETMANAGER_DIR	:= ./socketManager
SOCKETMANAGER		:= $(SOCKETMANAGER_DIR)/libsocketManager.a

INC_DIR		:= 	-I./include/ \
			-I./parse/ \
			-I$(SOCKETMANAGER_DIR)/include \
			# -I./include/config/ \
			# -I./include/error/ \
			# -I./include/httpTransfer/ \

ifeq ($(UNAME), Darwin)
SUDO		:= 
SSLCFLAGS	:= -I$(shell brew --prefix)/opt/openssl@3/include/
SSLLDFLAGS	:= -L$(shell brew --prefix)/opt/openssl@3/lib/ -lssl -lcrypto
CFLAGS		:= -Wall -Wextra -Werror -MMD -MP -g -std=c++98 $(SSLCFLAGS) $(INC_DIR)
LDFLAGS		:= $(SSLLDFLAGS) -L$(SOCKETMANAGER_DIR) -lsocketManager
else ifeq ($(UNAME), Linux)
SUDO		:= sudo
CFLAGS		:= -Wall -Wextra -Werror -MMD -MP -g -std=c++98 $(INC_DIR)
LDFLAGS		:= -lssl -lcrypto -L./$(SOCKETMANAGER_DIR) -lsocketManager
endif

CC		= c++

# -MMD and -MP are ussed to create dependecy files
# CFLAGS	= -Wall -Wextra -Werror -MMD -MP -g -std=c++98 -pedantic $(INC_DIR)

# add source files with header with the same name
SOURCE	=	# Config.cpp \
			# Error.cpp \
			# httpTransfer.cpp

HEADER	= $(addprefix $(INC_DIR), $(SOURCE:.cpp=.hpp))

# add other header files here
HEADER	+= 

# add source files without header with the same name and the file with the main function has to be the first in the list
SRCS	=	webserver.cpp \
			Request.cpp \
			Response.cpp \
			Status.cpp \
			Delete.cpp \
			Get.cpp \
			Post.cpp \
			Cgi.cpp \
			read.cpp \
			configFile.cpp \
			configUtil.cpp \
			location.cpp \
			mimeTypes.cpp \
			root.cpp \
			$(SOURCE)

OBJ_DIR	= ./obj/
ifeq ($(UNAME), Darwin)
OBJ_DIR = ./obj/mac/
else ifeq ($(UNAME), Linux)
OBJ_DIR = ./obj/linux/
endif

OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.cpp=.o))


# in case of subdirectories in the src folder add them here
VPATH := src include src/config src/error src/httpTransfer src/parse

all:
	echo $(UNAME) $(OS) $(OS_LIKE)
	@$(MAKE) -s proname_header
	@$(MAKE) -s std_all

std_all: $(SOCKETMANAGER)
	@$(PRINT) "$(CLEARLINE)%s$(RESET)\n" "$(FPurple)Compiling $(PRONAME)"
	@-include $(OBJS:.o=.d) > /dev/null 2>&1
	@$(MAKE) -s $(PRONAME)
	@$(PRINT) "$(SETCURUP)$(CLEARLINE)\r$(FPurple)%-33s$(FGreen)$(TICKBOX)$(RESET)\n" "Compiling $(PRONAME)"

$(PRONAME): $(OBJS) $(SOCKETMANAGER)
# @printf "\n\n$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(PRONAME)\n\n"
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(PRONAME)

$(OBJ_DIR)%.o: %.cpp
ifeq ($(shell test -d $(OBJ_DIR) || echo $$?), 1)
	@$(PRINT) "$(CLEARLINE)\r$(Yellow)creting obj dir$(RESET)"
	@mkdir -p $(OBJ_DIR)
endif
	@$(PRINT) "$(CLEARLINE)\r%-40s$(RESET)" "$(Yellow)Compiling $< ..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(SOCKETMANAGER):
ifeq ($(shell test $(SOCKETMANAGER_DIR)/Makefile || echo $$?), 1)
	@$(PRINT) "$(CLEARLINE)\r%-40s$(RESET)" "$(FCyan)Initializing submodule"
	@git submodule update --init > /dev/null
	@$(PRINT) "$(FGreen)$(TICKBOX)$(RESET)\n"
endif
	@$(PRINT) "$(CLEARLINE)\r%-40s\n$(RESET)" "$(FCyan)Compiling submodule"
	@$(MAKE) std_all -s -C $(SOCKETMANAGER_DIR)
	@$(PRINT) "$(SETCURUP)$(SETCURUP)$(SETCURUP)%-40s$(RESET)$(FGreen)$(TICKBOX)$(RESET)\n\n\n" "$(FCyan)Compiling submodule"

clean:
	@$(MAKE) -s proname_header
	@$(PRINT) "%-40s$(RESET)" "$(FRed)Cleaning $(PRONAME)"
	@$(MAKE) -s std_clean
	@$(PRINT) "$(FGreen)$(TICKBOX)$(RESET)\n"

fclean:
	@$(MAKE) -s proname_header
	@$(MAKE) -s cleanator

re:
	@$(MAKE) -s proname_header
	@$(MAKE) -s cleanator 2> /dev/null
	@$(MAKE) -s std_all 2> /dev/null

run: re
	./$(PRONAME)

arch:
	-docker rm -f webserv
	docker-compose -f container/Arch/docker-compose.yml build
	docker-compose -f container/Arch/docker-compose.yml up

ubuntu:
	-docker rm -f webserv
	docker-compose -f container/Ubuntu/docker-compose.yml build
	docker-compose -f container/Ubuntu/docker-compose.yml up

debian:
	-docker rm -f webserv
	docker-compose -f container/Debian/docker-compose.yml build
	docker-compose -f container/Debian/docker-compose.yml up

alpine:
	-docker rm -f webserv
	docker-compose -f ./container/Alpine/docker-compose.yml build
	docker-compose -f ./container/Alpine/docker-compose.yml up

logs:
	docker logs webserv

restart_docker:
	docker restart webserv

siege:
	brew install siege

std_clean:
	@rm -rf $(OBJ_DIR)

cleanator:
	@$(PRINT) "%-40s$(RESET)\n" "$(FRed)FCleaning $(PRONAME)"
	@rm -rf $(OBJ_DIR)
	@rm -f $(PRONAME)
	@$(MAKE) cleanator -s -C socketManager
	@$(PRINT) "$(SETCURUP)$(SETCURUP)$(CLEARLINE)%-40s$(FGreen)$(TICKBOX)$(RESET)\n\n" "$(FRed)FCleaning $(PRONAME)"

proname_header:
	@$(PRINT) "$(FYellow)╔══════════════════════╗\n\
$(FYellow)║$(FRed)          (    (      $(FYellow)║$(RESET)\n\
$(FYellow)║$(FRed)     (    )\\ ) )\\ )   $(FYellow)║$(RESET)\n\
$(FYellow)║$(FRed)     )\\  (()/((()/(   $(FYellow)║$(RESET)\n\
$(FYellow)║$(FRed)   (((_)  /(_))/(_))  $(FYellow)║$(RESET)\n\
$(FYellow)║$(FRed)   )\\$(FBlue)___ $(FRed)($(FBlue)_$(FRed))) ($(FBlue)_$(FRed)))    $(FYellow)║$(RESET)\n\
$(FYellow)║$(FRed)  (($(FBlue)/ __|| _ \\| _ \\   $(FYellow)║$(RESET)\n\
$(FYellow)║$(FBlue)   | ($(FBlue)__ |  _/|  _/   $(FYellow)║$(RESET)\n\
$(FYellow)║$(FBlue)    \\___||_|  |_|     $(FYellow)║$(RESET)\n\
$(FYellow)╚══════════════════════╝\n$(RESET)"

.PHONY: all clean fclean re pro_header std_all std_clean cleanator proname_header