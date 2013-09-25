/*
** options.h for life
**
** Made by barthelemy delemotte <delemo_b@epitech.net>
*/

#ifndef _OPTIONS_H_
# define _OPTIONS_H_

# include "life.h"

# define TOKEN_SIZE_X 'x'
# define TOKEN_SIZE_Y 'y'
# define TOKEN_DELTA_TIME 't'
# define TOKEN_CH_MODE 'c'
# define TOKEN_FULLSCREEN 'f'
# define TOKEN_CH_FILE_TYPE 'i'
# define TOKEN_RULES 'r'
# define TOKEN_RULES_DELIMITOR '/'
# define TOKEN_RULES_MAP_TYPE 'm'

# define WITHOUT_ARGS_TOKENS "cfim"

# define SIZE_X_DFL 0
# define SIZE_Y_DFL 0
# define DELTA_TIME_DFL 30
# define MODE_DFL GRAPHIC_MODE
# define MODE_CHANGED CONSOLE_MODE
# define FILE_TYPE_DFL FILE_IMAGE
# define FILE_TYPE_CHANGED FILE_TEXT
# define RULES_BORN_DFL 0x4 /* 00000100 (3) */
# define RULES_LIVE_DFL 0x6 /* 00000110 (23) */
# define RULES_MAP_TYPE_DFL MAP_CIRCULAR
# define RULES_MAP_TYPE_CHANGED MAP_CLOSED


/*
** modes ( graphic | console )
*/
enum mode { CONSOLE_MODE, GRAPHIC_MODE };
typedef enum mode t_mode;

/*
** Options
*/
struct s_opt
{
  t_mode mode;
  enum { FILE_TEXT, FILE_IMAGE } file_type;
  unsigned int delta_time;
  unsigned int size_x;
  unsigned int size_y;
  char *filename;
  int fullscreen;
  t_rules rules;
};

typedef struct s_opt t_opt;

extern int parse_opt(int ac, char **av, t_opt *opt);

#endif /* _OPTIONS_H_ */
