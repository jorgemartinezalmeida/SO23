#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
enum
{ Maxsize = 1024 + 1
};

int
counttokens (char *path)
{
  char *aux = NULL;
  aux = strdup (path);
  int n = 0;
  char *rest = NULL;
  char *token;
  for (token = strtok_r (aux, "/", &rest);
       token != NULL; token = strtok_r (NULL, "/", &rest))
    {
      n++;
    }
  free (aux);
  return n;
}

int
tokenpath (char *list[], char *path)
{

  char *aux = NULL;
  aux = strdup (path);
  int n = 0;
  char *rest = NULL;
  char *token;
  for (token = strtok_r (path, "/", &rest);
       token != NULL; token = strtok_r (NULL, "/", &rest))
    {
      if ((list[n] = malloc ((strlen (token) + 1) * sizeof (char))) == NULL)
	{
	  perror ("Error in malloc");
	  exit (EXIT_FAILURE);
	}
      snprintf (list[n], strlen (token) + 1, "%s", token);
      n++;
    }
  free (aux);
  return n;
}

void
directorioprincipal (char *path)
{
  char *rest = NULL;
  char *token;
  for (token = strtok_r (path, "/", &rest);
       token != NULL; token = strtok_r (NULL, ":", &rest))
    {
      snprintf (path, strlen (token) + 1, "%s", token);
      break;			/* solo coja el primero */
    }
}

char *
adcwd (char *argument, char *userpath)
{
  char cwd[Maxsize] = "";
  if (argument[0] != '/')
    {

      if (getcwd (cwd, Maxsize) == NULL)
	{
	  perror ("getcwd");
	  exit (EXIT_FAILURE);
	}
      if ((userpath =
	   malloc ((strlen (argument) + strlen (cwd) + 2) *
		   sizeof (char))) == NULL)
	{
	  exit (EXIT_FAILURE);
	}
      if (argument[0] == '.' && argument[1] == '.')
	{
	  directorioprincipal (cwd);
	}
      strncat (cwd, "/", ((strlen (cwd) + 1) * sizeof (char)));
      strncat (cwd, argument,
	       (strlen (cwd) + strlen (argument) * sizeof (char)));
      snprintf (userpath, ((strlen (cwd)) + 1) * sizeof (char), "%s\n", cwd);
    }
  else
    {
      if ((userpath =
	   (char *) malloc ((strlen (argument) + 1) * sizeof (char))) == NULL)
	{
	  exit (EXIT_FAILURE);
	}
      snprintf (userpath, (strlen (argument) + 1) * sizeof (char), "%s\n",
		argument);
    }
  return userpath;
}

int
verpunto (char *elemento)
{
  if (strncmp (elemento, ".", strlen(elemento)*sizeof (char)) == 0)
    {
      return 0;
    }
  return 1;
}

int
verdospuntos (char *elemento)
{
  if (strncmp (elemento, "..", strlen(elemento) * sizeof (char)) == 0)
    {
      return 0;
    }
  return 1;
}

void
imprimirlista (char *userpath[], int size)
{
  for (int i = 0; i < size; i++)
    {
      if (verpunto (userpath[i]) == 1 && verdospuntos (userpath[i]) == 1)
	{


	  fprintf (stderr, "/%s", userpath[i]);
	}
    }
  fprintf (stderr, "\n");
}

void
liberarlista (char *userpath[], int size)
{
  for (size_t i = 0; i < size; i++)
    {
      free (userpath[i]);
    }
}

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      fprintf (stderr, "Clearpath usage: ./cleanpath path\n");
      exit (EXIT_SUCCESS);
    }
  char *pathtoclean = NULL;
  pathtoclean = adcwd (argv[1], pathtoclean);
  int npaths = counttokens (pathtoclean);
  char *listpath[npaths];
  tokenpath (listpath, pathtoclean);
  imprimirlista (listpath, npaths);
  free (pathtoclean);
  liberarlista (listpath, npaths);
  exit (EXIT_SUCCESS);
}
