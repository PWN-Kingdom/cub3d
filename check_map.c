#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int		win_w;
int		win_h;
int		F;
int		C;
char	*NO;
char	*WE;
char	*EA;
char	*SO;
char	*S;

char    check_name(char *name)
{
    size_t  i;

    i = 0;
    while (name[i] != '\0')
        i++;
    if (i < 4)
        return (0);
    i -= 4;
    if (name[i] == '.' && name[i + 1] == 'm' &&
                name[i + 2] == 'a' && name[i + 3] == 'p')
        return (1);
	printf("File name error\n");
	return (0);
}

char	scip_trash(int file)
{
	char	buf;

	read(file, &buf, 1);
	while (buf == ' ' || buf == '\n' || buf == ',')
		if (read(file, &buf, 1) == 0)
			break;
	return (buf);
}

int		read_number(int file)
{
	char	buf;
	int		number;

	number = 0;
	buf = scip_trash(file);
	while (buf >= '0' && buf <= '9')
	{
		number = number * 10 + buf - '0';
		if (read(file, &buf, 1) == 0)
			break;
	}
	return (number);
}

int		read_color(int file)
{
	int		number;

	number = read_number(file) << 16;
	number += read_number(file) << 8;
	number += read_number(file);
	return (number);
}

char	*str_plus_char(char *src, char c)
{
	int		i;
	char	*array;

	i = 0;
	if (src != NULL)
	{
		while (src[i] != '\0')
			i++;
		if ((array = (char *)malloc(i + 2)) == NULL)
			return (NULL);
		i = 0;
		while (src[i] != '\0')
		{
				array[i] = src[i];
				i++;
		}
	}
	else
		if ((array = (char *)malloc(i + 2)) == NULL)
			return (NULL);
	array[i] = c;
	array[i + 1] = '\0';
	free(src);
	return (array);
}

char	*read_string(int file, char c)
{
	char	buf;
	char	*str;

	str = NULL;
	if (c != 0)
	{
		read(file, &buf, 1);
		if (buf != c)
			return (NULL);
	}
	buf = scip_trash(file);
	while (buf != ' ' && buf != '\n')
	{
		str = str_plus_char(str, buf);
		if (read(file, &buf, 1) == 0)
			break;
	}
	return str;
}

void	check_file(int file)
{
	char	buf;
	int		i;

	i = 0;
	while (i < 8)
	{
		buf = scip_trash(file);
		if (buf == 'R')						// R
		{
			win_w = read_number(file);
			win_h = read_number(file);
		}
		else if (buf == 'F')				// F
			F = read_color(file);
		else if (buf == 'C')				// C
			C = read_color(file);
		else if (buf == 'N')				// NO
			NO = read_string(file, 'O');
		else if (buf == 'W')				// WE
			WE = read_string(file, 'E');
		else if (buf == 'E')				// EA
			EA = read_string(file, 'A');
		else if (buf == 'S')
		{
			read(file, &buf, 1);
			if (buf == 'O')					// SO
				SO = read_string(file, 0);
			else if (buf == ' ')			// S
				S = read_string(file, 0);
		}
		else								// Error
		{
			printf("Error in parameters\n");
			break;
		}
		i++;
	}

	// Print param
	printf("\n\n~~~   PARAM   ~~~\n");
	printf("win_w %d\n", win_w);
	printf("win_h %d\n", win_h);
	printf("F = %#x\n", F);
	printf("C = %#x\n", C);
	printf("NO = %s\n", NO);
	printf("WE = %s\n", WE);
	printf("EA = %s\n", EA);
	printf("SO = %s\n", SO);
	printf("S = %s\n", S);
	printf("%c\n", buf);


	printf("\n\n	MAP\n");
	while (read(file, &buf, 1))
	{
		printf("%c", buf);
	}
}

int     main(int argc, char *argv[])
{
	int		file;

    if (argc == 2 && check_name(argv[1]))
    {
		file = open(argv[1], O_RDONLY);
		if (file == -1)
		{
			printf("Open failed\n");
			return (1);
		}
		check_file(file);
	}
	else
    {
        printf("Not two arguments");
    }
    return (0);
}
