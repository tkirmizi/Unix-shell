#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COUNT 2

int main(int ac, char **av, char **env)
{
	int	fds[4][2];
	int	i = 1; // ilk
	int	j = 0;
	int	k = 0;

	while (j < 4)
	{
		if (j != i-1)
			printf("close fds[%d][0]\n", j);
		j++;
	}
	while (k < 4)
	{
		if (k != i)
			printf("close [%d][1]\n", k);
		k++;
	}
}
