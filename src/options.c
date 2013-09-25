/*
** options.c for life in /home/delemo_b/life
** 
** Made by barthelemy delemotte
** Login   <delemo_b@epitech.net>
** 
** Started on  Fri May 31 14:01:04 2013 barthelemy delemotte
** Last update Wed Jun  5 21:40:32 2013 barthelemy delemotte
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "options.h"

static t_rules get_rules(const char *arg);

int parse_opt(int ac, char **av, t_opt *opt)
{
  int i;

  /*
  ** default options
  */
  opt->filename = NULL;
  opt->file_type = FILE_TYPE_DFL;
  opt->size_y = SIZE_Y_DFL;
  opt->size_x = SIZE_X_DFL;
  opt->delta_time = DELTA_TIME_DFL;
  opt->mode = MODE_DFL;
  opt->fullscreen = 0;
  opt->rules.r_born = RULES_BORN_DFL;
  opt->rules.r_live = RULES_LIVE_DFL;
  opt->rules.map_type = RULES_MAP_TYPE_DFL;

  i = 1;
  while (i < ac)
    {
      if (av[i][0] == '-')
	{
	  if (strchr(WITHOUT_ARGS_TOKENS, av[i][1]) == NULL && i + 1 >= ac)
	    {
	      fprintf(stderr, "%s: option without argument\n", av[0]);
	      exit(EXIT_FAILURE);
	    }
	  switch (av[i][1])
	    {
	    case TOKEN_SIZE_X:
	      opt->size_x = atoi(av[i + 1]);
	      break;
	    case TOKEN_SIZE_Y:
	      opt->size_y= atoi(av[i + 1]);
	      break;
	    case TOKEN_DELTA_TIME:
	      opt->delta_time = atoi(av[i + 1]);
	      break;
	    case TOKEN_CH_MODE:
	      opt->mode = MODE_CHANGED;
	      break;
	    case TOKEN_FULLSCREEN:
	      opt->fullscreen = 1;
	      break;
	    case TOKEN_CH_FILE_TYPE:
	      opt->file_type = FILE_TYPE_CHANGED;
	      break;
	    case TOKEN_RULES:
	      opt->rules = get_rules(av[i + 1]);
	      break;
	    case TOKEN_RULES_MAP_TYPE:
	      opt->rules.map_type = RULES_MAP_TYPE_CHANGED;
	      break;
	    default:
	      fprintf(stderr, "%s: unknow option '%c'\n", av[0], av[i][1]);
	      exit(EXIT_FAILURE);
	    }
	  if (strchr(WITHOUT_ARGS_TOKENS, av[i][1]) == NULL)
	    i++;
	}
      else
	opt->filename = av[i];
      i++;
    }
  return (0);
}

static t_rules get_rules(const char *arg)
{
  t_rules rules;
  int i;
  int nbr;

  rules.r_born = 0;
  rules.r_live = 0;

  i = 0;
  while (arg[i] && arg[i] != TOKEN_RULES_DELIMITOR)
    {
      nbr = (int)(arg[i] - '0');
      if (nbr < 1 || nbr > 8)
	{
	  fprintf(stderr, "life: invalid rules number '%c'\n", arg[i]);
	  exit(EXIT_FAILURE);
	}
      rules.r_born |= (0x1 << (nbr - 1));
      i++;
    }
  if (arg[i])
    i++;
  while (arg[i])
    {
      nbr = (int)(arg[i] - '0');
      if (nbr < 1 || nbr > 8)
	{
	  fprintf(stderr, "life: invalid rules number '%c'\n", arg[i]);
	  exit(EXIT_FAILURE);
	}
      rules.r_live |= (0x1 << (nbr - 1));
      i++;
    }
  return (rules);
}
