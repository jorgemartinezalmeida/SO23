#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
enum {
	Bufsize = 8 * 1024,
};
int
istxt(char d_name[256])
{
	if (strchr(d_name, '.') != NULL) {
		if (strncmp(strchr(d_name, '.'), ".txt", 4) == 0)
			return 1;
	}
	return 0;
}

void
filedestname(char d_name_origin[256], char d_name_dest[256])
{
	int last = strlen(d_name_origin) - 1;

	strncpy(d_name_dest, d_name_origin, 256);
	d_name_dest[last] = 'w';
	d_name_dest[last - 1] = 'o';
	d_name_dest[last - 2] = 'l';
}

void
procestxt(char d_name_origin[256])
{
	int fd_origin;
	int fd_dest;
	int nr;
	char buf[Bufsize] = "";
	char d_name_dest[256];

	filedestname(d_name_origin, d_name_dest);
	if ((fd_origin = open(d_name_origin, O_RDONLY)) < 0) {
		err(EXIT_FAILURE, "cannot open file");
	}
	if ((fd_dest =
	     open(d_name_dest, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0) {
		err(EXIT_FAILURE, "cannot open file");
	}

	while ((nr = read(fd_origin, buf, Bufsize)) != 0) {
		if (nr < 0) {
			err(EXIT_FAILURE, "can't read");
		}
		for (size_t i = 0; i < nr; i++) {
			buf[i] = tolower(buf[i]);
		}
		if (write(fd_dest, buf, nr) != nr) {
			err(EXIT_FAILURE, "can't write");
		}
	}
	close(fd_dest);
	close(fd_origin);
}

int
main(int argc, char const *argv[])
{
	DIR *d;
	struct dirent *ent;

	if (argc != 2) {
		errx(EXIT_FAILURE, " usage: %s dir", argv[0]);
	}
	if ((d = opendir(argv[1])) == NULL) {
		errx(EXIT_FAILURE, " opendir error: %s ", argv[1]);
	}
	while ((ent = readdir(d)) != NULL) {
		if (istxt(ent->d_name)) {
			procestxt(ent->d_name);
		}
	}
	closedir(d);
	exit(EXIT_FAILURE);
}
