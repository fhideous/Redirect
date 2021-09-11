#include "pipex.h"

int    heredoc_2(t_all *all)
{
	char    buf[100];
	int        fd;
	int        f;

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
		all->err = heredoc(all, argc, argv);
	else
	{
		all->fd0 = open(argv[1], O_RDONLY);
		all->fd1 = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (all->fd1 == -1 ||
			all->fd0 == -1 )
			all->err = 3;
	}
	return (all->err);
} 
