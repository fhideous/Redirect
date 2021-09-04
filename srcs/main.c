
#include "pipex.h"

/*
*	for wait();
*/
#include <sys/types.h>
#include <sys/wait.h>

void all_init(void* all, size_t n)
{
	while (n--)
		((unsigned char *)all)[n] = '\0';
}

void struct_free(t_all *all)
{
	int i;
	int j;

	if (all->is_hd)
	{
		unlink(".heredoc");
		free(all->hd);
	}
	if (all->path)
	{
		i = 0;
		while ((all->path + i) && *(all->path + i) )
			free (*(all->path + i++));
		free (all->path);
	}
	if (all->argvs)
	{
		i = 0;
		while ((all->argvs + i) && (i < all->commands_numb))
		{
			j = 0;
			while ((*(all->argvs + i) + j) && *(*(all->argvs + i) + j))
				free(*(*(all->argvs + i) + j++ ));
			free(*(all->argvs + i ));
			i++;
		}
		free (all->argvs);                 
	}
	if (all->fd_pipe)
		free(all->fd_pipe);
}

int error(t_all *all)
{
	if (all->err == 1)
		write(2, "Error: wrong number of arguments.\n", 35);
	else if (all->err == 2)
		write(2, "Error: there is no PATH in env.\n", 33); 
	else  
		write(2, "Original error\n", 16);
	if (all->err == -1)
		struct_free(all); 
	return (all->err);
}   

int	heredoc_2(t_all *all)
{
	char	buf[100];
	int		fd;
	int		f;

	f = 0;
	fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
		return (all->err = -1);
	while (1)
	{
		ft_bzero(buf, 100);
		write(1, "> ", 2);
		f = read(0, buf, 99);
		if (!ft_strncmp(buf, all->hd, ft_strlen(all->hd)))
		{
			close(fd);
			return (0);
		}
		write(fd, buf, ft_strlen(buf));
	}
}

int heredoc(t_all *all, const int argc, const char **argv)
{
	all->is_hd = 1;
	if (argc < 6)
		return (4);
	all->hd = ft_strjoin(argv[2], "\n");
	all->fd1 = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (heredoc_2(all))
		return (4);
	all->fd0 = open(".heredoc", O_RDONLY);
	if (all->fd0 == -1 || all->fd1 == -1)
		all->err = 3;
	return (all->err);
}

int fd_open(t_all *all, const int argc, const char **argv)
{
	if (!ft_strncmp(argv[1], "here_doc", 9))
	{
		if (heredoc(all, argc, argv))
			return (1);
	}
	else
	all->fd0 = open(argv[1], O_RDONLY);
	all->fd1 = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (all->fd1 == -1 ||
		all->fd0 == -1 )
		all->err = 3;
	return (all->err);
} 

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

int fd_pipe_alloc(t_all *all)
{
	all->fd_pipe = ft_calloc( 2, sizeof(int *));
	if (!all->fd_pipe)
		return (all->err = -1);
	return (0);
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

// -1 memory error
//  0 path find
//  1 path not find 

int check_curr_dir(char **argv)
{
	if (!ft_strncmp(argv[0], "./", 2) && argv[0][3] != '\0')
		if (access(argv[0], F_OK))
			return (0);
	return (1);
}

int free_return(char *data, int n)
{
	free(data);
	return n;
}

int check_all_path_dir(char **argv, char **path)
{
	char	*tmp;
	char	*full_path;
	int		i;

	tmp = ft_strjoin("/", argv[0]);
	if (!tmp)
		return (-1);
	i = -1;
	while (*(path + (++i)))
	{
		full_path = ft_strjoin(*(path + i), tmp);
		if (!full_path)
			return(free_return(tmp, -1));
		if (!access(full_path, F_OK))
		{
			free(argv[0]);
			argv[0] = full_path;
			free(tmp);
			return (0); 
		}
		free (full_path);
	}
	free (tmp);
	return (1);
}

int find_commands_path(t_all *all)
{
	int	i;
	int j;

	i = -1;
	while (++i < all->commands_numb)
	{
		j = check_curr_dir(all->argvs[i]);
		if (j == -1)
			return (j);
		else if (j == 0)
			continue;
		j = check_all_path_dir(all->argvs[i], all->path);
		if (j == 0)
			continue;
		return (j);		
	}
	return (0);
}

int fork_execv(char **argvs, char **env,
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

int pipes(t_all *all, char **env)
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

int main(int argc, const char **argv, char **env)
{
	t_all all;

	all_init(&all, sizeof(all));
	// for bonus < 5, for origin != 5
	if (argc < 5)
	{
		all.err = 1;
		return (error(&all));
	}
	if (fd_open(&all, argc, argv))
		return (error(&all));
	if (take_path(&all, env))
		return (error(&all));
	if (split_commands(&all, argc, argv))
		return (error(&all));
	if (fd_pipe_alloc(&all))
		return (error(&all));
	if (find_commands_path(&all))
		return (error(&all));   
	if (pipes(&all, env))
		return (error(&all));
	struct_free(&all);
	return (0);
}