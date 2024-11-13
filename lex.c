/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:41:09 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:26:13 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *copy_token_value(t_token *token)
{
	char *new_str;

	new_str = ft_strdup(token->value);
	if (!new_str)
	{
		write(2, "Memory allocation failed\n", 23);
		return (NULL);
	}
	return (new_str);
}

void clean_str_array(char **arr, int limit)
{
	int idx;

	idx = 0;
	if (limit == -1)
	{
		while (arr[idx])
		{
			free(arr[idx]);
			idx++;
		}
	}
	else
	{
		while (idx < limit)
		{
			free(arr[idx]);
			idx++;
		}
	}
	free(arr);
}

int count_cmd_args(t_token *tok)
{
	int count;
	t_token *tmp;

	count = 0;
	tmp = tok;
	while (tmp && tmp->type != PIPE && tmp->type != END)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

int store_args(t_cmd *cmd, t_token **tok, int size)
{
	int i;
	char *arg;

	i = 0;
	while (i < size)
	{
		if ((*tok)->type == WORD)
			arg = copy_token_value(*tok);
		else
			arg = copy_token_value(*tok);
		if (!arg)
		{
			clean_str_array(cmd->args, i);
			return (0);
		}
		cmd->args[i] = arg;
		*tok = (*tok)->next;
		i++;
	}
	cmd->args[i] = NULL;
	return (1);
}

int extr_args(t_token **token, t_cmd *cmd)
{
	int arg_count;

	arg_count = count_cmd_args(*token);
	if (arg_count == 0)
		return (1);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		write(2, "Memory allocation error\n", 22);
		return (0);
	}
	if (!store_args(cmd, token, arg_count))
		return (0);
	return (1);
}

void display_cmd_content(t_cmd *current)
{
	int idx;

	idx = 0;
	write(1, "Command Arguments: ", 18);
	if (current->args)
	{
		while (current->args[idx])
		{
			write(1, "argument ->|--:", 14);
			write(1, current->args[idx], ft_strlen(current->args[idx]));
			write(1, ":--|\n", 5);
			idx++;
		}
	}
}

void print_commands(t_cmd *cmd)
{
	t_cmd *current;

	if (!cmd)
	{
		write(1, "error: no commands \n", 21);
		return;
	}
	current = cmd;
	while (current)
		current = current->next;
}

void display_tokens(t_token *token)
{
	char type_str[12];
	char *value;

	while (token)
	{
		write(1, "\n--------\n", 10);
		ft_itoa_buf(token->type, type_str);
		write(1, "Token Type: ", 12);
		write(1, type_str, ft_strlen(type_str));
		write(1, "\nToken Value: ", 13);
		value = token->value ? token->value : "(null)";
		write(1, value, ft_strlen(value));
		write(1, "\n", 1);
		token = token->next;
	}
}

int handle_parse_error(t_ms *ms)
{
	free_tokens(ms->token);
	return (0);
}

int parse(t_ms *ms)
{
	if (!lexing(ms))
		return (handle_parse_error(ms));
	if (syntax_error(ms))
		return (handle_parse_error(ms));
	if (!commands(ms))
		return (handle_parse_error(ms));
	expand(ms);
	if (!ft_manage_redir(ms))
		return (handle_parse_error(ms));
	free_tokens(ms->token);
	return (1);
}

int init_new_command(t_cmd **cmd)
{
	*cmd = malloc(sizeof(t_cmd));
	if (!*cmd)
	{
		write(2, "Memory allocation failed\n", 23);
		return (0);
	}
	(*cmd)->args = NULL;
	(*cmd)->next = NULL;
	(*cmd)->prev = NULL;
	(*cmd)->fd_in = STDIN_FILENO;
	(*cmd)->fd_out = STDOUT_FILENO;
	return (1);
}

int setup_new_cmd(t_cmd **prev, t_token **token, t_cmd **cmd)
{
	t_cmd *new;

	if (!init_new_command(&new))
		return (0);
	new->prev = *prev;
	if (*prev)
		(*prev)->next = new;
	if (!*cmd)
		*cmd = new;
	*prev = new;
	if ((*token)->prev && (*token)->prev->type != PIPE)
		*token = (*token)->next;
	return (1);
}

int build_cmd_list(t_token *token, t_cmd **cmds)
{
	t_cmd *cmd;

	cmd = NULL;
	while (token->type != END)
	{
		if ((!token->prev || token->prev->type == PIPE) && !setup_new_cmd(&cmd, &token, cmds))
			return (0);
		else if (token->type != PIPE && !extr_args(&token, cmd))
			return (0);
		else if (token->type == PIPE)
			token = token->next;
	}
	return (1);
}

int commands(t_ms *ms)
{
	t_cmd *result;

	result = NULL;
	if (!build_cmd_list(ms->token, &result))
		return (0);
	if (result)
		ms->cmd = result;
	return (1);
}

int get_num_len(int n)
{
	int len;

	len = 1;
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

void ft_itoa_buf(int n, char *buf)
{
	int len;
	long num;
	int i;

	if (!buf)
		return;
	num = n;
	len = get_num_len(num);
	i = len - 1;
	buf[len] = '\0';
	if (num == 0)
		buf[0] = '0';
	if (num < 0)
	{
		buf[0] = '-';
		num = -num;
	}
	while (num > 0)
	{
		buf[i] = (num % 10) + '0';
		num /= 10;
		i--;
	}
}

char *ft_fetch_env(const char *name, char **env)
{
	int name_len;
	int i;

	name_len = ft_strlen(name);
	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			return (&(env[i][name_len + 1]));
		}
		i++;
	}
	return (NULL);
}

void ft_strnjoin_helper(char *dest, const char *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

int ft_exp_concan(char *str, int *i, char **ex_str, t_ms *ms)
{
	char *expansion;
	char *temp;
	int start_pos;
	int end_pos;

	start_pos = *i;
	while (str[*i] && ft_isalnum(str[*i]))
	{
		(*i)++;
		end_pos = *i;
	}
	temp = ft_substr(str, start_pos, end_pos - start_pos);
	if (!temp)
		return (0);
	expansion = ft_fetch_env(temp, ms->env);
	if (!expansion)
	{
		free(temp);
		return (0);
	}
	*ex_str = ft_strnjoin(*ex_str, expansion, ft_strlen(expansion));
	ft_set_env(temp, expansion, 1, ms->env_s);
	free(temp);
	return (1);
}

char **ft_generate_args(char **new_args, char **args, char **splt_args,
						const int *n)
{
	int i;
	int j;
	int k;

	i = 0;
	k = 0;
	while (args[i])
	{
		if (i == *n)
		{
			j = 0;
			while (splt_args[j])
				new_args[k++] = splt_args[j++];
		}
		else
			new_args[k++] = args[i];
		i++;
	}
	new_args[k] = NULL;
	return (new_args);
}

int process_pid_exit(const char *str, int *i, char **ex_str, t_ms *ms)
{
	char *expansion;

	(*i)++;
	if (str[*i] == '$')
	{
		expansion = ft_itoa((int)getpid());
		if (!expansion)
			return (0);
		*ex_str = ft_strnjoin(*ex_str, expansion, ft_strlen(expansion));
		free(expansion);
	}
	else if (str[*i] == '?')
	{
		expansion = ft_itoa(ms->exit_code);
		if (!expansion)
			return (0);
		*ex_str = ft_strnjoin(*ex_str, expansion, ft_strlen(expansion));
		free(expansion);
	}
	return (1);
}

int process_dbl_quotes(char *str, int *i, char **ex_str, t_ms *ms)
{
	(*i)++;
	while (str[*i] && str[*i] != '\"')
	{
		if (str[*i] == '$' && str[*i + 1] && str[*i + 1] != '\"' && str[*i + 1] != ' ')
		{
			process_var_expasion_first(str, i, ex_str, ms);
			(*i)--;
		}
		else
			*ex_str = ft_strnjoin(*ex_str, &str[*i], 1);
		(*i)++;
	}
	if (str[*i])
		(*i)++;
	return (1);
}

int process_single_qutoes(char *str, int *i, char **ex_str)
{
	int start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	*ex_str = ft_strnjoin(*ex_str, &str[start], *i - start);
	if (str[*i] == '\'')
		(*i)++;
	return (1);
}

int process_var_expasion_first(char *str, int *i, char **ex_str, t_ms *ms)
{
	if (*i < (int)ft_strlen(str))
		(*i)++;
	if (str[*i] == '$' || str[*i] == '?')
		return (process_pid_exit(str, i, ex_str, ms));
	ft_exp_concan(str, i, ex_str, ms);
	return (1);
}

int process_var_expasion_second(char *str, int *i, char **ex_str, t_ms *ms, int **s_flag)
{
	**s_flag = 1;

	if (*i < (int)ft_strlen(str))
		(*i)++;
	if (str[*i] == '$' || str[*i] == '?')
		return (process_pid_exit(str, i, ex_str, ms));
	ft_exp_concan(str, i, ex_str, ms);
	return (1);
}

char **allocate_new_args(char **args, const char *temp, char **split_args,
						 int *n)
{
	int i;
	int j;
	char **new_args;

	i = 0;
	while (args[i])
		i++;
	j = 0;
	while (split_args[j])
		j++;
	new_args = malloc(sizeof(char *) * (i + j));
	if (!new_args)
		return (NULL);
	new_args = ft_generate_args(new_args, args, split_args, n);
	return ((void)temp, new_args);
}

void subtitute_free_args(char ***args, int *n, int *s_flag)
{
	char *temp;
	char **split_args;
	char **new_args;

	*s_flag = 0;
	temp = (*args)[*n];
	split_args = ft_split(temp, ' ');
	if (!split_args)
		return;

	new_args = allocate_new_args(*args, temp, split_args, n);
	if (!new_args)
	{
		free(split_args);
		return;
	}

	free(temp);
	free(*args);
	free(split_args);
	*args = new_args;
}

char *str_expander(char *expanded_str, char *str, int *s_flag, t_ms *ms, int len)
{
	int i;

	i = -1;
	while (++i < len)
	{
		if (str[i] == '\'' || str[i] == '\"' || str[i] == '$')
		{
			if (str[i] == '\'')
			{
				process_single_qutoes(str, &i, &expanded_str);
				continue;
			}
			else if (str[i] == '\"')
			{
				process_dbl_quotes(str, &i, &expanded_str, ms);
				continue;
			}
			else if (str[i] == '$')
				process_var_expasion_second(str, &i, &expanded_str, ms, &s_flag);
		}
		else
			expanded_str = ft_strnjoin(expanded_str, &str[i], 1);
	}
	return (expanded_str);
}

int ft_valid_expand(char **s, t_ms *ms, int *s_flag)
{
	char *expanded_str;
	char *str;

	str = *s;
	expanded_str = NULL;
	expanded_str = str_expander(expanded_str, str, s_flag, ms, ft_strlen(str));
	if (expanded_str)
	{
		free(*s);
		*s = expanded_str;
		return (1);
	}
	return (0);
}

int ft_strcmp(const char *s1, const char *s2)
{
	if ((*s1 != *s2) || (*s1 == '\0') || (*s2 == '\0'))
		return ((unsigned char)*s1 - (unsigned char)*s2);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

void heredoc_all(int fd, char *del, t_ms *mini)
{
	char *line;

	while (1)
	{
		line = readline(">");
		if (!ft_strcmp(line, del))
		{
			free(line);
			break;
		}
		if (ft_strchr(line, '$'))
		{
			ft_valid_expand(&line, mini, 0);
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

void ft_del_array(char ***array_ptr, int index)
{
	char **array;
	int size;
	int i;

	i = index;
	array = *array_ptr;
	size = 0;
	while (array[size] != NULL)
		size++;
	free(array[index]);
	while (i < size - 1)
	{
		array[i] = array[i + 1];
		i++;
	}
	array[size - 1] = NULL;
}

void remove_cmd_node(t_ms *mini, t_cmd *node_to_remove)
{
	int i;

	if (mini == NULL || node_to_remove == NULL)
		return;
	if (node_to_remove == mini->cmd)
		mini->cmd = node_to_remove->next;
	if (node_to_remove->prev != NULL)
		node_to_remove->prev->next = node_to_remove->next;
	if (node_to_remove->next != NULL)
		node_to_remove->next->prev = node_to_remove->prev;
	if (node_to_remove->fd_in != STDIN_FILENO)
		close(node_to_remove->fd_in);
	if (node_to_remove->fd_out != STDOUT_FILENO)
		close(node_to_remove->fd_out);
	if (node_to_remove->args != NULL)
	{
		i = 0;
		while (node_to_remove->args[i])
		{
			free(node_to_remove->args[i]);
			i++;
		}
		free(node_to_remove->args);
	}
	free(node_to_remove);
}

t_lex *lexer_init(t_ms *ms)
{
	t_lex *lexer;

	lexer = (t_lex *)malloc(sizeof(t_lex));
	if (!lexer)
		return (NULL);
	lexer->input = ms->input;
	lexer->dq = 0;
	lexer->sq = 0;
	lexer->pos = 0;
	return (lexer);
}

t_token *get_token(t_lex *lexer)
{
	int input_len;
	char current_char;

	input_len = ft_strlen(lexer->input);
	while (lexer->pos < input_len)
	{
		current_char = lexer->input[lexer->pos];
		if (ft_isspace(current_char))
			lexer->pos++;
		else if (current_char == '|')
			return (ft_create_pipe_t(lexer));
		else if (current_char == '<')
			return (ft_input_token(lexer));
		else if (current_char == '>')
			return (ft_output_token(lexer));
		else if ((current_char == '\'' || current_char == '\"') &&
				 lexer->input[lexer->pos + 1] == current_char)
			return (lexer->pos++, lexer->pos++, cr_empty_q_t());
		else
			return (ft_word_token(lexer));
	}
	return (ft_endof_t());
}

int lexing(t_ms *ms)
{
	t_lex *lexer;
	t_token *token;

	lexer = lexer_init(ms);
	if (!lexer || !lexer->input)
		return (0);
	ms->token = get_token(lexer);
	if (!ms->token)
		return (0);
	ms->token->prev = NULL;
	token = ms->token;
	while (token->type != END)
	{
		token->next = get_token(lexer);
		if (!token->next || token->type == ERR)
		{
			ms->exit_code = 258;
			ft_putstr_fd("Error! unclosed quotes\n", 2);
			return (free(lexer), 0);
		}
		token->next->prev = token;
		token = token->next;
	}
	return (free(lexer), 1);
}

void free_tokens(t_token *token)
{
	t_token *temp;

	while (token)
	{
		temp = token->next;
		free(token->value);
		free(token);
		token = temp;
	}
}

int ft_valid_file(char *path)
{
	struct stat file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": is a directory\n", 2);
			exit(126);
		}
	}
	return (1);
}

void set_fd_helper(char *re, char *path, t_cmd *cmd)
{
	if ((!ft_strcmp(re, ">>") || !ft_strcmp(re, ">")) &&
		cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	if (!ft_strcmp(re, ">"))
	{
		cmd->fd_out = open(path, O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	if (!ft_strcmp(re, ">>"))
	{
		cmd->fd_out = open(path, O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	if ((!ft_strcmp(re, "<<") || !ft_strcmp(re, "<")) &&
		cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (!ft_strcmp(re, "<"))
		cmd->fd_in = open(path, O_RDONLY);
}

int set_fd(char *re, char *path, t_cmd *cmd, t_ms *mini)
{
	set_fd_helper(re, path, cmd);
	if (!ft_strcmp(re, "<<"))
	{
		cmd->fd_in = open(".heredoc_all", O_CREAT | O_RDWR | O_TRUNC, 0644);
		heredoc_all(cmd->fd_in, path, mini);
		close(cmd->fd_in);
		cmd->fd_in = open(".heredoc_all", O_RDONLY);
	}
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
	{
		perror("minishell");
		return (0);
	}
	return (1);
}

void handle_directory(char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": is a directory\n", 2);
}

int file_exist(t_ms *mini, char *path)
{
	struct stat file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if (!S_ISDIR(file_stat.st_mode))
		{
			if (access(path, R_OK) == -1)
			{
				mini->exit_code = 126;
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(path, 2);
				ft_putstr_fd(": Permission denied \n", 2);
				return (1);
			}
		}
		else
		{
			mini->exit_code = 126;
			handle_directory(path);
			return (1);
		}
	}
	return (0);
}

int ft_manage_redir_out(t_ms *mini, t_cmd *cmd, int *i)
{
	if (!cmd->args[*i + 1])
		return (1);
	if (!set_fd(cmd->args[*i], cmd->args[(*i) + 1], cmd, mini))
	{
		mini->exit_code = 1;
		return (0);
	}
	ft_del_array(&cmd->args, *i);
	ft_del_array(&cmd->args, *i);
	(*i)--;
	return (1);
}

int ft_manage_redir_in(t_ms *mini, t_cmd *cmd, int *i)
{
	if (!cmd->args[*i + 1])
		return (1);
	if ((!ft_strcmp(cmd->args[*i], "<") || (!ft_strcmp(cmd->args[*i], "<<"))) &&
		!ft_valid_file(cmd->args[(*i) + 1]))
	{
		mini->exit_code = 1;
		return (0);
	}
	if (!set_fd(cmd->args[*i], cmd->args[(*i) + 1], cmd, mini))
	{
		mini->exit_code = 1;
		return (0);
	}
	ft_del_array(&cmd->args, *i);
	ft_del_array(&cmd->args, *i);
	(*i)--;
	return (1);
}

void remove_args(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	cmd->args = NULL;
}

int ft_manage_redir_helper(t_ms *mini, t_cmd *cmd, int *i)
{
	if (cmd->args[*i][0] == '<')
	{
		if (!ft_manage_redir_in(mini, cmd, i))
			return (remove_args(cmd), 0);
	}
	else if (cmd->args[*i][0] == '>')
	{
		if (!ft_manage_redir_out(mini, cmd, i))
			return (remove_args(cmd), 0);
	}
	return (1);
}

int ft_manage_redir(t_ms *mini)
{
	t_cmd *temp;
	int i;

	temp = mini->cmd;
	while (mini->cmd)
	{
		i = 0;
		while (mini->cmd->args[i])
		{
			if (!ft_manage_redir_helper(mini, mini->cmd, &i))
			{
				if (!mini->cmd->next)
					return (0);
				break;
			}
			i++;
		}
		mini->cmd = mini->cmd->next;
	}
	mini->cmd = temp;
	return (1);
}

t_env *find_last(t_env *env_s)
{
	if (NULL == env_s)
		return (NULL);
	while (env_s->next)
		env_s = env_s->next;
	return (env_s);
}

void free_struct(t_env **stack)
{
	t_env *tmp;
	t_env *current;

	current = NULL;
	if (!stack)
		return;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	*stack = NULL;
}

void ft_set_node(t_env **env_s, char *token, char *type)
{
	t_env *node;
	t_env *last_node;

	if (NULL == env_s)
		return;
	node = malloc(sizeof(t_env));
	if (NULL == node)
		return (free_struct(env_s), (void)NULL);
	node->next = NULL;
	node->env_name = ft_strdup(token);
	free(token);
	if (type)
	{
		node->env_value = ft_strdup(type);
		free(type);
	}
	if (NULL == *env_s)
		*env_s = node;
	else
	{
		last_node = find_last(*env_s);
		last_node->next = node;
	}
}

t_env *ft_save_env(char **env, t_env *env_struct)
{
	char **env_split;
	int i;

	i = 0;
	while (env[i])
	{
		env_split = ft_split(env[i], '=');
		if (env_split[0] && !env_split[1])
			env_split[1] = ft_strdup("");
		ft_set_node(&env_struct, env_split[0], env_split[1]);
		free(env_split);
		i++;
	}
	return (env_struct);
}

int redir_error_check(t_token *token)
{
	if (!token->next || token->next->type != WORD)
	{
		ft_putstr_fd("error: wrong token \n", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("'\n", 2);
		return (1);
	}
	return (0);
}

int pipe_error_check(t_token *token)
{
	if (token->next == NULL || token->next->type == END || token->prev == NULL ||
		token->prev->type != WORD)
		return (ft_putstr_fd("error: wrong token \n", 2), 1);
	return (0);
}

int syntax_error(t_ms *ms)
{
	t_token *token;

	token = ms->token;
	while (token)
	{
		if ((token->type == PIPE && pipe_error_check(token)) ||
			((token->type == IN || token->type == H_DOC || token->type == OUT ||
			  token->type == APPEND) &&
			 redir_error_check(token)))
		{
			ms->exit_code = 258;
			return 1;
		}
		token = token->next;
	}
	return 0;
}

t_token *ft_endof_t(void)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = END;
	token->value = NULL;
	token->next = NULL;
	return (token);
}

t_token *ft_ext_word_val(int start, t_lex *lex)
{
	t_token *token;
	char *value;

	value = ft_substr(lex->input, start, lex->pos - start);
	token = (t_token *)malloc(sizeof(t_token));
	if (!value || !token)
		return (free(value), NULL);
	token->type = WORD;
	token->value = value;
	return (token);
}

void ext_quote_val(t_lex *l, char q)
{
	if (q == '\'' && !l->sq && !l->dq)
		l->sq = 1;
	else if (q == '\'' && l->sq && !l->dq && (l->input[l->pos] == '\''))
		l->sq = 0;
	else if (q == '\"' && !l->sq && !l->dq)
		l->dq = 1;
	else if (q == '\"' && !l->sq && l->dq && (l->input[l->pos] == '\"'))
		l->dq = 0;
}

t_token *open_quote_val(void)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = ERR;
	token->value = ft_strdup("Unclosed quote");
	token->next = NULL;
	return (token);
}

t_token *ft_create_pipe_t(t_lex *lexer)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = PIPE;
	token->value = ft_strdup("|");
	return (lexer->pos++, token);
}

t_token *ft_input_token(t_lex *lexer)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (lexer->input[lexer->pos + 1] == '<')
	{
		token->type = H_DOC;
		token->value = ft_strdup("<<");
		return (lexer->pos += 2, token);
	}
	else
	{
		token->type = IN;
		token->value = ft_strdup("<");
		return (lexer->pos++, token);
	}
}

t_token *ft_output_token(t_lex *lexer)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (lexer->input[lexer->pos + 1] == '>')
	{
		token->type = APPEND;
		token->value = ft_strdup(">>");
		return (lexer->pos += 2, token);
	}
	else
	{
		token->type = OUT;
		token->value = ft_strdup(">");
		return (lexer->pos++, token);
	}
}

t_token *cr_empty_q_t(void)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = WORD;
	token->value = ft_strdup("");
	return (token);
}

t_token *ft_word_token(t_lex *lex)
{
	int start;

	start = lex->pos;
	while (lex->input[lex->pos])
	{
		if (lex->input[lex->pos] == '\'' || lex->input[lex->pos] == '\"')
			ext_quote_val(lex, lex->input[lex->pos]);
		if ((ft_isspace(lex->input[lex->pos]) || lex->input[lex->pos] == '|' ||
			 lex->input[lex->pos] == '<' || lex->input[lex->pos] == '>') &&
			!(lex->dq || lex->sq))
			break;
		lex->pos++;
	}
	if ((lex->dq || lex->sq))
		return (open_quote_val());
	return (ft_ext_word_val(start, lex));
}

char *clean_quotes(char *str)
{
	char *cleaned;
	int i = 0, j = 0;
	int in_squote = 0, in_dquote = 0;

	if (!str)
		return NULL;
	cleaned = malloc(strlen(str) + 1);
	if (!cleaned)
	{
		free(str);
		return NULL;
	}
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else
			cleaned[j++] = str[i];
		i++;
	}
	cleaned[j] = '\0';
	free(str);
	return cleaned;
}

char *ft_strnjoin(char *s1, const char *s2, size_t n)
{
	char *str;
	size_t len1;
	size_t i;

	i = -1;
	if (!s2)
		return (s1);
	len1 = s1 ? ft_strlen(s1) : 0;
	if (n > ft_strlen(s2))
		n = ft_strlen(s2);
	str = (char *)malloc((len1 + n + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (++i < len1)
		str[i] = s1[i];
	i = -1;
	while (++i < n)
		str[len1 + i] = s2[i];
	str[len1 + i] = '\0';
	if (s1)
		free(s1);
	return (str);
}

t_env_var *create_env_var(const char *name, const char *value)
{
	t_env_var *var;

	var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!var)
		return (NULL);

	var->name = ft_strdup(name);
	var->value = ft_strdup(value);
	if (!var->name || !var->value)
	{
		free(var->name);
		free(var->value);
		free(var);
		return (NULL);
	}
	var->name_len = ft_strlen(name);
	var->value_len = ft_strlen(value);
	return (var);
}

void destroy_env_var(t_env_var *var)
{
	if (!var)
		return;
	free(var->name);
	free(var->value);
	free(var);
}

t_expansion *init_expansion(void)
{
	t_expansion *exp;

	exp = (t_expansion *)malloc(sizeof(t_expansion));
	if (!exp)
		return (NULL);

	exp->result = NULL;
	exp->pos = 0;
	exp->len = 0;
	exp->in_squote = 0;
	exp->in_dquote = 0;
	exp->last_exit_code = 0;
	exp->shell_pid = getpid();
	return (exp);
}

void destroy_expansion(t_expansion *exp)
{
	if (!exp)
		return;
	free(exp->result);
	free(exp);
}

int handle_quote(char *str, t_expansion *exp, char quote_type)
{
	size_t start;

	exp->pos++;
	start = exp->pos;
	if (quote_type == '\'')
		exp->in_squote = 1;
	else
		exp->in_dquote = 1;
	while (str[exp->pos] && str[exp->pos] != quote_type)
		exp->pos++;
	if (!str[exp->pos])
		return (0);
	return (handle_quote_cont(str, exp, quote_type, start));
}

int handle_quote_cont(char *str, t_expansion *exp, char quote_type, size_t start)
{
	char *expanded;

	if (quote_type == '\'')
	{
		exp->result = ft_strnjoin(exp->result, &str[start], exp->pos - start);
		exp->in_squote = 0;
	}
	else
	{
		char *quoted = ft_substr(str, start, exp->pos - start);
		if (!quoted)
			return (0);
		expanded = expand_variables(quoted, exp);
		free(quoted);
		if (!expanded)
			return (0);
		exp->result = ft_strnjoin(exp->result, expanded, ft_strlen(expanded));
		free(expanded);
		exp->in_dquote = 0;
	}
	exp->pos++;
	return (1);
}

char **ft_realloc_array(char **ptr, size_t new_size)
{
	char **new_ptr;
	size_t i;

	i = -1;
	if (!ptr)
		return ((char **)malloc(new_size));
	new_ptr = (char **)malloc(new_size);
	if (!new_ptr)
		return (NULL);
	while (ptr[i] && ++i < (new_size / sizeof(char *) - 1))
	{
		new_ptr[i] = ft_strdup(ptr[i]);
		if (!new_ptr[i])
		{
			while (i > 0)
				free(new_ptr[--i]);
			free(new_ptr);
			return (NULL);
		}
	}
	// new_ptr[i] = NULL;
	i = 0;
	while (ptr[i])
		free(ptr[i++]);
	free(ptr);
	return (new_ptr);
}

int handle_split(t_cmd *cmd, char **split, int i, int original_count)
{
	int split_count;
	char **new_args;
	int j;
	int shift;
	size_t new_size;

	split_count = 0;
	while (split[split_count])
		split_count++;
	new_size = sizeof(char *) * (original_count + split_count + 1);
	new_args = ft_realloc_array(cmd->args, new_size);
	if (!new_args)
		return (0);
	cmd->args = new_args;
	free(cmd->args[i]);
	cmd->args[i] = ft_strdup(split[0]);
	if (split_count > 1)
		handle_split_cont(&cmd, split, &i, &original_count, &split_count);
	return (split_count);
}

void	handle_split_cont(t_cmd **cmd, char **split, int *i, int *original_count, int *split_count)
{
	int shift;
	int	j;

	j = (*original_count) - 1;
	shift = (*split_count) - 1;
	while (j > (*i))
	{
		(*cmd)->args[j + shift] = (*cmd)->args[j];
		j--;
	}
	j = 1;
	while (j < (*split_count))
	{
		(*cmd)->args[(*i) + j] = ft_strdup(split[j]);
		j++;
	}
	(*cmd)->args[(*original_count) + shift] = NULL;
}

int handle_expanded(t_cmd *cmd, char *expanded, int i, t_expansion *exp)
{
	char **split;
	int original_count;
	int split_count;
	int j;

	j = -1;
	if (ft_strchr(expanded, ' ') && !exp->in_squote && !exp->in_dquote)
	{
		split = split_expanded(expanded);
		free(expanded);
		if (!split)
			return (-1);
		original_count = 0;
		while (cmd->args[original_count])
			original_count++;
		split_count = handle_split(cmd, split, i, original_count);
		while (split[++j])
			free(split[j]);
		free(split);
		if (!split_count)
			return (-1);
		return (split_count - 1);
	}
	handle_explanded_sec(&cmd, &expanded, &i, &exp);
	return (0);
}
void handle_explanded_sec(t_cmd **cmd, char **expanded, int *i, t_expansion **exp)
{
	free((*cmd)->args[*i]);
	(*cmd)->args[*i] = (*expanded);
}

void reset_expansion(t_expansion *exp)
{
	exp->result = NULL;
	exp->pos = 0;
	exp->in_squote = 0;
	exp->in_dquote = 0;
}

int expand_args(t_cmd *cmd, t_expansion *exp)
{
	int i;
	char *expanded;
	int split_result;

	i = 0;
	while (cmd->args[i])
	{
		reset_expansion(exp);
		expanded = expand_variables(cmd->args[i], exp);
		if (!expanded)
			return (0);
		split_result = handle_expanded(cmd, expanded, i, exp);
		if (split_result == -1)
			return (0);
		i += split_result + 1;
	}
	return (1);
}

int expand(t_ms *ms)
{
	t_cmd *cmd;
	t_expansion *exp;
	int result;

	if (!ms || !ms->cmd)
		return (0);
	exp = init_expansion();
	if (!exp)
		return (0);
	exp->last_exit_code = ms->exit_code;
	cmd = ms->cmd;
	result = 1;
	while (cmd && result)
	{
		if (!cmd->args)
		{
			cmd = cmd->next;
			continue;
		}
		result = expand_args(cmd, exp);
		cmd = cmd->next;
	}
	destroy_expansion(exp);
	return (result);
}

int handle_split_args(char **split, t_cmd *cmd, int i, int org_count)
{
	int split_count;
	char **new_args;
	int j;
	int shift;
	size_t new_size;

	split_count = 0;
	while (split[split_count])
		split_count++;
	new_size = sizeof(char *) * (org_count + split_count + 1);
	new_args = ft_realloc_array(cmd->args, new_size);
	if (!new_args)
		return (0);
	free(new_args[i]);
	new_args[i] = ft_strdup(split[0]);
	j = org_count;
	shift = split_count - 1;
	while (j > i + 1)
	{
		new_args[j + shift - 1] = new_args[j - 1];
		j--;
	}
	cmd->args = new_args;
	return (split_count);
}

int copy_split_words(char **split, char **args, int i)
{
	int j;
	int split_count;

	split_count = 0;
	while (split[split_count])
		split_count++;
	j = 1;
	while (j < split_count)
	{
		args[i + j] = ft_strdup(split[j]);
		j++;
	}
	args[i + split_count] = NULL;
	j = 0;
	while (split[j])
	{
		free(split[j]);
		j++;
	}
	free(split);
	return (split_count - 1);
}

int handle_expanded_arg(char *expanded, t_cmd *cmd, int i, t_expansion *exp)
{
	char **split;
	int org_count;
	int split_count;

	if (ft_strchr(expanded, ' ') && !exp->in_squote && !exp->in_dquote)
	{
		split = split_expanded(expanded);
		free(expanded);
		if (!split)
			return (0);
		org_count = 0;
		while (cmd->args[org_count])
			org_count++;
		split_count = handle_split_args(split, cmd, i, org_count);
		if (!split_count)
			return (0);
		return (copy_split_words(split, cmd->args, i));
	}
	free(cmd->args[i]);
	cmd->args[i] = expanded;
	return (0);
}

void ft_eval_args(t_cmd *cmd, t_ms *ms)
{
	int i;
	char *expanded;
	t_expansion *exp;
	int split_count;

	if (!cmd || !cmd->args)
		return;
	exp = init_expansion();
	if (!exp)
		return;
	exp->last_exit_code = ms->exit_code;
	i = 0;
	while (cmd->args[i])
	{
		expanded = expand_variables(cmd->args[i], exp);
		if (expanded)
		{
			split_count = handle_expanded_arg(expanded, cmd, i, exp);
			i += split_count;
		}
		i++;
	}
	destroy_expansion(exp);
}

int count_words(char *str)
{
	int word_count;
	int i;

	word_count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i])
			word_count++;
		while (str[i] && !ft_isspace(str[i]))
			i++;
	}
	return (word_count);
}

char **clean_split_fail(char **words, int word_count)
{
	while (--word_count >= 0)
		free(words[word_count]);
	free(words);
	return (NULL);
}

int fill_word(char **words, char *str, int *i, int word_count)
{
	int start;

	start = *i;
	while (str[*i] && !ft_isspace(str[*i]))
		(*i)++;
	words[word_count] = ft_substr(str, start, *i - start);
	if (!words[word_count])
		return (0);
	return (1);
}

char **split_expanded_cont(char *str, char **words)
{
	int i;
	int word_count;

	i = 0;
	word_count = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (!str[i])
			break;
		if (!fill_word(words, str, &i, word_count))
			return (clean_split_fail(words, word_count));
		word_count++;
	}
	words[word_count] = NULL;
	return (words);
}

char **split_expanded(char *str)
{
	char **words;
	int word_count;

	word_count = count_words(str);
	words = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	return (split_expanded_cont(str, words));
}

char *handle_exit_code(t_expansion *exp, char **result)
{
	char *exit_str;
	char *temp;

	exit_str = ft_itoa(exp->last_exit_code);
	if (!exit_str)
		return (NULL);
	temp = ft_strnjoin(*result, exit_str, ft_strlen(exit_str));
	free(exit_str);
	if (!temp)
		return (NULL);
	*result = temp;
	return (*result);
}

char *handle_pid(t_expansion *exp, char **result)
{
	char *pid_str;
	char *temp;

	pid_str = ft_itoa(exp->shell_pid);
	if (!pid_str)
		return (NULL);
	temp = ft_strnjoin(*result, pid_str, ft_strlen(pid_str));
	free(pid_str);
	if (!temp)
		return (NULL);
	*result = temp;
	return (*result);
}

char *handle_env_var(char *str, size_t *i, char **result)
{
	char *var_name;
	char *var_value;
	char *temp;
	size_t start;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	if (!var_name)
		return (NULL);
	var_value = ft_fetch_env(var_name, NULL);
	if (var_value)
	{
		temp = ft_strnjoin(*result, var_value, ft_strlen(var_value));
		if (!temp)
			return (NULL);
		*result = temp;
	}
	free(var_name);
	return (*result);
}

char *handle_dollar(char *str, size_t *i, t_expansion *exp, char **result)
{
	(*i)++;
	if (str[*i] == '?')
	{
		if (!handle_exit_code(exp, result))
			return (NULL);
		(*i)++;
	}
	else if (str[*i] == '$')
	{
		if (!handle_pid(exp, result))
			return (NULL);
		(*i)++;
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		if (!handle_env_var(str, i, result))
			return (NULL);
	}
	return (*result);
}

char *handle_single_quotes(char *str, size_t *i, char *result, t_expansion *exp)
{
	char *temp;

	exp->in_squote = !exp->in_squote;
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		temp = ft_strnjoin(result, &str[*i], 1);
		if (!temp)
			return (NULL);
		result = temp;
		(*i)++;
	}
	if (str[*i] == '\'')
	{
		exp->in_squote = !exp->in_squote;
		(*i)++;
	}
	return (result);
}

char *handle_double_quotes(char *str, size_t *i, char *result, t_expansion *exp)
{
	char *temp;

	exp->in_dquote = !exp->in_dquote;
	(*i)++;
	while (str[*i] && str[*i] != '\"')
	{
		if (str[*i] == '$' && str[*i + 1])
		{
			result = handle_dollar(str, i, exp, &result);
			if (!result)
				return (NULL);
		}
		else
		{
			temp = ft_strnjoin(result, &str[*i], 1);
			if (!temp)
				return (NULL);
			result = temp;
			(*i)++;
		}
	}
	if (str[*i] == '\"')
		handle_double_q_cont(str, i, result, exp);
	return (result);
}

void	handle_double_q_cont(char *str, size_t *i, char *result, t_expansion *exp)
{
	exp->in_dquote = !exp->in_dquote;
	(*i)++;
}

char *handle_regular_char(char *str, size_t *i, char *result)
{
	char *temp;

	temp = ft_strnjoin(result, &str[*i], 1);
	if (!temp)
		return (NULL);
	(*i)++;
	return (temp);
}

char *expand_variables(char *str, t_expansion *exp)
{
	char *result;
	size_t i;

	result = NULL;
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'')
			result = handle_single_quotes(str, &i, result, exp);
		else if (str[i] == '\"')
			result = handle_double_quotes(str, &i, result, exp);
		else if (str[i] == '$' && str[i + 1])
			result = handle_dollar(str, &i, exp, &result);
		else
			result = handle_regular_char(str, &i, result);

		if (!result)
			return (NULL);
	}
	return (result);
}
