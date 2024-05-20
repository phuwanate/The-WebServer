NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS_DIR = src/
INCLUDES_DIR = includes/
OBJS_DIR = objects/

SRCS = $(SRCS_DIR)main.cpp \
       $(SRCS_DIR)Config.cpp \
       $(SRCS_DIR)LocationBlock.cpp \
       $(SRCS_DIR)ServerBlock.cpp \
       $(SRCS_DIR)utils.cpp \
       $(SRCS_DIR)ServerHandler.cpp \
       $(SRCS_DIR)Client.cpp \
       $(SRCS_DIR)mock.cpp \
       $(SRCS_DIR)Request.cpp \
       $(SRCS_DIR)Response.cpp \
       $(SRCS_DIR)HttpStatus.cpp \
       $(SRCS_DIR)Cgi.cpp

HEADERS = $(INCLUDES_DIR)Config.hpp \
          $(INCLUDES_DIR)LocationBlock.hpp \
          $(INCLUDES_DIR)ServerBlock.hpp \
          $(INCLUDES_DIR)utils.hpp \
          $(INCLUDES_DIR)ServerHandler.hpp \
          $(INCLUDES_DIR)Client.hpp \
          $(INCLUDES_DIR)mock.hpp \
          $(INCLUDES_DIR)Request.hpp \
          $(INCLUDES_DIR)Response.hpp \
          $(INCLUDES_DIR)HttpStatus.hpp \
          $(INCLUDES_DIR)Cgi.hpp


OBJS = $(OBJS_DIR)main.o \
       $(OBJS_DIR)Config.o \
       $(OBJS_DIR)LocationBlock.o \
       $(OBJS_DIR)ServerBlock.o \
       $(OBJS_DIR)utils.o \
       $(OBJS_DIR)ServerHandler.o \
       $(OBJS_DIR)Client.o \
       $(OBJS_DIR)mock.o \
       $(OBJS_DIR)Request.o \
       $(OBJS_DIR)Response.o \
       $(OBJS_DIR)HttpStatus.o \
       $(OBJS_DIR)Cgi.o

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp $(HEADERS)
	mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
