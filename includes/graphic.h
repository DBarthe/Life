/*
** graphic.h for life
**
** Made by barthelemy delemotte <delemo_b@epitech.net>
*/

#ifndef _GRAPHIC_H_
# define _GRAPHIC_H_

# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>

# include "life.h"

# define SCREEN_X 1800
# define SCREEN_Y 1000
# define SCREEN_BPP 32

# define STATUS_X 500
# define STATUS_Y 40
# define STATUS_COLOR SDL_MapRGB(g->screen->format, 200, 200, 200)
# define STATUS_MARGIN_LEFT 150
# define STATUS_MARGIN_INTERVAL 100
# define STATUS_BUFFER_SIZE 64

# define MAP_X SCREEN_X
# define MAP_Y (SCREEN_Y - STATUS_Y)
# define MAP_COLOR SDL_MapRGB(g->screen->format, 0, 0, 0)

# define _RGB(r, g, b) SDL_MapRGB(g->screen->format, r, g, b)
# define BACK_COLOR SDL_MapRGB(g->screen->format, 20, 20, 20)
# define CEL_NEW_COLOR SDL_MapRGB(g->screen->format, 0, 255, 0)
# define CEL_OLD_COLOR SDL_MapRGB(g->screen->format, 0, 0, 255)
# define CEL_DYING_COLOR SDL_MapRGB(g->screen->format, 255, 0, 0)
# define CEL_SHORT_COLOR SDL_MapRGB(g->screen->format, 255, 255, 0)

# define ZOOM_STEP 1
# define MVT_STEP max(50 / g->cellule_size, 1)
# define DELTA_TIME_STEP 1

struct s_game_st
{
  enum { GS_EXIT, GS_PLAY, GS_PAUSE, GS_STOP, GS_FW } state;
  unsigned int delta_time;
  unsigned int timer_last;
  t_map map_origin;
};

typedef struct s_game_st t_game_st;

struct s_status
{
  unsigned int it;
  unsigned int pop;
  t_game_st *gs;
  char buffer[STATUS_BUFFER_SIZE];

  SDL_Rect position;
  SDL_Color font_color;
  SDL_Surface *background;
  SDL_Surface *it_surface;
  SDL_Surface *pop_surface;
  SDL_Surface *pos_surface;
  SDL_Surface *zoom_surface;
  SDL_Surface *delta_time_surface;
};

typedef struct s_status t_status;

struct s_graphic
{
  unsigned int screen_x;
  unsigned int screen_y;
  unsigned int map_size_x;
  unsigned int map_size_y;

  unsigned int cellule_size_origin;
  unsigned int cellule_size;

  unsigned int zoom;
  unsigned int pos_x;
  unsigned int pos_y;

  t_status status;

  SDL_Surface *screen;
  SDL_Surface *map;
  SDL_Surface *cellule_surface_new;
  SDL_Surface *cellule_surface_old;
  SDL_Surface *cellule_surface_dying;
  SDL_Surface *cellule_surface_short;
  SDL_Surface *cellule_surface_out;

  TTF_Font *font;
};

typedef struct s_graphic t_graphic;

extern void life_graphic(t_map map, unsigned int size_x, unsigned int size_y,
			 unsigned int delta_time, int fullscreen, t_rules rules);

/*
** Fonts
*/
# define FONT_PATH "./fonts/FreeSans.ttf"
# define FONT_SIZE 20

#endif /* _GRAPHIC_H_ */
