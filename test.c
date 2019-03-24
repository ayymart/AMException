#include <stdio.h>
#include "AMException.h"

void func(void) {
    THROW (1);
}

void func2(void) {
    TRY {
        puts("inner try");
        func();
    } CATCH (2) {
        puts("bad");
    } FINALLY {
        puts("finally");
    } END_TRY;
}

int main(void) {
    TRY {
        puts("try");
        func2();
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

    puts("This should not happen");

    return 0;
}