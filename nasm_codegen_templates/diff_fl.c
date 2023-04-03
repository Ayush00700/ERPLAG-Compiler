#include <stdio.h>
#include <inttypes.h>

double diff(double[], uint64_t);

int main() {
    double test[] = {
        40.5, 26.7, 21.9, 1.5, -40.5, -23.4
    };
    printf("%20.7f\n", diff(test, 2));
    return 0;
}