#include <stdio.h>
#include "AMException.h"

void func() {
    puts("throwing 1");
    THROW (1);
}

void func2() {
    TRY {
        puts("inner try");
        func();
    } CATCH (2) {
        puts("This should not happen");
    } FINALLY {
        puts("inner finally");
    } TRY_END;
}

void func3() {
    puts("throwing 1");
    THROW (1, (void*)"Hello from func3!");
}

int main() {
    TRY {
        puts("try");
        func2();
        puts("This should not happen");
    } CATCH (1) {
        puts("catch 1");
    } CATCH_ALL {
        puts("This should not happen");
    } TRY_END;

    TRY {
        puts("try 2");
        puts("throwing 2");
        THROW (2);
    } CATCH (1) {
        puts("catch 1");
    } CATCH_ALL {
        puts("catch all");
    } FINALLY {
        puts("finally");
    } TRY_END;

    TRY {
        puts("try3");
        func3();
    } CATCH (1) {
        puts((char*)AMException.arg);
    } TRY_END;

    TRY {
        puts("try 4");
        puts("throwing 2");
        THROW (2);
    } CATCH (1) {
        puts("catch 1");
    } FINALLY {
        puts("finally");
    } TRY_END;

    puts("This should not happen");

    return 0;
}
