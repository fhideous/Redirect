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
	char		**path;
	int			*fd_pipe;
	int			fd0;
	int			fd1;
	int			commands_numb;
	int			err;
}			t_all;



#endif