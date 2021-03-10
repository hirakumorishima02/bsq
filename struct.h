#ifndef FT_POINT_H
# define FT_POINT_H

typedef struct	s_point
{
    int n; //何×何か
	int x; //最大図形の右下番地
	int y; //上に同じ
    int largest; //正方形の辺
    char full; //埋まっているを指す文字
    char empty; //空を指す文字
    char obst; //障害物を指す文字
}				t_point;

#endif
