//
// Created by Marleen Maryjane on 4/9/21.
//

#include "minishell.h"

int	ft_lstsize(t_env *lst)
{
	int size;

	size = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}

int		ft_kolenvp(char **envp)
{
	int t;
	int c;

	t = -1;
	c = 0;
	while (envp[++t])
		c++;
//	printf("kolichestvo-%d\n", c);
	return (c);
}

void	ft_copyenvp(t_com *com)
{
	t_env *temp;
	int t;

	temp = com->env;
	com->envp = malloc(sizeof (char *) * (ft_lstsize(com->env) + 1));
	if (!(com && com->envp))
		ft_error(-2);
	t = -1;
//	while (envp[++t])
//		com->envp[t] = ft_strdup(envp[t]);
//	com->envp[t] = NULL;
	while (com->env)
	{
		com->envp[++t] = ft_strjoin(com->env->key, ft_strjoin("=", com->env->content));
		com->env = com->env->next;
	}
	com->envp[++t] = NULL;

	com->env = temp;
}