/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmorishi <hmorishi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 06:14:51 by hmorishi          #+#    #+#             */
/*   Updated: 2021/03/09 13:35:44 by hmorishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define BUF_SIZE 512

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strncat(char *dest, char *src, unsigned int nb)
{
	unsigned int count;
	unsigned int i;

	count = 0;
	i = 0;
	while (dest[count] != '\0')
		count++;
	while (src[i] != '\0' && i < nb)
	{
		dest[count + i] = src[i];
		i++;
	}
	dest[count + i] = '\0';
	return (dest);
}

char	*malloc_and_cpy(char *str, int malloc_size)
{
	int		i;
	char	*new_str;

	new_str = 0;
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) + malloc_size + 1));
	i = -1;
	while (str[++i] != '\0')
		new_str[i] = str[i];
	free(str);
	return (new_str);
}

char	*read_file(char *file_name)
{
	int		fd;
	int		ret;
	char	buf[BUF_SIZE];
	char	*read_str;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (NULL);
	read_str = 0;
	read_str = (char *)malloc(sizeof(char));
	read_str[0] = '\0';
	ret = 1;
	while (ret)
	{
		ret = read(fd, buf, BUF_SIZE);
		read_str = malloc_and_cpy(read_str, ret);
		ft_strncat(read_str, buf, ret);
		if (ret < BUF_SIZE)
			ret = 0;
	}
	if (close(fd) == -1)
		return (NULL);
	return (read_str);
}

char	charcmp(char c, char *charset)
{
	int i;

	i = 0;
	while (charset[i] != '\0')
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

void	ft_strcpy(char *box, char *src, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		box[i] = src[i];
		i++;
	}
	box[i] = '\0';
}

char	**memory_allocate(char *str, char *charset)
{
	int i;
	int len;

	i = 0;
	len = 0;
	while (i <= ft_strlen(str))//文字列全てを検出
	{//strの文字とcharsetの文字が同じか比較||strの最後になるまで
		if (charcmp(str[i], charset) || str[i] == '\0')
		{//1つ前の文字がcharsetに当てはまらなければ  "AAxxBBxxCC";
			if (i != 0 && !charcmp(str[i - 1], charset))
				len++;//カウントする
		}
		i++;
	}//カウント分+1だけメモリを確保
	return ((char **)malloc(sizeof(char *) * (len + 1)));
}

char	**ft_split(char *str, char *charset)
{
	int		i;
	int		j;
	int		len;
	char	**box;

	box = memory_allocate(str, charset);
	i = -1;
	j = 0;
	len = 0;
	while (++i <= ft_strlen(str))
	{
		if ((charcmp(str[i], charset) || str[i] == '\0') &&
			(len > 0 && !charcmp(str[i - 1], charset)))
		{
			box[j] = (char *)malloc(len + 1);
			ft_strcpy(box[j], &str[i - len], len);
			j++;
			len = 0;
		}
		else if (!charcmp(str[i], charset))
			len++;
	}
	box[j] = 0;
	return (box);
}

int		**find_error(char **map)
{
	int error;

	error = 0;
	
	return (error);
}

int		main(int argc, char *argv[])
{
	char *file_content;
	char **map;

	file_content = read_file(argv[1]);
	map = ft_split(file_content, "\n");
	
	if (find_error(**map))
	{
		write(2, "file_content error\n", 10);
		return (0);
	}
	return (**map);
}