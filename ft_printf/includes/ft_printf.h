/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poatmeal <poatmeal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 16:36:37 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/30 14:23:01 by poatmeal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# define FLAGS "#0-+ "
# define SPECIFIER "cspdiouUxXf%Z"
# define NEEDED "#0-+ cspdiouUxXf%Z.123456789lhLjz*"
# define BIG_BUFF 60000

typedef struct			s_printf
{
	const char			*cpy;
	char				fh;
	char				fz;
	char				fm;
	char				fp;
	char				fs;
	long int			width;
	long int			precis;
	int					f_prec;
	char				modln[4];
	char				convs;
	int					sign;
	int					lfloat;
	int					star;
	int					len;
	int					i;
	va_list				avs;
}						t_printf;

typedef struct			s_count
{
	int					l;
	int					j;
	int					k;
}						t_count;

typedef struct			s_dbl
{
	unsigned long long	m: 64;
	unsigned long long	e: 15;
	unsigned long long	s: 1;
}						t_dbl;

typedef union			u_mask
{
	t_dbl				bits;
	long double			num;
}						t_mask;

typedef	struct			s_buf
{
	char				*wh_num;
	char				*div_num;
	char				*pow_2;
	char				*pow_5;
}						t_buf;

int						ft_printf(const char *format, ...);
int						ft_parse_format(t_printf *f);
int						ft_get_all_flags(t_printf *f);

t_printf				*ft_init(const char *format);
t_printf				*ft_reset_init(t_printf *f);

t_printf				*get_flags_hzmps(t_printf *f);
t_printf				*get_width(t_printf *f);
t_printf				*get_precision(t_printf *f);
t_printf				*get_mod_length(t_printf *f);
t_printf				*get_conversion(t_printf *f);

void					ft_print_all(t_printf *f);
void					ft_print_str(t_printf *f);
void					ft_print_char(t_printf *f);
void					ft_print_address(t_printf *f);
void					ft_print_int(t_printf *f);
void					ft_print_uint(t_printf *f);
void					ft_print_oint(t_printf *f);
void					ft_print_xint(t_printf *f);
void					ft_print_percent(t_printf *f);
void					ft_print_other(t_printf *f);

void					ft_spacing(char c, t_printf *f, int length);
void					ft_ispacing(char c, t_printf *f, int length);
intmax_t				ft_get_num_modlen(t_printf *f);
void					print_sign(t_printf *f, long long res, int length);
uintmax_t				ft_get_unum_modlen(t_printf *f);

void					init_f(t_printf *get);
int						mem_alloc(t_buf *buf);
void					take_to_2(t_buf *buf, int pw);
void					take_to_5(t_buf *buf, int pow, int nul);
void					print_f(t_buf *buf, t_printf *get);
void					free_buf(t_buf *buf);
void					ft_round(t_buf *buf, t_printf *get);
void					change_div(t_buf *buf, int i);
void					change_whole(t_buf *buf);
int						check_plus_space(t_printf *get);
void					fill_width(t_buf *buf, t_printf *get);
int						is_this_number(t_mask bits, t_printf *get);

char					*ft_strnew(size_t size);
void					ft_putchar(char c);
void					*ft_memset(void *b, int c, size_t len);
char					*ft_strchr(const char *s, int c);
int						ft_strcmp(const char *s1, const char *s2);
size_t					ft_strlen(const char *s);
void					ft_putstr(char const *s);
char					*ft_strndup(const char *s1, int count);
char					*ft_itoa_base(int value, int base);
char					*ft_itoa_base_address16(unsigned long num);
char					*ft_itoa_base_ll(long long num, int base);
char					*ft_itoa_base_ll_pos(long long num, int base);
char					*ftbaseull(unsigned long long num, int base, char c);
void					ft_putunbr(unsigned long long n);
void					ft_putendl(char const *s);
void					*ft_memalloc(size_t size);
void					ft_memdel(void **ap);
void					ft_strdel(char **as);

#endif
