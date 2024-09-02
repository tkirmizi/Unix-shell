#include <stdio.h>
#include <stdlib.h>

#define NUM_BUILTINS 7

typedef struct s_bin {
    char *builtin[NUM_BUILTINS];
} t_bin;

typedef struct s_ms {
    t_bin bin;
} t_ms;

void ft_set_builtin(t_ms *ms) {
    ms->bin.builtin[0] = "cd";
    ms->bin.builtin[1] = "pwd";
    ms->bin.builtin[2] = "echo";
    ms->bin.builtin[3] = "env";
    ms->bin.builtin[4] = "export";
    ms->bin.builtin[5] = "unset";
    ms->bin.builtin[6] = "exit";
}

int main() {
    t_ms ms;
    ft_set_builtin(&ms);

    // Dizi elemanlarını yazdırarak kontrol edelim
    for (int i = 0; i < NUM_BUILTINS; i++) {
        printf("%s\n", ms.bin.builtin[i]);
    }

    return 0;
}
