/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sniemela <sniemela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:51:16 by sniemela          #+#    #+#             */
/*   Updated: 2024/09/24 10:35:58 by sniemela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "Libft/libft.h"
# include <unistd.h>
# include <stdarg.h>
# include <fcntl.h>
# include <stdlib.h> 
# include <stdio.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_pipex {
	int		argc;
	char	**argv;
	char	**envp;
	int		infile;
	int		outfile;
	int		pipe_fd[2];
}	t_pipex;

void	close_pipes(t_pipex *pipex);
void	exit_with_error(t_pipex *pipex, char *message);
void	exit_cmd_error(t_pipex *pipex, char *message, char **cmd);
void	free_resources(t_pipex *pipex);
void	free_2d_array(char **array);
int		fork_and_execute(t_pipex *pipex, int cmd_i);
char	*get_path(t_pipex *pipex, const char *cmd);
void	init_pipex(t_pipex *pipex, int argc, char **argv, char **envp);
char	*is_cmd_path(t_pipex *pipex, const char *cmd);
int		open_files(t_pipex *pipex);
void	setup_child(t_pipex *pipex, int i);
void	setup_pipes(t_pipex *pipex);
int		wait_for_children(pid_t pid1, pid_t pid2);

#endif