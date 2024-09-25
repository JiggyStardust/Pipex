# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sniemela <sniemela@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 10:48:27 by sniemela          #+#    #+#              #
#    Updated: 2024/09/24 10:29:04 by sniemela         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CFLAGS = -Wall -Werror -Wextra
CC = gcc

SOURCES = pipex.c \
			mem_and_errors.c \
			child_process.c \
			get_command.c
		
OBJS = $(SOURCES:.c=.o)

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a 

INCLUDES = -I$(LIBFT_DIR)

CYAN_BOLD := \e[1;96m
HIB_GREEN := \e[1;92m
GREEN_UNDER := \e[4;32m
ANSI_RESET := \033[0m	

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "$(HIB_GREEN)$(NAME) has been successfully built!$(ANSI_RESET)"

%.o: %.c
	@echo "Compiling included files..."
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(LIBFT):
	@echo "Building libft..."
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean
	@echo "$(CYAN_BOLD)Cleaned up object files from pipex and libft.$(ANSI_RESET)"

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean
	@echo "$(CYAN_BOLD)Removed $(NAME) and libft.a$(ANSI_RESET)"

re: fclean all
	@echo "$(GREEN_UNDER)Rebuilt everything from scratch$(ANSI_RESET)"

.PHONY: all clean fclean re