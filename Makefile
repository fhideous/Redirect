NAME = pipex

SRCS =					main.c

HEAD =					pipex.h

HEADPATH = $(addprefix includes/, $(HEAD))

OBJS = $(SRCS:.c=.o)

OBJDIR = objs
			

OBJSPATH = $(addprefix $(OBJDIR)/, $(OBJS))

FLAGS = -g -Wall -Wextra -Werror -Ilibft -Iincludes #-fsanitize=address

vpath %.o objs
vpath %.c srcs
vpath %.h includes

.Phony: all $(NAME) clean fclean libft.a re $(OBJDIR)

all: libft.a $(NAME)

libft.a:
	$(MAKE) -C libft

$(NAME): $(OBJDIR) $(OBJS)
	clang $(FLAGS) -o $(NAME) $(OBJSPATH) libft/libft.a

%.o: %.c $(HEADPATH)
	clang $(FLAGS) -o $(addprefix objs/, $@) -c $<

bonus: libft.a
	$(MAKE) $(NAME) SRCS='$(SRCS:.c=_bonus.c)' HEAD='$(HEAD:.h=_bonus.h)'

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJDIR)

fclean:
	$(MAKE) -C libft fclean
	rm -rf $(OBJDIR) $(NAME)

re: fclean all

