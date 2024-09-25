/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sniemela <sniemela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:38:18 by sniemela          #+#    #+#             */
/*   Updated: 2024/09/24 10:35:45 by sniemela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status;

	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

void	init_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->envp = envp;
	pipex->infile = -1;
	pipex->outfile = -1;
	pipex->pipe_fd[0] = -1;
	pipex->pipe_fd[1] = -1;
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	pid1;
	pid_t	pid2;
	int		exit;

	if (argc != 5)
	{
		ft_putstr_fd("Invalid number of arguments\n", 2);
		return (1);
	}
	init_pipex(&pipex, argc, argv, envp);
	if (pipe(pipex.pipe_fd) == -1)
		exit_with_error(&pipex, "Pipe error");
	pid1 = fork_and_execute(&pipex, 0);
	pid2 = fork_and_execute(&pipex, 1);
	close_pipes(&pipex);
	exit = wait_for_children(pid1, pid2);
	free_resources(&pipex);
	return (exit);
}
