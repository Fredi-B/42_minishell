#include "minishell.h"

int	g_exit_status;

static void	init_struct_and_signals(t_data *data);
static void	cpy_env(t_data *data, char **env);
static char	*get_prompt(t_data *data);
static int	lexing_and_parsing(t_data *data, char *read);

static char	*str_append_chr(char *str, char append)
{
	char	*new_str;
	int		i;

	if (str == NULL)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 2);
	if (new_str != NULL)
	{
		i = 0;
		while (str[i])
		{
			new_str[i] = str[i];
			i++;
		}
		new_str[i] = append;
		new_str[i + 1] = '\0';
	}
	free(str);
	return (new_str);
}

static char	*minishell_gnl_free_line(char *line)
{
	free(line);
	return (NULL);
}

char	*minishell_get_next_line(int fd)
{
	char	*line;
	char	buffer;
	int		check;

	line = ft_strdup("");
	if (line == NULL)
		return (NULL);
	check = read(fd, &buffer, 1);
	if (check == -1 || check == 0)
		return (minishell_gnl_free_line(line));
	while (check > 0)
	{
		line = str_append_chr(line, buffer);
		if (line == NULL)
			return (NULL);
		if (buffer == '\n')
			return (line);
		check = read(fd, &buffer, 1);
	}
	if (check == -1)
		return (minishell_gnl_free_line(line));
	return (line);
}

int	main(int argc __attribute__ ((unused)), \
			char **argv __attribute__ ((unused)), char **env)
{
	t_data	data;
	char	*read;

	cpy_env(&data, env);
	while (1)
	{
		init_struct_and_signals(&data);
		read = get_prompt(&data);
		if (read == NULL)
			break ;
		if (read[0] == '\0')
		{
			free(read);
			continue ;
		}
		if (lexing_and_parsing(&data, read) == ERROR)
			continue ;
		if (isatty(STDIN_FILENO)) //for testing
			add_history(read);
		free(read);
		launch_cmds(&data);
		free_data(&data);
		//system("leaks minishell");
	}
	if (data.envp != NULL)
		free_two_d_arr(data.envp);
	return (0);
}

static char	*get_prompt(t_data *data)
{
	char	*str;
	char	*tmp;
	struct termios	terminos_p;

	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDOUT_FILENO, &terminos_p) == -1)
			err_exit(data, "Error: terminos", 15);
		terminos_p.c_lflag &= ~(ECHOCTL);
		if (tcsetattr(STDOUT_FILENO, TCSANOW, &terminos_p) == -1)
			err_exit(data, "Error: terminos", 15);
		tmp = readline("FRENO: ");
		str = (ft_strtrim(tmp, " \t\n"));
		free(tmp);
		terminos_p.c_lflag |= ECHOCTL;
		if (tcsetattr(STDOUT_FILENO, TCSANOW, &terminos_p) == -1)
			err_exit(data, "Error: terminos", 15);
	}
	else
	{
		str = minishell_get_next_line(STDIN_FILENO);
		int x = ft_strlen(str);
		if (str == NULL)
			return (NULL);
		if (str[x - 1] == '\n')
			str[x - 1] = '\0';
		//printf("%c___%d", str[x - 1], x);
	}
	return (str);
}

static int	lexing_and_parsing(t_data *data, char *read)
{
	if (lexing(data, read) == ERROR)
	{
		free(read);
		free_data(data);
		return (ERROR);
	}
	if (parsing(data) == ERROR)
	{
		free(read);
		free_data(data);
		return (ERROR);
	}
	return (OK);
}

static void	init_struct_and_signals(t_data *data)
{
	data->paths = NULL;
	data->pipe_segments = NULL;
	data->p_s_head = NULL;
	data->pipe_flag_a = OFF;
	data->pipe_flag_b = OFF;
	ft_memset(&data->s_ctrl_c, 0, sizeof(data->s_ctrl_c));
	ft_memset(&data->s_ctrl_back_slash, 0, sizeof(data->s_ctrl_back_slash));
	data->s_ctrl_c.sa_handler = sig_int_prompt;
	data->s_ctrl_c.sa_flags = 0;
	sigaction(SIGINT, &data->s_ctrl_c, NULL);
	data->s_ctrl_back_slash.sa_handler = SIG_IGN;
	data->s_ctrl_back_slash.sa_flags = 0;
	sigaction(SIGQUIT, &data->s_ctrl_back_slash, NULL);
}

static void	cpy_env(t_data *data, char **env)
{
	int	env_cnt;

	env_cnt = 0;
	while (env[env_cnt] != NULL)
		env_cnt++;
	data->cnt = env_cnt + 1;
	data->envp = (char **)malloc(sizeof(char *) * (env_cnt + 1));
	if (!data->envp)
		err_exit(data, "Error: cpy_env", 14);
	data->envp[env_cnt] = NULL;
	env_cnt--;
	while (env_cnt >= 0)
	{
		data->envp[env_cnt] = ft_strdup(env[env_cnt]);
		env_cnt--;
	}
}
