/*
** import.c for life
**
** Made by barthelemy delemotte <delemo_b@epitech.net>
*/

#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "life.h"

static void pixel_to_cellule(t_cellule *cellule, Uint8 r, Uint8 g, Uint8 b);
static t_map surface_8bpp_to_map(SDL_Surface *s, unsigned int *size_x, unsigned int *size_y);
static t_map surface_32bpp_to_map(SDL_Surface *s, unsigned int *size_x, unsigned int *size_y);

/*
** create a map from a surface
*/
t_map surface_to_map(SDL_Surface *s, unsigned int *size_x, unsigned int *size_y)
{
  switch (s->format->BitsPerPixel)
    {
    case 8:
      return (surface_8bpp_to_map(s, size_x, size_y));
    case 32:
      return (surface_32bpp_to_map(s, size_x, size_y));
    default:
      fprintf(stderr, "Cannot import image: bad bpp %d\n", s->format->BitsPerPixel);
    }
  return (NULL);
}

/*
** create a map from an image with the size of the image
*/
t_map import_image(const char *file, unsigned int *size_x, unsigned int *size_y)
{
  SDL_Surface *s;
  t_map map;

  s = IMG_Load(file);
  if (s == NULL)
    {
      fprintf(stderr, "life: %s\n", IMG_GetError());
      return (NULL);
    }
  map = surface_to_map(s, size_x, size_y);
  SDL_FreeSurface(s);
  return (map);
}

/*
** create and return a copy of 'map' with the size (sx2, sy2)
** to be moved in map.c
*/
t_map map_extend(t_map map, unsigned int sx1, unsigned int sy1,
		 unsigned int sx2, unsigned int sy2)
{
  t_map new;
  unsigned int i, j;

  new = map_new(sx2, sy2);
  for (j = 0; j < sy1 && j < sy2; j++)
    {
      for (i = 0; i < sx1 && i < sx2; i++)
	{
	  new[j][i] = map[j][i];
	}
    }
  return (new);
}

/*
** create a map from an image with a fixed size
*/
t_map import_image_with_size(const char *file, unsigned int size_x, unsigned int size_y)
{
  t_map map_img, map_extended;
  unsigned int img_size_x, img_size_y;

  map_img = import_image(file, &img_size_x, &img_size_y);
  if (map_img == NULL)
    return (NULL);
  map_extended = map_extend(map_img, img_size_x, img_size_y, size_x, size_y);
  map_free(map_img, img_size_y);
  return (map_extended);
}

static void pixel_to_cellule(t_cellule *cellule, Uint8 r, Uint8 g, Uint8 b)
{
  int rand_nbr;

  if (r == 255 && g == 0 && b == 0)
    {
      rand_nbr = rand() % 2;
      if (rand_nbr > 0)
	*cellule = CEL_NEW;
      else
	*cellule = DEAD;
    }
  else if (r == 0 && g == 0 && b == 0)
    *cellule = CEL_NEW;
  else
    *cellule = DEAD;
}

static t_map surface_8bpp_to_map(SDL_Surface *s, unsigned int *size_x, unsigned int *size_y)
{
  t_map map;
  Uint8 *data;
  SDL_PixelFormat *fmt;
  SDL_Color color;
  Uint8 index;
  int i, j;

  fmt = s->format;

  SDL_LockSurface(s);

  map = map_new(s->w, s->h);
  data = (Uint8 *)s->pixels;

  for (j = 0; j < s->h; j++)
    {
      for (i = 0; i < s->w; i++)
	{
	  index = data[j * s->w + i];
	  color = fmt->palette->colors[index];
	  pixel_to_cellule(&map[j][i], color.r, color.g, color.b);
	}
    }

  SDL_UnlockSurface(s);

  *size_x = s->w;
  *size_y = s->h;
  return (map);
}

static t_map surface_32bpp_to_map(SDL_Surface *s, unsigned int *size_x, unsigned int *size_y)
{
  t_map map;
  Uint32 *data;
  SDL_PixelFormat *fmt;
  Uint32 pixel, tmp;
  Uint8 r, g, b;
  int i, j;

  fmt = s->format;

  SDL_LockSurface(s);

  map = map_new(s->w, s->h);
  data = (Uint32 *)s->pixels;

  for (j = 0; j < s->h; j++)
    {
      for (i = 0; i < s->w; i++)
	{
	  pixel = data[j * s->w + i];

	  /* Get Red component */
	  tmp = pixel & fmt->Rmask;
	  tmp = tmp >> fmt->Rshift;
	  tmp = tmp << fmt->Rloss;
	  r = (Uint8)tmp;

	  /* Get Green component */
	  tmp = pixel & fmt->Gmask;
	  tmp = tmp>>fmt->Gshift;
	  tmp = tmp<<fmt->Gloss;
	  g = (Uint8)tmp;

	  /* Get Blue component */
	  tmp = pixel & fmt->Bmask;
	  tmp = tmp >> fmt->Bshift;
	  tmp = tmp << fmt->Bloss;
	  b = (Uint8)tmp;

	  pixel_to_cellule(&map[j][i], r, g, b);
	}
    }

  SDL_UnlockSurface(s);

  *size_x = s->w;
  *size_y = s->h;
  return (map);
}
