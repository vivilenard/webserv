# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pharbst <pharbst@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/17 12:55:54 by pharbst           #+#    #+#              #
#    Updated: 2024/03/11 10:42:57 by pharbst          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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

CC		= c++

# -MMD and -MP are ussed to create dependecy files
CFLAGS	= -Wall -Wextra -Werror -MMD -MP -g -fsanitize=address -std=c++98 -pedantic $(INC_DIR)

INC_DIR	= 	-I./include/ \
			-I./include/socketManager/ \
			-I./include/Interface/ \
			-I./parse/ \
			# -I./include/config/ \
			# -I./include/error/ \
			# -I./include/httpTransfer/ \

# add source files with header with the same name
SOURCE	=	socketManager.cpp \
			Interface.cpp \
			# Config.cpp \
			# Error.cpp \
			# httpTransfer.cpp

HEADER	= $(addprefix $(INC_DIR), $(SOURCE:.cpp=.hpp))

# add other header files here
HEADER	+= 

# add source files without header with the same name and the file with the main function has to be the first in the list
SRCS	=	webserver.cpp \
			socketManagerTools.cpp \
			socketSEPOLL.cpp \
			InterfaceTools.cpp \
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
			defaultConf.cpp \
			$(SOURCE)

OBJ_DIR	= ./obj/
ifeq ($(UNAME), Darwin)
OBJ_DIR = ./obj/mac/
else ifeq ($(UNAME), Linux)
OBJ_DIR = ./obj/linux/
endif

OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.cpp=.o))


# in case of subdirectories in the src folder add them here
VPATH := src include src/socketManager src/Interface src/config src/error src/httpTransfer parse defaultConf

all:
	@$(MAKE) -s proname_header
	@$(MAKE) -s std_all

std_all:
	@$(PRINT) "%s$(RESET)\n" "$(FPurple)Compiling $(PRONAME)"
	@-include $(OBJS:.o=.d)
	@$(MAKE) -s $(PRONAME)
	@$(PRINT) "$(SETCURUP)$(CLEARLINE)$(SETCURUP)$(CLEARLINE)\r$(FPurple)%-21s$(FGreen)$(TICKBOX)$(RESET)\n" "Compiling $(PRONAME)"

$(PRONAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PRONAME)

$(OBJ_DIR)%.o: %.cpp
ifeq ($(shell test -d $(OBJ_DIR) || echo $$?), 1)
	@$(PRINT) "$(CLEARLINE)\r$(Yellow)creting obj dir$(RESET)"
	@mkdir -p $(OBJ_DIR)
endif
	@$(PRINT) "$(CLEARLINE)\r%-28s$(RESET)" "$(Yellow)Compiling $< ..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -s proname_header
	@$(PRINT) "%-28s$(RESET)" "$(FRed)Cleaning $(PRONAME)"
	@$(MAKE) -s std_clean
	@$(PRINT) "$(FGreen)$(TICKBOX)$(RESET)\n"

fclean:
	@$(MAKE) -s proname_header
	@$(MAKE) -s cleanator

re:
	@$(MAKE) -s proname_header
	@$(MAKE) -s cleanator
	@$(MAKE) -s std_all

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

std_clean:
	@rm -rf $(OBJ_DIR)

cleanator:
	@$(PRINT) "%-28s$(RESET)" "$(FRed)FCleaning $(PRONAME)"
	@rm -rf $(OBJ_DIR)
	@rm -f $(PRONAME)
	@$(PRINT) "$(FGreen)$(TICKBOX)$(RESET)\n"

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