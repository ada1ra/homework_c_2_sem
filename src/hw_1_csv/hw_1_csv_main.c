#include "hw_1_csv.h"
#include <stdio.h>
#include <stdlib.h>

// ТЕСТЫ!!!!
int main()
{
    int rowsCount, colsCount;
    Row* rows = csvReading("input.csv", &rowsCount, &colsCount);
    if (!rows || rowsCount == 0) {
        printf("No data in input.csv\n");
        return 1;
    }

    int* maxWidth = maxColWidthFinding(rows, rowsCount, colsCount);

    FILE* outputFile = fopen("output.txt", "w");
    if (!outputFile) {
        printf("Cannot open output.txt\n");
        freeRows(rows, rowsCount, colsCount);
        free(maxWidth);
        return 1;
    }

    printTable(rows, rowsCount, colsCount, maxWidth, outputFile);

    fclose(outputFile);

    freeRows(rows, rowsCount, colsCount);
    free(maxWidth);

    return 0;
}