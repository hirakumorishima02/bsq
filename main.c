/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmorishi <hmorishi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 06:14:51 by hmorishi          #+#    #+#             */
/*   Updated: 2021/03/10 12:18:30 by hmorishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "algorism.c"

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

int		two_find_error1(int error, char **map, int col_num)
{
	int i;

	i = 0;
	while (map[0][i] != 'x')//1行目が四文字以下・最初の行に文字がない・最初の行の形を検証
		i++;
	if (map[0][0] < 49 && map[0][0] > 57)//2桁だった場合の対応はまだ
		error++;
	if (map[0][i - 2] <= 32 && map[0][i - 2] > 126)
		error++;
	if (map[0][i - 1] <= 32 && map[0][i - 1] > 126)
		error++;
	if (map[0][i] <= 32 && map[0][i] > 126)
		error++;
	return (error);
}

int		two_find_error2(int error, char **map)//マップは’"empty"文字’、行、および’"obstacle"文字’で構成
{
	int i;
	int j;
	int cnt;
	int tmp;

	i = 1;
	j = 0;
	cnt = 0;
	while (map[0][i++] != '\0')
		cnt++;
	i = 1;
	while (i <= (map[0][0] - 48))
	{
		while (map[i][j] != '\0')
		{
			// if (map[i][j] != map[0][cnt] && map[i][j] != map[0][cnt - 1])//条件式？？？
			// 	error++;
			j++;
		}
		if (i > 2)
		{
			if (j != tmp)
				error++;
		}
		i++;
		tmp = j;
		j = 0;
	}
	return (error);
}

int		error_two_dimension(char **map, int col_num)
{
	int error;

	error = 0;
	error = two_find_error1(error, map, col_num);
	error = two_find_error2(error, map);
	return (error);
}

int		find_error(char *file_content, int col_num)//1.oxかつ行が1行の時にsegment faultになる
{
	int error;
	int i;
	int cnt;

	error = 0;
	i = 0;
	cnt = -1;
	while (file_content[i] != '\0')
	{
		if (file_content[i] == '\n')
			cnt++;
		i++;
	}
	if (cnt != col_num)
		error++;
	if (cnt < 1)
		error++;
	return (error);
}

int		ft_atoi(char *str)
{
	int i;
	int sign_flag;
	int result;

	i = 0;
	sign_flag = 0;
	result = 0;
	while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == '\n') ||
			(str[i] == '\v') || (str[i] == '\f') || (str[i] == '\r'))
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign_flag++;
		i++;
	}
	sign_flag = sign_flag % 2 == 0 ? 1 : (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10;
		result += str[i] - '0';
		i++;
	}
	return (result * sign_flag);
}

int		get_col_num(char *file_content)
{
	int		i;
	int		j;
	char	str[10];

	i = 0;
	while (*file_content >= 48 && *file_content <= 57)
	{
		str[i] = *file_content;
		i++;
		file_content++;
	}
	str[i] = '\0';
	return (ft_atoi(str));
}

int		get_length(char *file_content)
{
	int i;
	int cnt;

	i = 0;
	while (file_content[i] != '\n')
		i++;
	cnt = 0;
	i += 1;
	while (file_content[i] != '\n')
	{
		cnt++;
		i++;
	}
	return (cnt);
}

int		main(int argc, char *argv[])
{
	char	*file_content;
	char	**map;
	int		col_num;
	int		length;

	file_content = read_file(argv[1]);
	col_num = get_col_num(file_content);
	length = get_length(file_content);
	if (find_error(file_content, col_num))
	{
		write(2, "map error\n", 10);
		return (0);
	}
	map = ft_split(file_content, "\n");
	if (error_two_dimension(map, col_num))
	{
		write(2, "map error\n", 10);
		return (0);
	}
	receive_array(map);
	return (0);
}
