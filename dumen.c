#include <unistd.h>

int main()
{
	int	i;

	i = 0;
	char	*s = "exit";
	while (s[i])
		write(STDERR_FILENO, &s[i++], 1);
	return 0;
}