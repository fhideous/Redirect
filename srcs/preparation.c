#include "pipex.h"

int take_path(t_all *all, char **env)
{
	int i;

	i = -1;
	while (env[++i])
		if (!ft_strncmp(env[i], "PATH=", 5))
			break ;
	if (!env[i])
		all->err = 2;
	all->path = ft_split(env[i] + 5, ':');
	if (!all->path)
		all->err = 3;
	return (all->err);
}

int split_commands(t_all *all,const int argc,const char **argv)
{
	int i;

	all->commands_numb = argc - 3 - all->is_hd;
	all->argvs = ft_calloc(all->commands_numb, sizeof(char *));
	if (!all->argvs)
		return (all-> err = -1);
	i = 0;
	while (i < all->commands_numb)
	{
		all->argvs[i] = ft_split(argv[i + 2 + all->is_hd], ' ');
		if (!all->argvs[i])
			return (all->err = -1);
		i++;
	}
	return (0);
}

int fd_pipe_alloc(t_all *all)
{
	all->fd_pipe = ft_calloc( 2, sizeof(int *));
	if (!all->fd_pipe)
		return (all->err = -1);
	return (0);
}
