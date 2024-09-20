#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define COUNT 2

int main(int ac, char **av, char **env)
{
	struct termios oldterminos, newterminos;

	if(tcgetattr(STDIN_FILENO, &oldterminos))
		perror("Problem on tcgetattr\n");
	printf("tcgetattr worked\n");
	newterminos = oldterminos;
	newterminos.c_lflag &= ~(ICANON | ECHO); 

	if(tcsetattr(STDIN_FILENO, TCSANOW, &newterminos))
		perror("problem on setattr\n");
	
	printf("input >\n");
	
	char ch;
	while(read(STDIN_FILENO, &ch, 1) > 0 && ch != 'q')
		printf("girdiniz = %c\n", ch);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldterminos);
	return (0);

}
