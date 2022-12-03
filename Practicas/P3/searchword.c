#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void
comprobarargumentos (int nargs)
{
  if (nargs % 2 == 0)
    {
      errx (EXIT_FAILURE,
	    "searchword usage: ./searchword file1 word1 file2 word2...\n");
    }
}

void
ejecutarfgrep (char *file, char *word)
{
  switch (fork ())
    {
    case 1:
      err (EXIT_FAILURE, "fork failed!");
    case 0:
      execl ("/bin/fgrep", "fgrep", "-H", "-n", word, file, NULL);
      err (EXIT_FAILURE, "Error in execl : ");
      break;

    }
}

int
main (int argc, char *argv[])
{
  comprobarargumentos (argc);
  for (int i = 1; i < argc; i = i + 2)
    {
      ejecutarfgrep (argv[i], argv[i + 1]);
    }
  exit (EXIT_SUCCESS);
}
