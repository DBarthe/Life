/*
** import.h for life
**
** Made by barthelemy delemotte <delemo_b@epitech.net>
*/

#ifndef _IMPORT_H_
# define _IMPORT_H_

extern t_map import_image(const char *file, unsigned int *size_x, unsigned int *size_y);
extern t_map map_extend(t_map map, unsigned int sx1, unsigned int sy1,
			unsigned int sx2, unsigned int sy2);
extern t_map import_image_with_size(const char *file, unsigned int size_x, unsigned int size_y);

#endif /* _IMPORT_H_ */
