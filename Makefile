NAME		:= computorv
SRC_FR	:= ./src/
OBJ_FR	:= obj/
INCLUDE	:= -I ./include
CC			:= c++

ifdef DEBUG
CFLAGS	:= -g -fsanitize=address
else
CFLAGS	:= -Wall -Werror -Wextra
endif

SRC_FILES	:= utils/tester_main.cpp \
						 lexer/Token.cpp \
						 lexer/Lexer.cpp \
						 parser/Parser.cpp \
						 polynomial/Polynomial.cpp \
						 polynomial/Term.cpp \

OBJS		:= $(addprefix $(OBJ_FR), $(SRC_FILES:.cpp=.o))

all: ${NAME}
ifdef DEBUG
	@echo "$(CYAN)Using Debug...$(NC)"
endif

$(OBJ_FR)%.o : ${SRC_FR}%.cpp
	@mkdir -p $(dir $@)
	@echo "$(PURPLE)Compiling... $(NC)🧱 $(PURPLE) $< $(NC)"
	@${CC} ${CFLAGS} $(INCLUDE) -c $^ -o $@

$(NAME): ${OBJS}
	@echo "$(BLD_GRN)Building... 🏗$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@
	@echo "$(BLD_GRN)Done! 🏠$(NC)"

clean:
	@echo "$(CYAN)Cleaning...$(NC) 🧹"
	@rm -rf ${OBJ_FR}

fclean: clean
	@echo "$(RED)Destroying...$(NC) 💥"
	@rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re

# Reset
NC		:= \033[0m# 	Text Reset

# Regular Colors
BLACK	:= \033[0;30m#	Black
RED		:= \033[3;31m#	Red
GREEN	:= \033[3;32m#	Green
BLD_GRN	:= \033[1;32m#	Green but bold
YELLOW	:= \033[3;33m#	Yellow
BLUE	:= \033[1;34m#	Blue
PURPLE	:= \033[3;35m#	Purple
BLD_PUR	:= \033[1;35m#	Purple but bold
CYAN	:= \033[3;36m#	Cyan
WHITE	:= \033[1;37m#	White

