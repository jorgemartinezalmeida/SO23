#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>

enum
{ Maxsize = 1024 + 1
};

struct lista
{
  char *dir;
  struct lista *sig;
};
void
checkarguments (int nargs, char *args[])
{
  if (nargs > 2)
    {
      err (1, "Usage: ./showpath (1 optional prefix)");
    }
}

int
counttokens ()
{
  int n = 0;
  char path[Maxsize] = "";

  if (getenv ("PATH") == NULL)
    {
      err (1, "Error getenv");
    }
  snprintf (path, Maxsize, "%s", getenv ("PATH"));
  char *rest = NULL;
  char *token;
  for (token = strtok_r (path, ":", &rest);
       token != NULL; token = strtok_r (NULL, ":", &rest))
    {
      n++;
    }
  return n;
}

int
tokenpath (char *list[])
{
  int n = 0;
  char path[Maxsize] = "";

  if (getenv ("PATH") == NULL)
    {
      err (1, "Error getenv");
    }
  snprintf (path, Maxsize, "%s", getenv ("PATH"));
  char *rest = NULL;
  char *token;
  for (token = strtok_r (path, ":", &rest);
       token != NULL; token = strtok_r (NULL, ":", &rest))
    {
      if ((list[n] =
	   (char *) malloc ((strlen (token) + 1) * sizeof (char))) == NULL)
	{
	  perror ("Error in malloc");
	  exit (EXIT_FAILURE);
	}
      snprintf (list[n], strlen (token) + 1, "%s", token);
      n++;
    }

  return n;
}

void
liberarmemoria (char *list[], int n)
{
  for (size_t i = 0; i < n + 1; i++)
    {
      fprintf (stderr, "%s\n", list[n]);
      free (list[n]);
    }
}

void
borrarlista (struct lista *cabeza)
{
  struct lista *anterior = cabeza;
  while (cabeza != NULL)
    {
      cabeza = cabeza->sig;
      free (anterior->dir);
      free (anterior);
      anterior = cabeza;
    }

}

struct lista *
insertar (struct lista *lista1, char *nuevoelemento)
{
  struct lista *nuevonodo;
  if ((nuevonodo = malloc (sizeof (struct lista))) == NULL)
    {
      borrarlista (lista1);
      perror ("Error in malloc");
      exit (EXIT_FAILURE);
    }
  if ((nuevonodo->dir =
       malloc ((strlen (nuevoelemento) + 1) * sizeof (char))) == NULL)
    {
      borrarlista (lista1);
      perror ("Error in malloc");
      exit (EXIT_FAILURE);
    }
  snprintf (nuevonodo->dir, (strlen (nuevoelemento) * sizeof (char) + 1),
	    "%s", nuevoelemento);
  nuevonodo->sig = lista1;
  return nuevonodo;
}

void
imprimirlista (struct lista *lista1)
{
  struct lista *listaaux = lista1;
  while (listaaux != NULL)
    {
      fprintf (stdout, "%s\n", listaaux->dir);
      listaaux = listaaux->sig;
    }

};

void
imprimirlistaconprefijo (struct lista *lista1, char prefix[])
{
  struct lista *listaaux = lista1;
  while (listaaux != NULL)
    {
      if (strstr (listaaux->dir, prefix) != NULL)
	{
	  if (strncmp (strstr (listaaux->dir, prefix), listaaux->dir, Maxsize)
	      == 0)
	    {
	      fprintf (stderr, "TIENE EL PREFIJO: ");
	      fprintf (stdout, "%s\n", listaaux->dir);
	    }
	}
      listaaux = listaaux->sig;
    }
  listaaux = lista1;

  while (listaaux != NULL)
    {
      if (strstr (listaaux->dir, prefix) == NULL)
	{
	  fprintf (stdout, "%s\n", listaaux->dir);
	}
      if (strstr (listaaux->dir, prefix) != NULL)
	{
	  if (strncmp (strstr (listaaux->dir, prefix), listaaux->dir, Maxsize)
	      != 0)
	    {
	      fprintf (stdout, "%s\n", listaaux->dir);
	    }
	}

      listaaux = listaaux->sig;
    }
};

int
main (int argc, char *argv[])
{
  int ntokens = 0;
  checkarguments (argc, argv);
  struct lista *listadirectorios;
  listadirectorios = NULL;
  ntokens = counttokens ();
  char *pathtokens[ntokens];
  tokenpath (pathtokens);
  for (int i = ntokens - 1; i > -1; i--)
    {
      listadirectorios = insertar (listadirectorios, pathtokens[i]);
      free (pathtokens[i]);

    }
  if (argc == 1)
    {
      imprimirlista (listadirectorios);
    }
  else if (argc == 2)
    {
      imprimirlistaconprefijo (listadirectorios, argv[1]);
    }
  borrarlista (listadirectorios);
  exit (EXIT_SUCCESS);
}
