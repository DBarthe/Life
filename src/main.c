/*
** main.c for life in /home/delemo_b/life
** 
** Made by barthelemy delemotte
** Login   <delemo_b@epitech.net>
** 
** Started on  Fri May 31 13:33:52 2013 barthelemy delemotte
** Last update Wed Jun  5 20:52:49 2013 barthelemy delemotte
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "options.h"
#include "life.h"
#include "console.h"
#include "graphic.h"
#include "import.h"

static t_map load_map(t_opt *opt)
{
  t_map map;
  int fd;

  srand(time(NULL));
  if (opt->file_type == FILE_IMAGE)
    {
      if (opt->size_x != SIZE_X_DFL && opt->size_y != SIZE_Y_DFL)
	map = import_image_with_size(opt->filename, opt->size_x, opt->size_y);
      else
	map = import_image(opt->filename, &opt->size_x, &opt->size_y);
      if (map == NULL)
	exit(EXIT_FAILURE);
    }
  else
    {
      fd = open(opt->filename, O_RDONLY);
      if (fd == -1)
	{
	  fprintf(stderr, "life: '%s': %s\n", opt->filename, strerror(errno));
	  exit(EXIT_FAILURE);
	}
      if (opt->size_x == 0)
	opt->size_x = 50;
      if (opt->size_y == 0)
	opt->size_y = 50;
      map = map_new(opt->size_x, opt->size_y);
      map_fill_with_input(map, opt->size_x, opt->size_y, fd);
    }
  return (map);
}


int main(int ac, char **av)
{
  t_map map;
  t_opt	opt;

  parse_opt(ac, av, &opt);
  if (opt.filename == NULL)
    {
      fprintf(stderr, "life: missing file name\n");
      exit(EXIT_FAILURE);
    }

  map = load_map(&opt);

  if (opt.mode == CONSOLE_MODE)
    life_console(map, opt.size_x, opt.size_y, opt.delta_time * 1000, opt.rules);
  else if (opt.mode == GRAPHIC_MODE)
    life_graphic(map, opt.size_x, opt.size_y, opt.delta_time, opt.fullscreen, opt.rules);
  map_free(map, opt.size_y);
  return (0);
}
