#include <stdio.h>

int ahoj(char *arr)
{
	*(arr + 1) = 'x';
	return 0;
}

int main(int argc, char *argv[])
{
	char arr[1][2];
	arr[0][0] = 'F';
	arr[0][1] = 'G';
	printf("%d %s\n", ahoj(arr[0]), arr); /** Vytiskne: 0 Fx **/

	return 0;

}
