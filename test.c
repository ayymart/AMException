#include <stdio.h>
#include "AMException.h"

void func(void) {
    THROW (1);
}

int main(void) {
    TRY {
        puts("try");
        func();
        puts("bad");
    } CATCH (1) {
        puts("catch 1");
    } END_TRY;

    TRY {
        puts("try 2");
        THROW (2);
    } CATCH (1) {
        puts("catch 1");
    } CATCH_ALL {
        puts("catch all");
    } FINALLY {
        puts("finally");
    } END_TRY;

    TRY {
        puts("try 3");
        THROW (2);
    } CATCH (1) {
        puts("catch 1");
    } FINALLY {
        puts("finally");
    } END_TRY;

    return 0;
}