// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>

// char	*ft_strrchr(const char *s, int c)
// {
// 	unsigned int	i;
// 	char			*res;
// 	char			d;

// 	i = 0;
// 	d = c;
// 	res = NULL;
// 	while (s[i])
// 	{
// 		if (s[i] == d)
// 			res = ((char *) &s[i]);
// 		i++;
// 	}
// 	if (s[i] == d)
// 		res = ((char *) &s[i]);
// 	return (res);
// }

// int main(int argc, char *argv[])
// {
// 	char *pwd = "/Users/tkirmizi/my42projects/MINISHELL_ALL/minishell_area";
// 	char	*newpwd;
// 	newpwd = ft_strrchr(pwd, '/');
// 	printf("newpwd = %s\n", newpwd);
// }


#include <stdio.h>
#include <string.h>

int main() {
    char path[] = "/Users/tkirmizi/my42projects/MINISHELL_ALL/minishell_area";
    char *last_slash = strrchr(path, '/');  // Dizgedeki son slash'ı bulur

    if (last_slash != NULL) 
        *last_slash++ = '\0';  // Son slash'ın bulunduğu yere null karakter ekler

    printf("Son slash'a kadar olan kısım: %s\n", path);

    return 0;
}