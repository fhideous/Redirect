
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
