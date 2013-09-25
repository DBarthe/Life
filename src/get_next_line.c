/*
** get_next_line.c for life
**
** Made by barthelemy delemotte <delemo_b@epitech.eu>
*/

#include <stdlib.h>
#include <unistd.h>

#define READ_LEN 64

static void *xmalloc(size_t size)
{
  void *data;

  data = malloc(size);
  if (data == NULL)
    exit(EXIT_FAILURE);
  return (data);
}

static unsigned int my_strlen(const char *s)
{
  register unsigned int len;

  len = 0;
  if (s)
    while (s[len])
      len++;
  return (len);
}

static int in_line(char *s)
{
  if (!s)
    return (1);
  while (*s)
    if (*(s++) == '\n')
      return (0);
  return (1);
}

static char *remake_store(char *store, char *s1, char *s2)
{
  char *new_store;
  int i;
  int j;

  i = 0;
  j = 0;
  new_store = xmalloc((my_strlen(s1) + my_strlen(s2) + 1) * sizeof(char));
  if (new_store == NULL)
    return (NULL);
  while (s1 && s1[i])
    {
      new_store[i] = s1[i];
      i++;
    }
  while (s2 && s2[j])
    {
      new_store[i + j] = s2[j];
      j++;
    }
  new_store[i + j] = '\0';
  if (store)
    free(store);
  return (new_store);
}

static char *make_line(char *sread, char **store)
{
  char *line;
  int ist;
  int isr;
  int ili;

  line = xmalloc((my_strlen(sread) + my_strlen(*store) + 1) * sizeof(char));
  if (line == NULL)
    return (NULL);
  ili = 0;
  ist = 0;
  while (*store && (*store)[ist] && (*store)[ist] != '\n')
    line[ili++] = (*store)[ist++];
  if (*store && (*store)[ist] == '\n')
    ist++;
  isr = 0;
  while (sread && in_line(*store) && sread[isr] && sread[isr] != '\n')
    line[ili++] = sread[isr++];
  if (sread && sread[isr] == '\n')
    isr++;
  *store = remake_store(*store, *store + ist, sread + isr);
  line[ili] = '\0';
  return (line);
}

static void gnt_free(char *s1, char *s2, char *s3, char *s4)
{
  free(s1);
  free(s2);
  if (!s4)
    free(s3);
}

char *get_next_line(const int fd)
{
  char *buf;
  char *sread;
  static char *store = NULL;
  int ret;
  char *line;

  if ((buf = xmalloc((READ_LEN + 1) * sizeof(char))) == NULL)
    return (NULL);
  buf[0] = '\0';
  ret = 1;
  sread = NULL;
  while (in_line(store) && in_line(buf) && ret > 0)
    {
      if ((ret = read(fd, buf, READ_LEN)) > 0)
	{
	  buf[ret] = '\0';
	  sread = remake_store(sread, sread, buf);
	}
    }
  line = NULL;
  if (!(ret < 0 || (!ret && !my_strlen(store) && !my_strlen(sread))))
    line = make_line(sread, &store);
  gnt_free(buf, sread, store, line);
  return (line);
}
