/*
** life.h for life
**
** Made by barthelemy delemotte <delemo_b@epitech.eu>
*/

#ifndef _LIFE_H_
# define _LIFE_H_

# include "map.h"

struct s_rules
{
  enum { MAP_CIRCULAR, MAP_CLOSED } map_type;
  unsigned char r_born;
  unsigned char r_live;
};

typedef struct s_rules t_rules;

extern void map_update(t_map map, unsigned int size_x, unsigned int size_y, t_rules rules);
extern void map_update_cel_state(t_map map, unsigned int size_x, unsigned int size_y,
				 unsigned int *pop_p, t_rules rules);

#endif /*_LIFE_H_ */
