#include "hw_8_countries.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, m;
    printf("Введите количество городов и количество дорог: ");
    scanf("%d%d", &n, &m);
    createGraph(n, m);

    printf("Введите %d дорог в формате: u v w (где u и v — номера городов, w — длина)\n", m);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        addRoad(u, v, w);
    }

    int k;
    printf("Введите количество столиц (государств): ");
    scanf("%d", &k);

    int* capitals = malloc(k * sizeof(int));
    printf("Введите %d номеров столиц через пробел: ", k);
    for (int i = 0; i < k; i++)
        scanf("%d", &capitals[i]);

    runDistribution(k, capitals);

    for (int i = 1; i <= k; i++) {
        int cnt;
        int* cities = getStateCities(i, &cnt);
        
        printf("Государство %d:", i);
        for (int j = 0; j < cnt; j++)
            printf(" %d", cities[j]);
        printf("\n");
    }

    free(capitals);
    freeMemory();
    return 0;
}