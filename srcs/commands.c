#include "pipex.h"

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
    char    *tmp;
    char    *full_path;
    int        i;

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
    int    i;
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
