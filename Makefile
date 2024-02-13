NAME=webserv

SRC = main.cpp \
      httpTransfer.cpp \
      Server.cpp \
      Error.cpp \
      parse/configFile.cpp \
      parse/configUtil.cpp \
      parse/root.cpp \
      parse/location.cpp \
      defaultConf/defaultConf.cpp \

OBJ = $(SRC:.cpp=.o)

CXXFLAGS = -Wall -Werror -Wextra -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	@c++ $(LDFLAGS) $^ -o $@
	@printf "${RED}compiled ${DF}\n"

%.o: %.cpp
	@c++ $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@printf "${GREEN}cleaned ${DF}\n"

fclean: clean
	@rm -f ./$(NAME)

re: fclean all

#colors
PURPLE=\033[0;95m
GREEN=\033[92m
CYAN=\033[96m
BLUE=\033[94m
WHITE=\033[0m
RED=\033[91m
YELLOW=\033[33m
DF=

.PHONY: all clean fclean re
