#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum
{ Maxsize = 1024 + 1,
  Tomayus = -32
};
int
main (int argc, char *argv[])
{
  char str[Maxsize] = "";
  char straux[Maxsize] = "";
  void mayusandreverse (char str2[Maxsize])
  {
    for (int j = strlen (getenv (str2)) - 1; j >= 0; j--)
      {
	snprintf (straux, Maxsize, "%c", getenv (str2)[j] + Tomayus);
	strncat (str, straux, Maxsize);
      }
  }
  for (int i = 1; i < argc; i++)
    {
      if (getenv (argv[i]) != NULL)
	{
	  mayusandreverse (argv[i]);
	}
    }
  if (strlen (str) != 0)
    {
      printf ("%s\n", str);
    }
  exit (EXIT_SUCCESS);
}
