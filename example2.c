#include <stdio.h>
#include <stdlib.h>
#include "AMException.h"

enum EXCEPTION_CLASS {
    // 0 cannot be thrown
    MY_EXCEPTION = 1,
    OTHER_EXCEPTION
};

typedef struct {
    const char* msg;
    int numerator;
} MyException;

int checked_divide(int numerator, int denominator) {
    if (denominator == 0) {
        MyException *arg = malloc(sizeof(MyException));
        arg->msg = "Divide by zero. Numerator=%d.\n";
        arg->numerator = numerator;
        THROW_ARG(MY_EXCEPTION, (void*)arg);
    }
    return numerator / denominator;
}

int main() {
    TRY {
        checked_divide(1, 0);
    } CATCH(MY_EXCEPTION) {
        MyException *arg = (MyException*)AMException.arg;
        printf(arg->msg, arg->numerator);
        free(arg);
    } TRY_END;
}
