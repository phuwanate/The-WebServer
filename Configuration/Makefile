NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS =	main.cpp \
		Config.cpp \
		LocationBlock.cpp \
		ServerBlock.cpp \
		utils.cpp \
		ServerHandler.cpp \
		RequestResponse.cpp

HEADERS =	Config.hpp \
			LocationBlock.hpp \
			ServerBlock.hpp \
			utils.hpp \
			ServerHandler.hpp \
			RequestResponse.hpp

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