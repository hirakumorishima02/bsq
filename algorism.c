/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorism.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmorishi <hmorishi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 15:57:07 by koshibat          #+#    #+#             */
/*   Updated: 2021/03/10 12:19:39 by hmorishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "struct.h"

int col_num = 9;
int length = 18;

int **charmap_to_int(char **p, char block)
{
    int **q;
    int i;
    int j;

    q = (int**)malloc(sizeof(int*) * col_num);
    i = 0;
    while (i < col_num)
    {
        q[i] = (int*)malloc(sizeof(int) * length);
        i++;
    }
    i = 0;
    while (i < col_num)
    {
        j = 0;
        while (j < length)
        {
            q[i][j] = (p[i][j] == block) ? 0 : 1;
            j++;
        }
        i++;
    }
    return (q);
}

int min(int a, int b)
{
    if (a < b)
        return (a);
    return (b);
}

void    get_largest_square(int **q,t_point *t)
{
    int i;
    int j;
    int keep[9];

    keep[2] = 0;
    i = 0;
    while (++i < col_num)
    {
        j = 0;
        while (++j < length)
        {
            if (q[i][j] != 0)
                q[i][j] = min(q[i - 1][j - 1], min(q[i][j - 1], q[i - 1][j])) + 1;
            if (keep[2] < q[i][j])
            {
                keep[0] = i;
                keep[1] = j;
                keep[2] = q[i][j];
            }
        }
    }
    t->x = keep[0];
    t->y = keep[1];
    t->largest = keep[2];
}

char **ft_copy(char **p, char **q)
{
    int i;
    int j;

    i = 0;
    while (i < col_num)
    {
        j = 0;
        while (j < length)
        {
            q[i][j] = p[i][j];
            j++;
        }
        i++;
    }
    return (q);
}

char    **write_x(char **p, char full, t_point *t)
{
    char **q;
    int i;
    int j;

    q = (char**)malloc(sizeof(char*) * col_num);
    i = -1;
    while (++i < col_num)
        q[i] = (char*)malloc(sizeof(char) * length);
    ft_copy(p, q);
    i = t->x - t->largest;
    while (++i <= t->x)
    {
        j = t->y - t->largest;
        while (++j <= t->y)
            q[i][j] = full;
    }
    return (q);
}

#include <stdio.h>

void receive_array(char **map)
{
    int i;
    int j;
    char **str;
    char **q = str;
    int **ans;
    int *point;
    int answer[col_num];
    t_point t;
    char **chaos;

    i = 0;
    j = 1;
    while (j < col_num + 1)
    {
        str[i++] = map[j++]; 
    }
    ans = charmap_to_int(q, 'o');
    get_largest_square(ans, &t);
    chaos = write_x(q, 'x', &t);
    i = 0;
    while(i < col_num)
        printf("%s\n", chaos[i++]);
}
