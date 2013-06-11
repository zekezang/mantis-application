/*
 * main.c
 *
 *  Created on: 2013-5-8
 *      Author: zekezang
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>

/**
 * wpa_config_get_line - Read the next configuration file line
 * @s: Buffer for the line
 * @size: The buffer length
 * @stream: File stream to read from
 * @line: Pointer to a variable storing the file line number
 * @_pos: Buffer for the pointer to the beginning of data on the text line or
 * %NULL if not needed (returned value used instead)
 * Returns: Pointer to the beginning of data on the text line or %NULL if no
 * more text lines are available.
 *
 * This function reads the next non-empty line from the configuration file and
 * removes comments. The returned string is guaranteed to be null-terminated.
 */
static char * get_line(char *s, int size, FILE *stream, int *line, char **_pos) {
	char *pos, *end, *sstart;

	while (fgets(s, size, stream)) {
		(*line)++;
		s[size - 1] = '\0';
		pos = s;

		/* Skip white space from the beginning of line. */
		while (*pos == ' ' || *pos == '\t' || *pos == '\r')
			pos++;

		/* Skip comment lines and empty lines */
		if (*pos == '#' || *pos == '\n' || *pos == '\0')
			continue;

		/*
		 * Remove # comments unless they are within a double quoted
		 * string.
		 */
		sstart = strchr(pos, '"');
		if (sstart)
			sstart = strrchr(sstart + 1, '"');
		if (!sstart)
			sstart = pos;
		end = strchr(sstart, '#');
		if (end)
			*end-- = '\0';
		else
			end = pos + strlen(pos) - 1;

		/* Remove trailing white space. */
		while (end > pos && (*end == '\n' || *end == ' ' || *end == '\t' || *end == '\r'))
			*end-- = '\0';

		if (*pos == '\0')
			continue;

		if (_pos)
			*_pos = pos;
		return pos;
	}

	if (_pos)
		*_pos = NULL;
	return NULL;
}

int main(int argc, char **argv) {

	char buf[256], *pos;
	int line = 0;
	FILE *f = fopen("data", "r+");

	char *s = get_line(buf, 256, f, &line, &pos);
	printf("%s---line:%d---value:%s\n", s, line, pos);

	s = get_line(buf, 256, f, &line, &pos);
	printf("%s---line:%d---value:%s\n", s, line, pos);

	s = get_line(buf, 256, f, &line, &pos);
	printf("%s---line:%d---value:%s\n", s, line, pos);

	return 0;
}

