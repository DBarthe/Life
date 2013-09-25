/*
** graphic.c for life
**
** Made by barthelemy delemotte <delemo_b@epitech.eu>
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "graphic.h"
#include "life.h"

static void graphic_init(t_graphic *g, unsigned int cellule_size,
			 unsigned int screen_x, unsigned int screen_y,
			 unsigned int size_x, unsigned int size_y,
			 int fullscreen);
static void map_graphic_display(t_map map, t_graphic *g,
				unsigned int size_x, unsigned int size_y,
				unsigned int map_type);
static void display_status(t_graphic *g);
static void display_iteration(t_graphic *g);
static void change_zoom(t_graphic *g, unsigned int z, unsigned int size_x, unsigned int size_y);
static void change_position(t_graphic *g, int mvt_x, int mvt_y,
			    unsigned int size_x, unsigned size_y,
			    unsigned int map_type);
static inline SDL_Surface *x_new_rgb_surface(unsigned int height, unsigned int width, Uint32 color);
static void event_treatment(t_graphic *g, t_map map, t_game_st *gs,
			    unsigned int size_x, unsigned int size_y,
			    t_rules rules);


/*
** Tools
*/
static inline unsigned int min(unsigned int a, unsigned int b)
{
  return (a > b ? b : a);
}

static inline unsigned int max(unsigned int a, unsigned int b)
{
  return (a > b ? a : b);
}

/*
** main of the graphic mode
*/
void life_graphic(t_map map, unsigned int size_x, unsigned int size_y,
		  unsigned int delta_time, int fullscreen, t_rules rules)
{
  t_graphic g;
  t_game_st game_st;
  unsigned int cellule_size;

  /*
  ** to prevent floating exception
  */
  assert(size_x && size_y);

  /*
  ** calcul cellule size
  */
  cellule_size = min(MAP_X / size_x, MAP_Y / size_y);
  if (cellule_size == 0)
    {
      cellule_size = 1;
      /*
	fprintf(stderr, "life: the size of the map is too long\n");
	exit(EXIT_FAILURE);
      */
    }

  /*
  ** init graphic
  */
  graphic_init(&g, cellule_size, SCREEN_X, SCREEN_Y,
	       size_x, size_y, fullscreen);

  /*
  ** init game state
   */
  game_st.timer_last = 0;
  game_st.delta_time = delta_time;
  game_st.state = GS_STOP;
  map_update_cel_state(map, size_x, size_y, &g.status.pop, rules);
  game_st.map_origin = map_new(size_x, size_y);
  map_cpy(game_st.map_origin, map, size_x, size_y);
  g.status.gs = &game_st;

  /*
  ** first displaying
  */
  map_graphic_display(map, &g, size_x, size_y, rules.map_type);

  /*
  ** main loop
  */
  while (game_st.state != GS_EXIT)
    {
      event_treatment(&g, map, &game_st, size_x, size_y, rules);
      if (game_st.state == GS_PLAY)
	{
	  if (game_st.delta_time <= SDL_GetTicks() - game_st.timer_last)
	    {
	      game_st.timer_last = SDL_GetTicks();
	      map_update(map, size_x, size_y, rules);
	      map_update_cel_state(map, size_x, size_y, &g.status.pop, rules);
	      g.status.it++;
	      map_graphic_display(map, &g, size_x, size_y, rules.map_type);
	    }
	}
      else if (game_st.state == GS_FW)
	{
	  map_update(map, size_x, size_y, rules);
	  map_update_cel_state(map, size_x, size_y, &g.status.pop, rules);
	  g.status.it++;
	  display_iteration(&g);
	  SDL_Flip(g.screen);
	}
    }

  /*
  ** free SDL datas
  */
  SDL_FreeSurface(g.cellule_surface_new);
  SDL_FreeSurface(g.cellule_surface_old);
  SDL_FreeSurface(g.cellule_surface_dying);
  SDL_FreeSurface(g.cellule_surface_short);
  SDL_FreeSurface(g.cellule_surface_out);
  SDL_FreeSurface(g.status.it_surface);
  SDL_FreeSurface(g.status.pop_surface);
  SDL_FreeSurface(g.status.pos_surface);
  SDL_FreeSurface(g.status.zoom_surface);
  SDL_FreeSurface(g.status.delta_time_surface);
  TTF_CloseFont(g.font);

  /*
  ** free others datas
  */
  map_free(game_st.map_origin, size_y);
}

/*
** for SDL events ...
*/
static void event_treatment(t_graphic *g, t_map map, t_game_st *gs,
			    unsigned int size_x, unsigned int size_y,
			    t_rules rules)
{
  static SDL_Event event;
  int need_display;

  if (gs->state == GS_PLAY || gs->state == GS_FW)
    SDL_PollEvent(&event);
  else
    SDL_WaitEvent(&event);

  need_display = 0;
  switch (event.type)
    {
    case SDL_QUIT:
      gs->state = GS_EXIT;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
	  gs->state = GS_EXIT;
	  break;
	case SDLK_p:
	  gs->state = GS_PAUSE;
	  need_display = 1;
	  break;
	case SDLK_r:
	  gs->state = GS_PLAY;
	  break;
	case SDLK_s:
	  gs->state = GS_STOP;
	  g->status.it = 0;
	  map_cpy(map, gs->map_origin, size_x, size_y);
	  need_display = 1;
	  break;
	case SDLK_a:
	  change_zoom(g, ZOOM_STEP, size_x, size_y);
	  need_display = 1;
	  break;
	case SDLK_z:
	  change_zoom(g, -ZOOM_STEP, size_x, size_y);
	  need_display = 1;
	  break;
	case SDLK_UP:
	  change_position(g, 0, -MVT_STEP, size_x, size_y, rules.map_type);
	  need_display = 1;
	  break;
	case SDLK_DOWN:
	  change_position(g, 0, MVT_STEP, size_x, size_y, rules.map_type);
	  need_display = 1;
	  break;
	case SDLK_LEFT:
	  change_position(g, -MVT_STEP, 0, size_x, size_y, rules.map_type);
	  need_display = 1;
	  break;
	case SDLK_RIGHT:
	  change_position(g, MVT_STEP, 0, size_x, size_y, rules.map_type);
	  need_display = 1;
	  break;
	case SDLK_SPACE:
	  g->pos_x = size_x / 2;
	  g->pos_y = size_y / 2;
	  change_zoom(g, 1 - g->zoom, size_x, size_y);
	  need_display = 1;
	  break;
	case SDLK_v:
	  g->status.gs->delta_time += DELTA_TIME_STEP;
	  display_status(g);
	  SDL_Flip(g->screen);
	  break;
	case SDLK_c:
	  if (gs->delta_time >= DELTA_TIME_STEP)
	    gs->delta_time -= DELTA_TIME_STEP;
	  display_status(g);
	  SDL_Flip(g->screen);
	  break;
	case SDLK_f:
	  gs->state = GS_FW;
	  SDL_FillRect(g->screen, NULL, BACK_COLOR);
	  break;
	default:
	  break;
	}
      break;
    }

  if (need_display && gs->state != GS_FW)
    map_graphic_display(map, g, size_x, size_y, rules.map_type);
}


static void change_position(t_graphic *g, int mvt_x, int mvt_y,
			    unsigned int size_x, unsigned size_y,
			    unsigned int map_type)
{
  (void)map_type;
  /*
  unsigned int cel_nbr_vision_x, cel_nbr_vision_y;

  if (map_type == MAP_CLOSED)
    {
      cel_nbr_vision_x = (g->map_size_x / g->cellule_size) / 2;
      cel_nbr_vision_y = (g->map_size_y / g->cellule_size) / 2;
      if ((mvt_x < 0 && g->pos_x < cel_nbr_vision_x - mvt_x) ||
	  (mvt_x > 0 && g->pos_x + mvt_x + cel_nbr_vision_x >= size_x))
	return ;
      if ((mvt_y < 0 && g->pos_y < cel_nbr_vision_y - mvt_y) ||
	  (mvt_y > 0 && g->pos_y + mvt_y + cel_nbr_vision_y >= size_y))
	return ;
    }
  */
  g->pos_x = (g->pos_x + mvt_x + size_x) % size_x;
  g->pos_y = (g->pos_y + mvt_y + size_y) % size_y;
}

static void change_zoom(t_graphic *g, unsigned int z, unsigned int size_x, unsigned int size_y)
{
  unsigned int cellule_size;

  if (g->zoom + z <= 0)
    return ;
  g->zoom += z;

  /*
  ** re - initialize cellules surfaces
  */
  SDL_FreeSurface(g->cellule_surface_new);
  SDL_FreeSurface(g->cellule_surface_old);
  SDL_FreeSurface(g->cellule_surface_dying);
  SDL_FreeSurface(g->cellule_surface_short);
  SDL_FreeSurface(g->cellule_surface_out);

  g->cellule_size = g->cellule_size_origin * g->zoom;
  cellule_size = g->cellule_size;

  g->cellule_surface_new = x_new_rgb_surface(cellule_size, cellule_size, CEL_NEW_COLOR);
  g->cellule_surface_old = x_new_rgb_surface(cellule_size, cellule_size, CEL_OLD_COLOR);
  g->cellule_surface_dying = x_new_rgb_surface(cellule_size, cellule_size, CEL_DYING_COLOR);
  g->cellule_surface_short = x_new_rgb_surface(cellule_size, cellule_size, CEL_SHORT_COLOR);
  g->cellule_surface_out = x_new_rgb_surface(cellule_size, cellule_size, BACK_COLOR);

  /*
  ** re - initialize map surface
  */
  SDL_FreeSurface(g->map);

  g->map_size_x = min(MAP_X / cellule_size, size_x) * cellule_size;
  g->map_size_y = min(MAP_Y / cellule_size, size_y) * cellule_size;

  g->map = x_new_rgb_surface(g->map_size_x, g->map_size_y, MAP_COLOR);
}

static inline SDL_Surface *x_new_rgb_surface(unsigned int height, unsigned int width, Uint32 color)
{
  SDL_Surface *s;

  s = SDL_CreateRGBSurface(SDL_HWSURFACE, height, width, SCREEN_BPP, 0, 0, 0, 0);
  if (s == NULL)
    {
      fprintf(stderr, "Can't create RGB surface: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  SDL_FillRect(s, NULL, color);
  return (s);
}

static void status_init(t_graphic *g)
{
  /*
  ** initialize status surface
  */
  g->status.background = x_new_rgb_surface(g->screen_x, STATUS_Y, STATUS_COLOR);

  g->status.it = 0;
  g->status.pop = 0;

  g->status.position.x = 0;
  g->status.position.y = g->screen_y - STATUS_Y;

  g->status.font_color.r = 0;
  g->status.font_color.g = 0;
  g->status.font_color.b = 0;

  g->status.it_surface = NULL;
  g->status.pop_surface = NULL;
  g->status.pos_surface = NULL;
  g->status.zoom_surface = NULL;
  g->status.delta_time_surface = NULL;
}

/*
** initialize graphic datas
*/
static void graphic_init(t_graphic *g, unsigned int cellule_size,
			 unsigned int screen_x, unsigned int screen_y,
			 unsigned int size_x, unsigned int size_y,
			 int fullscreen)
{
  unsigned int map_size_x, map_size_y;

  map_size_x = min(MAP_X / cellule_size, size_x) * cellule_size;
  map_size_y = min(MAP_Y / cellule_size, size_y) * cellule_size;

  /*
  ** initialize SDL and video mode
  */
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
      fprintf(stderr, "Can't init SDL: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  atexit(SDL_Quit);
  g->screen = SDL_SetVideoMode(screen_x, screen_y, SCREEN_BPP,
			       SDL_HWSURFACE | SDL_DOUBLEBUF |
			       (fullscreen ? SDL_FULLSCREEN : 0));

  /*
  ** initialize SDL_ttf and fonts
  */
  if (TTF_Init() == -1)
    {
      fprintf(stderr, "Can't init SDL_ttf; %s\n", TTF_GetError());
      exit(EXIT_FAILURE);
    }
  atexit(TTF_Quit);
  if ((g->font = TTF_OpenFont(FONT_PATH, FONT_SIZE)) == NULL)
    {
      fprintf(stderr, "Can't open font '%s': %s\n", FONT_PATH, TTF_GetError());
      exit(EXIT_FAILURE);
    }

  /*
  ** initialize screen
  */
  if (g->screen == NULL)
    {
      fprintf(stderr, "Can't set video mode: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  SDL_FillRect(g->screen, NULL, BACK_COLOR);
  g->screen_x = screen_x;
  g->screen_y = screen_y;

  /*
  ** initialize cellules surfaces
  */
  g->cellule_surface_new = x_new_rgb_surface(cellule_size, cellule_size, CEL_NEW_COLOR);
  g->cellule_surface_old = x_new_rgb_surface(cellule_size, cellule_size, CEL_OLD_COLOR);
  g->cellule_surface_dying = x_new_rgb_surface(cellule_size, cellule_size, CEL_DYING_COLOR);
  g->cellule_surface_short = x_new_rgb_surface(cellule_size, cellule_size, CEL_SHORT_COLOR);
  g->cellule_surface_out = x_new_rgb_surface(cellule_size, cellule_size, BACK_COLOR);

  /*
  ** initialize map surface
  */
  g->map = x_new_rgb_surface(map_size_x, map_size_y, MAP_COLOR);
  g->map_size_x = map_size_x;
  g->map_size_y = map_size_y;


  g->zoom = 1;
  g->pos_x = (map_size_x / cellule_size) / 2;
  g->pos_y = (map_size_y / cellule_size) / 2;
  g->cellule_size_origin = cellule_size;
  g->cellule_size = cellule_size;

  status_init(g);

  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL);
}

/*
** display the map on windows
*/
static void map_graphic_display(t_map map, t_graphic *g,
				unsigned int size_x, unsigned int size_y,
				unsigned int map_type)
{
  SDL_Rect p;

  unsigned int i;
  unsigned int j;

  unsigned int start_screen_pos_x;
  unsigned int cellule_size;
  unsigned int cellules_nbr_x;
  unsigned int cellules_nbr_y;
  unsigned int start_cellule_x;
  unsigned int start_cellule_y;
  unsigned int end_cellule_x;
  unsigned int end_cellule_y;

  t_cellule cellule_value;

  cellule_size = g->cellule_size;

  SDL_FillRect(g->screen, NULL, BACK_COLOR);

  p.x = g->screen_x / 2 - g->map_size_x / 2;
  p.y = (g->screen_y - STATUS_Y) / 2 - g->map_size_y / 2;
  SDL_BlitSurface(g->map, NULL, g->screen, &p);

  cellules_nbr_x = g->map_size_x / cellule_size;
  cellules_nbr_y = g->map_size_y / cellule_size;

  start_cellule_x = (size_x + g->pos_x - cellules_nbr_x / 2) % size_x;
  start_cellule_y = (size_y + g->pos_y - cellules_nbr_y / 2) % size_y;

  end_cellule_x = start_cellule_x + cellules_nbr_x;
  end_cellule_y = start_cellule_y + cellules_nbr_y;

  start_screen_pos_x = p.x;
  for (j = start_cellule_y; j < end_cellule_y; j++)
    {
      p.x = start_screen_pos_x;
      for (i = start_cellule_x; i < end_cellule_x; i++)
	{
	  if (map_type == MAP_CIRCULAR)
	    cellule_value = map[j % size_y][i % size_x];
	  else /* MAP_CLOSED */
	    {
	      if (j < size_y && i < size_x)
		cellule_value = map[j][i];
	      else
		cellule_value = CEL_OUT_RANGE;
	    }
	  switch (cellule_value)
	    {
	    case CEL_NEW:
	      SDL_BlitSurface(g->cellule_surface_new, NULL, g->screen, &p);
	      break;
	    case CEL_OLD:
	      SDL_BlitSurface(g->cellule_surface_old, NULL, g->screen, &p);
	      break;
	    case CEL_DYING:
	      SDL_BlitSurface(g->cellule_surface_dying, NULL, g->screen, &p);
	      break;
	    case CEL_SHORT:
	      SDL_BlitSurface(g->cellule_surface_short, NULL, g->screen, &p);
	      break;
	    case CEL_OUT_RANGE:
	      SDL_BlitSurface(g->cellule_surface_out, NULL, g->screen, &p);
	      break;
	    }
	  p.x += cellule_size;
	}
      p.y += cellule_size;
    }
  display_status(g);
  SDL_Flip(g->screen);
}

static void display_iteration(t_graphic *g)
{
  SDL_Rect p;

  p = g->status.position;
  SDL_BlitSurface(g->status.background, NULL, g->screen, &p);

  /*
  ** display iteration
  */
  p.x += STATUS_MARGIN_LEFT;
  p.y += STATUS_Y / 2 - FONT_SIZE / 2;
  if (g->status.it_surface)
    SDL_FreeSurface(g->status.it_surface);
  snprintf(g->status.buffer, STATUS_BUFFER_SIZE, "iteration: %d", g->status.it);
  g->status.it_surface = TTF_RenderText_Solid(g->font, g->status.buffer, g->status.font_color);
  SDL_BlitSurface(g->status.it_surface, NULL, g->screen, &p);
}

static void display_status(t_graphic *g)
{
  SDL_Rect p;

  p = g->status.position;
  SDL_BlitSurface(g->status.background, NULL, g->screen, &p);

  /*
  ** display iteration
  */
  p.x += STATUS_MARGIN_LEFT;
  p.y += STATUS_Y / 2 - FONT_SIZE / 2;
  if (g->status.it_surface)
    SDL_FreeSurface(g->status.it_surface);
  snprintf(g->status.buffer, STATUS_BUFFER_SIZE, "iteration: %d", g->status.it);
  g->status.it_surface = TTF_RenderText_Solid(g->font, g->status.buffer, g->status.font_color);
  SDL_BlitSurface(g->status.it_surface, NULL, g->screen, &p);
  p.x += g->status.it_surface->w + STATUS_MARGIN_INTERVAL;

  /*
  ** display population
  */
  if (g->status.pop_surface)
    SDL_FreeSurface(g->status.pop_surface);
  snprintf(g->status.buffer, STATUS_BUFFER_SIZE, "population: %d", g->status.pop);
  g->status.pop_surface = TTF_RenderText_Solid(g->font, g->status.buffer, g->status.font_color);
  SDL_BlitSurface(g->status.pop_surface, NULL, g->screen, &p);
  p.x += g->status.pop_surface->w + STATUS_MARGIN_INTERVAL;

  /*
  ** display position
  */
  if (g->status.pos_surface)
    SDL_FreeSurface(g->status.pos_surface);
  snprintf(g->status.buffer, STATUS_BUFFER_SIZE, "position: [%d;%d] / zoom: %d00%%",
	   g->pos_x, g->pos_y, g->zoom);
  g->status.pos_surface = TTF_RenderText_Solid(g->font, g->status.buffer, g->status.font_color);
  SDL_BlitSurface(g->status.pos_surface, NULL, g->screen, &p);
  p.x += g->status.pos_surface->w + STATUS_MARGIN_INTERVAL;

  /*
  ** display delta_time
  */
  if (g->status.delta_time_surface)
    SDL_FreeSurface(g->status.delta_time_surface);
  snprintf(g->status.buffer, STATUS_BUFFER_SIZE, "delta time: %dms", g->status.gs->delta_time);
  g->status.delta_time_surface = TTF_RenderText_Solid(g->font, g->status.buffer, g->status.font_color);
  SDL_BlitSurface(g->status.delta_time_surface, NULL, g->screen, &p);
  p.x += g->status.delta_time_surface->w + STATUS_MARGIN_INTERVAL;
}
