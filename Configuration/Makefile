NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS =	main.cpp \
		Config.cpp \
		LocationBlocks.cpp \
		ServerBlocks.cpp

HEADERS =	Config.hpp \
			LocationBlocks.hpp \
			ServerBlocks.hpp

OBJS = $(SRCS:.cpp=.o)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS): $(HEADERS)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re