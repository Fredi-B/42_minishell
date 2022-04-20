#include "minishell.h"

//for testing only: prints all cmds in all pipe_segments
void	print_cmd_arr(t_data *data)
{
	int i;
	int j;

	j = 0;
	data->pipe_segments = data->p_s_head;
	while (data->pipe_segments != NULL)
	{
		i = 0;
		while (data->pipe_segments->cmd_arr[i] != NULL)
		{
			dsprintf(data->pipe_segments->cmd_arr[i]);
			printf("segment nr.: %i, arr nr.: %i\n", j, i);
			i++;
		}
		data->pipe_segments = data->pipe_segments->next;
		j++;
	}
	data->pipe_segments = data->p_s_head;
}

// for testing only: prints all tokens in all pipe_segments
void	print_token(t_data *data)
{
	data->pipe_segments = data->p_s_head;
	while (data->pipe_segments != NULL)
	{
	data->pipe_segments->tokens = data->pipe_segments->t_head;
		while (data->pipe_segments->tokens != NULL)
		{
			dsprintf(data->pipe_segments->tokens->token);
			data->pipe_segments->tokens = data->pipe_segments->tokens->next;
		}
		data->pipe_segments = data->pipe_segments->next;
	}
}

// for testing only: prints all ins_and_outs in all pipe_segments
void	print_ins_and_outs(t_data *data)
{
	data->pipe_segments->ins_and_outs = data->pipe_segments->i_o_head;
	while (data->pipe_segments->ins_and_outs != NULL)
	{
		dsprintf(data->pipe_segments->ins_and_outs->name);
		data->pipe_segments->ins_and_outs = data->pipe_segments->ins_and_outs->next;
	}	
}