NAME = webserv

CFLAGS = 

CC = g++

INCLUDE_DIR = inc
OBJS_DIR = objs
SRC_DIR = src

INC := -I $(INCLUDE_DIR)

SRCS = 	main.cpp \
		server/server.cpp \
		client/client.cpp \

OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))

all: $(NAME)
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(NAME): $(OBJS)
	@echo "\033[1;32mCompiling project...\033[0m"
	@$(CC) $(CFLAGS) $(INC) $^ -o $(NAME)

run: $(NAME)
	@./$(NAME)

clean:
	@rm -rf $(OBJS_DIR)
	@echo "\033[1;34mA\033[1;31ml\033[1;32ml \033[1;33mc\033[1;30ml\033[1;35me\033[1;36ma\033[1;37mn\033[0m"

fclean: clean
	@rm -rf webserv
	@echo "\033[0;31mFiles killed💀❌\033[0m"

re: fclean all

.PHONY: all clean fclean re