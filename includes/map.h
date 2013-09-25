/*
** map.h for life
**
** Made by barthelemy delemotte <delemo_b@epitech.eu>
*/

#ifndef _MAP_H_
# define _MAP_H_

/*
** datas types
*/
typedef char t_cellule;

typedef t_cellule **t_map;

/*
** cellule states
*/
enum
  {
    DEAD = 0,
    CEL_NEW,
    CEL_OLD,
    CEL_DYING,
    CEL_SHORT,
    CEL_OUT_RANGE
  };

# define ALIVE	(!DEAD)

extern t_map map_new(unsigned int size_x, unsigned int size_y);
extern inline void map_clean(t_map map, unsigned int size_x, unsigned int size_y);
extern void map_free(t_map map, unsigned int size_y);
extern void map_fill_with_input(t_map map, unsigned int size_x, unsigned int size_y, const int fd);
extern void map_cpy(t_map dst, t_map src, unsigned int size_x, unsigned int size_y);

#endif /* _MAP_H_ */
