/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_and_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sniemela <sniemela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:50:14 by sniemela          #+#    #+#             */
/*   Updated: 2024/09/24 10:37:11 by sniemela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_pipes(t_pipex *pipex)
{
	close(pipex->pipe_fd[0]);
	close(pipex->pipe_fd[1]);
}

void	free_resources(t_pipex *pipex)
{
	if (pipex->infile >= 0)
		close(pipex->infile);
	if (pipex->outfile >= 0)
		close(pipex->outfile);
	close_pipes(pipex);
}

void	exit_with_error(t_pipex *pipex, char *msg)
{
	int	exit_code;

	exit_code = 0;
	if (msg == pipex->argv[1] || msg == pipex->argv[4])
		ft_putstr_fd("pipex: ", 2);
	if (errno == EACCES || errno == ENOENT || errno == EISDIR)
	{
		perror(msg);
		exit_code = 1;
	}
	else
		perror(msg);
	free_resources(pipex);
	exit(exit_code);
}

void	exit_cmd_error(t_pipex *pipex, char *cmd, char **cmd_array)
{
	int	error;

	error = errno;
	if (errno == EACCES || errno == ENOENT || errno == EISDIR)
	{
		ft_putstr_fd("pipex: ", 2);
		perror(cmd);
	}
	else
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found", 2);
		ft_putstr_fd("\n", 2);
	}
	if (cmd_array)
		free_2d_array(cmd_array);
	if (cmd && cmd != pipex->argv[2] && cmd != pipex->argv[3])
		free(cmd);
	free_resources(pipex);
	if (error == EACCES || error == EISDIR)
		exit(126);
	else
		exit(127);
}

void	free_2d_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}
