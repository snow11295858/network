#include <stdio.h> /* perror()��ɬ�� */
#include <stdlib.h> /* exit()��ɬ�� */

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
