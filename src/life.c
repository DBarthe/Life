/*
** life.c for life
**
** Made by barthelemy delemotte <delemo_b@epitech.eu>
*/

#include <stdlib.h>

#include "life.h"

static char count_cel_contacts(t_map map, unsigned int i, unsigned int j,
			       unsigned int size_x, unsigned int size_y,
			       unsigned int map_type);

/*
** update the map (do one cycle)
*/
void map_update(t_map map, unsigned int size_x, unsigned int size_y, t_rules rules)
{
  static t_map new = NULL;
  unsigned int i;
  unsigned int j;
  char contact_nbr;
  unsigned char contact_mask;
  register t_cellule cellule;

  /*
  ** free 'new' if map is null
  */
  if (map == NULL)
    {
      map_free(new, size_y);
      new = NULL;
      return ;
    }

  if (new == NULL)
    new = map_new(size_x, size_y);
  else
    map_clean(new, size_x, size_y);
  for (j = 0; j < size_y; j++)
    {
      for (i = 0; i < size_x; i++)
	{
	  contact_nbr = count_cel_contacts(map, i, j, size_x, size_y, rules.map_type);
	  if (!contact_nbr)
	    continue;
	  contact_mask = 0x1 << (contact_nbr - 1);
	  if ((cellule = map[j][i]) == DEAD)
	    {
	      if (contact_mask & rules.r_born)
		new[j][i] = CEL_NEW;
	    }
	  else if (cellule == CEL_NEW || cellule == CEL_OLD)
	    {
	      new[j][i] = CEL_OLD;
	    }
	}
    }
  map_cpy(map, new, size_x, size_y);
}

/*
** update the cel state (for CEL_SHORT and CEL_DYING)
*/
void map_update_cel_state(t_map map, unsigned int size_x, unsigned int size_y,
			  unsigned int *pop_p, t_rules rules)
{
  unsigned int	i;
  unsigned int	j;
  unsigned int	contact_nbr;
  unsigned char contact_mask;
  unsigned int pop;
  register t_cellule cellule;

  pop = 0;
  for (j = 0; j < size_y; j++)
    {
      for (i = 0; i < size_x; i++)
	{
	  if ((cellule = map[j][i]) == DEAD)
	    continue;
	  pop++;
	  contact_nbr = count_cel_contacts(map, i, j, size_x, size_y, rules.map_type);
	  contact_mask = 0x1 << (contact_nbr - 1);

	  if (!(contact_mask & rules.r_live))
	    {
	      if (cellule == CEL_NEW)
		map[j][i] = CEL_SHORT;
	      else
		map[j][i] = CEL_DYING;
	    }
	}
    }
  *pop_p = pop;
}

#define		CEL_L(x) ((( x ) + size_x - 1) % size_x)
#define		CEL_R(x) ((( x ) + 1) % size_x)
#define		CEL_U(x) ((( x ) + size_y - 1) % size_y)
#define		CEL_D(x) ((( x ) + 1) % size_y)

/*
** count the contacts number with the cellule at position (i;j)
*/
static char count_cel_contacts(t_map map, unsigned int i, unsigned int j,
			       unsigned int size_x, unsigned int size_y,
			       unsigned int map_type)
{
  register char contact_nbr;

  contact_nbr = 0;
  if (map_type == MAP_CIRCULAR)
    {
      contact_nbr += map[j][CEL_L(i)] ? 1 : 0;
      contact_nbr += map[j][CEL_R(i)] ? 1 : 0;
      contact_nbr += map[CEL_U(j)][i] ? 1 : 0;
      contact_nbr += map[CEL_D(j)][i] ? 1 : 0;
      contact_nbr += map[CEL_U(j)][CEL_R(i)] ? 1 : 0;
      contact_nbr += map[CEL_U(j)][CEL_L(i)] ? 1 : 0;
      contact_nbr += map[CEL_D(j)][CEL_R(i)] ? 1 : 0;
      contact_nbr += map[CEL_D(j)][CEL_L(i)] ? 1 : 0;
    }
  else /* MAP_CLOSED */
    {
      if (i > 0)
	{
	  contact_nbr += map[j][i - 1] ? 1 : 0;
	  if (j > 0)
	    contact_nbr += map[j - 1][i - 1] ? 1 : 0;
	  if (j + 1 < size_y)
	    contact_nbr += map[j + 1][i - 1] ? 1 : 0;
	}
      if (i + 1 < size_x)
	{
	  contact_nbr += map[j][i + 1] ? 1 : 0;
	  if (j > 0)
	    contact_nbr += map[j - 1][i + 1] ? 1 : 0;
	  if (j + 1 < size_y)
	    contact_nbr += map[j + 1][i + 1] ? 1 : 0;
	}
      if (j > 0)
	contact_nbr += map[j - 1][i] ? 1 : 0;
      if (j + 1 < size_y)
	contact_nbr += map[j + 1][i] ? 1 : 0;
    }
  return (contact_nbr);
}
