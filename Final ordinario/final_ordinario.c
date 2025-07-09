#include studio.h
#include unistd.h
#include stdlib.h
#include pthread.h
#include signal.h

#define ROWS 4
#define COLS 3

int main()
{
    int matriz[ROWS][COLS] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10, 11, 12}};

    // Mostrar la matriz para verificar
    printf("Matriz de %dx%d:\n", ROWS, COLS);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }

    return 0;
}