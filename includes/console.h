/*
** console.h for life in /home/delemo_b/life
** 
** Made by barthelemy delemotte
** Login   <delemo_b@epitech.net>
** 
** Started on  Fri May 31 14:17:20 2013 barthelemy delemotte
** Last update Tue Jun  4 23:48:59 2013 barthelemy delemotte
*/

#ifndef _CONSOLE_H_
# define _CONSOLE_H_

# include "life.h"

extern void life_console(t_map map, unsigned int size_x, unsigned int size_y,
			 unsigned int delta_time, t_rules rules);

#endif /* _CONSOLE_H_ */
