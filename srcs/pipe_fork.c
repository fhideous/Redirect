#include "pipex.h"

int	fork_execv(char **argvs, char **env,
					const int fd_from, const int fd_to)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (dup2(fd_from, 0) < 0)
			return (3);
		close(fd_from);
		if (dup2(fd_to, 1) < 0)
			return (3);
		close(fd_to);
		if (execve(argvs[0], argvs, env))
			return (3);
	}
	else if (pid > 1)
	{
		wait(NULL);
		close(fd_to);
		close(fd_from);
	}
	if (pid < 0)
		return (3);
	return (0);
}

int	pipes(t_all *all, char **env)
{
	int		i;
	int		fd_from;
	int		fd_to;

	i = 0;
	fd_from = all->fd0;
	while (i < all->commands_numb - 1 && all->argvs[i + 1])
	{
		pipe(all->fd_pipe);
		fd_to = all->fd_pipe[1];
		all->err = fork_execv(all->argvs[i], env, fd_from, fd_to);
		if (all->err)
			return (all->err);
		fd_from = all->fd_pipe[0];
		i++;
	}
	fd_to = all->fd1;
	all->err = fork_execv(all->argvs[i], env, fd_from, fd_to);
	return (all->err);
}
