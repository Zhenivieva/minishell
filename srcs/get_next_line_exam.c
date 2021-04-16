#include "minishell.h"

void sigint(int num)
{
	(void)num;
//	write(1, "\b\b  \b\b", 6);
	write(1, "\nminishell$", 11);
}

void sigquit(int num)
{
	(void)num;
	write(1, "sigquit\n",8);

	if (g_p[0] == 1)
	{
		ft_putstr_fd("Quit: 3", 1);
	}
	exit(1);
//	write(1, "\b\b  \b\b", 6);
//	write(1, "\nnazhali ctrl-\\\n", 16);
}

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

char *ft_remove(char *buf, int count)
{
	int t;
	int k;
	char *ret;

	ret = malloc(sizeof(char) * ft_strlen(buf));
	if (ret == NULL)
		ft_error (-2);

	t = 0;
	k = 0;
	while (buf[t])
	{
		if (t == count) {
			t++;
			continue;
		}
		ret[k++] = buf[t++];
	}
	ret[k] = '\0';
	return (ret);
}

int get_next_line(char **line, t_com *com)
{
	char *buf;
	int res;
	char str[2000] = "";
	struct termios term;
	struct termios term1;
	char *term_name = "xterm-256color"; // на айос
//	int fd;
	int count;
	int max;
	int flag;

	t_tlist *thead;
	t_tlist *thead2;
	*line = NULL;
	buf = NULL; //buf = NULL


//	fd = open("/Users/mflor/history2.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
	tcgetattr(0, &term);
	tcgetattr(0, &term1);
	term.c_lflag &= ~(ECHO);
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ISIG);

	tcsetattr(0, TCSANOW, &term);

	tgetent(0, term_name);  // подгружает базу данных терминалa

	tputs(save_cursor, 1, ft_putchar);
	thead = com->head;
	thead2 = com->head;
	flag = 0;
	count = 0;
	max = 0;
	while (ft_strcmp(str, "\n"))
	{
		res = read(0, str, 100);
		str[res] = '\0';
//		if (ft_strlen(str) == 1)
//			write(fd, str, ft_strlen(str));
		if (!ft_strcmp(str, "\e[A"))
		{
			tputs(restore_cursor, 1, ft_putchar);
			tputs(tigetstr("ed"), 1, ft_putchar);
			if (com->head)
			{

				if (buf)
				{
					if (!(com->inited))
					{
						thead2 = com->head;
						com->head = thead;
						ft_init(&com->head, &com->tail, buf, com);
						com->head = thead2;
						com->head = com->head->prev;
					}
					else
					{
						thead2 = com->head;
						com->head = thead;
						insert_beginning(&com->head, buf);
						com->head = thead2;
						com->head = com->head->prev;
					}
					free(buf);
					buf = NULL;

				}
				if (com->head->next && flag == 1)
					com->head = com->head->next;
				*line = ft_strdup(com->head->content);
				write(1, com->head->content, ft_strlen(com->head->content));
				flag = 1;
			}

		}
		else if (!ft_strcmp(str, "\e[B"))
		{
			tputs(restore_cursor, 1, ft_putchar);
			tputs(tigetstr("ed"), 1, ft_putchar);
			if (com->head) {
				if (com->head->prev)
				{
					com->head = com->head->prev;
					write(1, com->head->content, ft_strlen(com->head->content));
					*line = ft_strdup(com->head->content);
				}
			}
		}
		else if (!ft_strcmp(str, "\177"))
		{
			if (flag) {
				buf = ft_strdup(com->head->content);
				count = ft_strlen(buf);
			}
			if (count > 0)
			{
				tputs(cursor_left, 1, ft_putchar);
				tputs(tgetstr("dc", 0), 1, ft_putchar);

				buf = ft_remove(buf, count-1);
				*line = ft_strdup(buf);
				count--;
				max--;
			}
		}
		else if (!ft_strcmp(str, "\e[D")) //to the left
		{

			if (count > 0)
			{
				tputs(cursor_left, 1, ft_putchar);
				count--;
			}
		}
		else if (!ft_strcmp(str, "\e[C")) //to the right
		{
			if (count < max)
			{
				tputs(cursor_right, 1, ft_putchar);
				count++;
			}
		}
		else if (!ft_strcmp(str, "\4"))
		{
			if (ft_strlen(buf) == 0) {
				write(1, "exit\n", 5);
				exit(0);
			}
		}
		else if (!ft_strcmp(str, "\3"))
		{
			free(*line);
			*line = NULL;
			free(buf);
			buf = NULL;
			write(1, "\nminishell$", 11);
		}
		else
		{
			write (1, str, res);
			if (ft_strcmp(str, "\n") && ft_strlen(str) == 1)
				buf = ft_strjoin_f(buf, str);
			count = ft_strlen(buf);
//			printf("count buf=%d\n", count);
//			if (flag)
//			{
//				*line = ft_strjoin_f(*line, buf);
//				count = ft_strlen(*line);
//				printf("count line=%d\n", count);
//			}
			max = count;
			if (ft_strcmp(str, "\n"))
			{
				if (flag)
					*line = ft_strjoin_f(*line, buf);
				else
					*line = ft_strdup(buf);
			}
//				*line = ft_strjoin_f(*line, buf);
		}
	}
	com->head = thead;
	free(buf);
	buf = NULL;
	if (ft_strcmp(*line, "") != -1 )
	{
		if (!(com->inited))
			ft_init(&com->head, &com->tail, *line, com);
		else
			insert_beginning(&com->head, *line);
	}
	term = term1;
//	term.c_lflag = 0x00000080;
//	term.c_lflag = 0x00000008;
//	term.c_lflag = 0x00000100;
	tcsetattr(0, TCSANOW, &term);
	return (res);
}


char	*ft_strchr(const char *s, int c)
{
	char	cc;

	cc = (char)c;
	while (*s)
	{
		if (*s == cc)
			return ((char*)s);
		s++;
	}
	if (*s == cc)
		return ((char*)s);
	return ((char*)NULL);
}

