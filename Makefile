SRCS			= $(LIBDIR)/

OBJS			= $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -I$(LIBDIR)

LIBDIR			= ./libexcet
NAME			= libexcet.a

all:			$(NAME)

$(NAME):		$(OBJS)
				@ar rc $(NAME) $(OBJS)
				@$(CC) $(CFLAGS) -o libexcet.a main.c 
				@echo "\033[0;92m* $(NAME) was created *\033[0m"

%.o : %.c
	@$(CC) $(CCFLAG) $(INCLUDE) $< -c -o $@

clean:
				@$(RM) $(OBJS) $(BONUS_OBJS) $(PRINTF_OBJS)

fclean:			clean
				@$(RM) $(NAME)
				@echo "\033[0;91m* $(NAME) was removed *\033[0m"

re:				fclean $(NAME)

.PHONY:			all clean fclean re bonus