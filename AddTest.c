#include <stdio.h>

int main(void)
{
    int num;

    printf("Please enter an integer: ");
    if (scanf("%d", &num) != 1) {
        printf("Input error, please enter a valid integer.\n");
        return 1;
    }

    if (num % 2 == 0) {
        printf("%d is even.\n", num);
    } else {
        printf("%d is not even.\n", num);
    }

    return 0;
}
