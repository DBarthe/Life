/*
** map.c for life
**
** Made by barthelemy delemotte <delemo_b@epitech.eu>
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map.h"

extern char *get_next_line(const int fd);

/*
** create a new map with size_x and size_y
*/
t_map map_new(unsigned int size_x, unsigned int size_y)
{
  t_map map;
  unsigned int j;

  if ((map = malloc(size_y * sizeof(*map))) == NULL)
    {
      fprintf(stderr, "unable to malloc %lu\n", size_y * sizeof(*map));
      exit(EXIT_FAILURE);
    }
  for (j = 0; j < size_y; j++)
    {
      if ((map[j] = malloc(size_x * sizeof(**map))) == NULL)
	{
	  fprintf(stderr, "unable to malloc %lu\n", size_x * sizeof(**map));
	  exit(EXIT_FAILURE);
	}
    }
  map_clean(map, size_x, size_y);
  return (map);
}

/*
** free a map
*/
void map_free(t_map map, unsigned int size_y)
{
  unsigned int j;

  if (map)
    {
      for (j = 0; j < size_y; j++)
	{
	  free(map[j]);
	}
      free(map);
    }
}

/*
** fill the map from the input fd ('0' -> dead,  != '0' -> alive)
*/
void map_fill_with_input(t_map map, unsigned int size_x, unsigned int size_y, const int fd)
{
  char *buffer;
  unsigned int i;
  unsigned int j;

  j = 0;
  while (j < size_y && (buffer = get_next_line(fd)))
    {
      for (i = 0; i < size_x && buffer[i]; i++)
	{
	  map[j][i] = (buffer[i] == '0' ? DEAD : CEL_NEW);
	}
      free(buffer);
      j++;
    }
}

/*
** copy the map src into the map dst (they must have the same size [size_x, size_y])
*/
void map_cpy(t_map dst, t_map src, unsigned int size_x, unsigned int size_y)
{
  unsigned int i;
  unsigned int j;

  for (j = 0; j < size_y; j++)
    {
      for (i = 0; i < size_x; i++)
	{
	  dst[j][i] = src[j][i];
	}
    }
}

/*
** clean map, fill it with zeros.
*/
inline void map_clean(t_map map, unsigned int size_x, unsigned int size_y)
{
  unsigned int j;

  for (j = 0; j < size_y; j++)
    {
      memset(map[j], 0, size_x * sizeof(**map));
    }
}
