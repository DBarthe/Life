/*
** console.c for life in /home/delemo_b/life
** 
** Made by barthelemy delemotte
** Login   <delemo_b@epitech.net>
** 
** Started on  Fri May 31 13:35:50 2013 barthelemy delemotte
** Last update Tue Jun  4 23:50:28 2013 barthelemy delemotte
*/

#include <unistd.h>
#include <stdio.h>

#include "life.h"

static void map_display(t_map map, unsigned int size_x, unsigned int size_y, int it);

/*
** main of the console mode
*/
void life_console(t_map map, unsigned int size_x, unsigned int size_y,
		  unsigned int delta_time, t_rules rules)
{
  int it;
  unsigned int pop;

  it = 0;
  map_update_cel_state(map, size_x, size_y, &pop, rules);
  while (42)
    {
      map_display(map, size_x, size_y, it);
      usleep(delta_time);
      map_update(map, size_x, size_y, rules);
      map_update_cel_state(map, size_x, size_y, &pop, rules);
      it++;
    }
}

/*
** display the map on terminal
*/
static void map_display(t_map map, unsigned int size_x, unsigned int size_y, int it)
{
  unsigned int i;
  unsigned int j;

  printf("\033[H\033[2J");
  printf("iteration n°%d\n", it);
#ifdef _BORDER
  for (i = 0; i < size_x + 1; i++)
    printf("--");
  printf("\n");
#endif
  for (j = 0; j < size_y; j++)
    {
#ifdef _BORDER
      printf("|");
#endif
      for (i = 0; i < size_x; i++)
	{
	  printf(" %c", map[j][i] != DEAD ? 'o' : ' ');
	}
#ifdef _BORDER
      printf("| ");
#endif
      printf("\n");
    }
#ifdef _BORDER
  for (i = 0; i < size_x + 1; i++)
    printf("--");
#endif
  printf("\n");
}
