#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_all
{
	char		***argvs;
	int			is_hd;
	char		*hd;
	char		**path;
	int			*fd_pipe;
	int			fd0;
	int			fd1;
	int			commands_numb;
	int			err;
}			t_all;

int fd_open(t_all *all, const int argc, const char **argv);

int take_path(t_all *all, char **env);
int split_commands(t_all *all,const int argc,const char **argv);
int fd_pipe_alloc(t_all *all);

int find_commands_path(t_all *all);

int pipes(t_all *all, char **env);


#endif
