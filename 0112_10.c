#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void	err(char *s, char *v)
{
	while(*s)
		write(2, s++, 1);
	if(v)
		while(*v)
			write(2, v++, 1);
	write(2, "\n", 1);
}

void	ft_exe(char **av, int i, int tmp, char **env)
{
	av[i] = NULL;
	dup2(tmp, 0);
	close(tmp);
	execve(av[0], av, env);
	err("error: cannot execute ", av[0]);
	exit(1);
}

int		main(int ac, char **av, char **env)
{
	(void)ac;
	int i = 0, fd[2], tmp = dup(0);

	while (av[i] && av[i + 1])
	{
		av += i +1;
		i = 0;
		while(av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;

		if (!strcmp(av[0],"cd"))
		{
			if(i != 2)
				err("error: cd: bad arguments", NULL);
			else if(chdir(av[1]))
				err("error: cd: cannot change directory to ", av[1]);
		}
		else if (i && (!av[i] || !strcmp(av[i], ";")))
		{
			if (!fork())
				ft_exe(av, i, tmp, env);
			else
			{
				close(tmp);
				while(waitpid(-1, NULL, WUNTRACED) != -1)
					;
				tmp = dup(0);
			}
		}
		else if (i && !strcmp(av[i], "|"))
		{
			pipe(fd);
			if(!fork())
			{
				dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);
				ft_exe(av, i, tmp, env);
			}
			else
			{
				close(tmp);
				close(fd[1]);
				tmp = fd[0];
			}
		}
	}
	close(tmp);
	return (0);
}
