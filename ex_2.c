#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void	err(char *str, char *arg)
{
	while (*str)
		write(2, str++, 1);
	if (arg)
		while (*arg)
			write(2, arg++, 1);
	write(2, "\n", 1);
}

void	ft_exe(char **av, int i int tmp, char **env)
{
	av[i] = NULL;
	dup2(tmp, 0);
	close(tmp);
	execve(av[0], av, env);
	err("error: cannot execute ", av[0]);
	exit(1);
}

int	main(int ac, char **av, char **env)
{
	int	i = 0, fd[2], tmp;
	(void)ac;
	tmp = dup(0);
	while (av[i] && av[i+1])
	{
		av = &av[i + 1]; //av += i + 1; same
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;

		if (!strcmp(av[0], "cd"))
		{
			if (i != 2)
				err("error: cd: bad arguments\n", NULL);
			else if (chdir(av[1]))
				err("error: cd: cannot change directory to ", av[1]);
		}
		else if (i && (!av[i] || !strcmp(av[i], ";")))
		{
			if (fork() == 0)
				ft_exe(av, i, tmp, env);
			else
			{
				close(tmp);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					;
				tmp = dup(0);
			}
		}
	}
	close(tmp);
	return (0);
}
