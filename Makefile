NAME	= webserv

SRCS_DIR	= srcs
# OBJS_DIR	= objs

# SRCS_NAMES	= main.cpp SimpleServer.cpp SimpleSocket.cpp BindingSocket.cpp ListeningSocket.cpp ConnectingSocket.cpp
SRCS_FILES	= $(SRCS_DIR)/main.cpp $(wildcard $(SRCS_DIR)/*/*/*.cpp)
# SRCS_FILES	= srcs/main.cpp srcs/Networking/Servers/SimpleServer.cpp srcs/Networking/Sockets/SimpleSocket.cpp srcs/Networking/Sockets/BindingSocket.cpp srcs/Networking/Sockets/ListeningSocket.cpp srcs/Networking/Sockets/ConnectingSocket.cpp
# OBJS_FILES	= $(addprefix $(OBJS_DIR)/, $(SRCS_NAMES:.cpp=.o))
OBJS_FILES	= $(SRCS_FILES:.cpp=.o)

CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98
FSAN		= -fsanitize=address -g

all:	$(NAME)

$(NAME):	$(OBJS_FILES)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS_FILES)

%.o:	%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

test:	$(OBJS_FILES)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS_FILES)
	@./$(NAME) $(ARG)

sanitize:	$(OBJS_FILES)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS_FILES) $(FSAN)
	@./$(NAME) $(ARG)

clean:
	@$(RM) $(OBJS_FILES)

fclean:	clean
	@$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
