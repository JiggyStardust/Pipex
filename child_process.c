/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sniemela <sniemela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 11:48:27 by sniemela          #+#    #+#             */
/*   Updated: 2024/09/24 10:36:44 by sniemela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	is_whitespace(char *str)
{
	while (*str)
	{
		if (*str != 32 && (*str < 9 || *str > 13))
			return (0);
		str++;
	}
	return (1);
}

void	check_access(t_pipex *pipex, char *cmd, char **cmd_arr)
{
	int	test_fd;

	if (access(cmd, F_OK) == -1)
	{
		if (ft_strchr(cmd, '/'))
			errno = ENOENT;
		else
			errno = 0;
		exit_cmd_error(pipex, cmd, cmd_arr);
	}
	test_fd = open(cmd, __O_DIRECTORY);
	if (test_fd != -1)
	{
		close(test_fd);
		if (cmd[ft_strlen(cmd) - 1] == '/')
			errno = EISDIR;
		else
			errno = 0;
		exit_cmd_error(pipex, cmd, cmd_arr);
	}
	if (access(cmd, X_OK) == -1)
	{	
		errno = EACCES;
		exit_cmd_error(pipex, cmd, cmd_arr);
	}
}

void	setup_child(t_pipex *pipex, int i)
{
	if (i == 0)
	{
		pipex->infile = open(pipex->argv[1], O_RDONLY);
		if (pipex->infile < 0)
			exit_with_error(pipex, pipex->argv[1]);
		if (dup2(pipex->infile, STDIN_FILENO) < 0)
			exit_with_error(pipex, "dup2 stdin fail child 1");
		if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) < 0)
			exit_with_error(pipex, "dup2 stdout fail");
		close(pipex->pipe_fd[0]);
	}
	else if (i == 1)
	{
		pipex->outfile = open(pipex->argv[4], O_CREAT | O_WRONLY
				| O_TRUNC, 0644);
		if (pipex->outfile < 0)
			exit_with_error(pipex, pipex->argv[4]);
		if (dup2(pipex->pipe_fd[0], STDIN_FILENO) < 0)
			exit_with_error(pipex, "dup2 stdin fail child 2");
		if (dup2(pipex->outfile, STDOUT_FILENO) < 0)
			exit_with_error(pipex, "dup2 stdout fail");
		close(pipex->pipe_fd[1]);
	}
	free_resources(pipex);
}

int	fork_and_execute(t_pipex *pipex, int cmd_i)
{
	pid_t	pid;
	char	**cmd;
	char	*cmd_path;

	pid = fork();
	if (pid < 0)
		exit_with_error(pipex, "Fork error");
	if (pid == 0)
	{
		setup_child(pipex, cmd_i);
		cmd = ft_split(pipex->argv[cmd_i + 2], ' ');
		if (!cmd || !cmd[0] || is_whitespace(pipex->argv[cmd_i + 2]))
			exit_cmd_error(pipex, pipex->argv[cmd_i + 2], cmd);
		cmd_path = get_path(pipex, cmd[0]);
		if (!cmd_path)
			check_access(pipex, cmd[0], cmd);
		check_access(pipex, cmd_path, cmd);
		execve(cmd_path, cmd, pipex->envp);
		free(cmd_path);
		exit_cmd_error(pipex, cmd[0], cmd);
	}
	return (pid);
}
