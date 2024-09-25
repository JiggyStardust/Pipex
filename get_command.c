/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sniemela <sniemela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:38:06 by sniemela          #+#    #+#             */
/*   Updated: 2024/09/23 10:43:20 by sniemela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*is_cmd_path(t_pipex *pipex, const char *cmd)
{
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	else
	{
		errno = ENOENT;
		ft_putstr_fd("pipex: ", 2);
		perror(cmd);
		free_resources(pipex);
		exit(127);
	}
}

char	*constr_full_path(char **paths, const char *cmd)
{
	char	*temp_path;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp_path = ft_strjoin(paths[i], "/");
		if (!temp_path)
			return (NULL);
		full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_env(t_pipex *pipex, const char *cmd)
{
	char	*path_env;
	int		i;

	i = 0;
	while (pipex->envp[i] != NULL)
	{
		if (ft_strncmp(pipex->envp[i], "PATH=", 5) == 0)
		{
			path_env = pipex->envp[i] + 5;
			break ;
		}
		i++;
	}
	if (pipex->envp[i] == NULL)
	{
		errno = ENOENT;
		exit_cmd_error(pipex, (char *)cmd, NULL);
	}
	return (path_env);
}

char	*get_path(t_pipex *pipex, const char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (ft_strchr(cmd, '/'))
		return (is_cmd_path(pipex, cmd));
	path_env = get_env(pipex, cmd);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = constr_full_path(paths, cmd);
	free_2d_array(paths);
	return (full_path);
}
