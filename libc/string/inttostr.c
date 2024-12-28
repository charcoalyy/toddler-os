#include <string.h>

void inttostr(int num, char* str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

	int len = 0;

    // track if negative sign needed
    if (num < 0) {
        str[0] = '-';
        len++;
        num = -1 * num;
    }

    // count number of digits
    int temp = num;
    while (temp != 0) {
        len++;
        temp /= 10;
    }

    // extract char from digits in reverse order
    str[len] = '\0';

    while (num != 0) {
        len--;
        str[len] = (num % 10) + '0';
        num /= 10;
    }
}
