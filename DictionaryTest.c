//Matthew Atmadja
//matmadja
//PA5

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Dictionary.h"

int main(int argc, char * argv[]){
    //NEW DICTIONARY
    Dictionary test = newDictionary(5);
    fprintf(stderr, "Unique: %d\n", getUnique(test));
    fprintf(stderr, "\n");

    //INSERT TEST
    fprintf(stderr, "INSERT TEST\n");
    insert(test, "b", 1);
    insert(test, "d", 2);
    insert(test, "a", 3);
    insert(test, "e", 4);
    insert(test, "c", 5);
    fprintf(stderr, "Size: %d\n", size(test));
    printDictionary(stderr, test);
    fprintf(stderr, "\n");

    //UNIQUE SWITCH CHECK
    //insert(test, "a", 6);

    //DELETE TEST
    fprintf(stderr, "DELETE TEST\n");
    delete(test, "b");
    fprintf(stderr, "Size: %d\n", size(test));
    printDictionary(stderr, test);
    fprintf(stderr, "\n");

    //LOOKUP TEST
    fprintf(stderr, "LOOKUP TEST\n");
    VAL_TYPE x = lookup(test, "a");
    fprintf(stderr, "lookup(a): "VAL_FORMAT"\n", x);
    x = lookup(test, "b");
    fprintf(stderr, "lookup(b): "VAL_FORMAT"\n", x);
    x = lookup(test, "c");
    fprintf(stderr, "lookup(c): "VAL_FORMAT"\n", x);
    x = lookup(test, "d");
    fprintf(stderr, "lookup(d): "VAL_FORMAT"\n", x);
    x = lookup(test, "e");
    fprintf(stderr, "lookup(e): "VAL_FORMAT"\n", x);
    fprintf(stderr, "\n");

    //FORWARD ITERATION TEST
    fprintf(stderr, "FORWARD ITERATION TEST\n");
    x = beginForward(test);
    KEY_TYPE y = currentKey(test);
    fprintf(stderr, "Val: "VAL_FORMAT" Key: "KEY_FORMAT"\n", currentVal(test), y);

    x = next(test);
    y = currentKey(test);
    while(x != VAL_UNDEF){
        fprintf(stderr, "Val: "VAL_FORMAT" Key: "KEY_FORMAT"\n", x, y);
        x = next(test);
        y = currentKey(test);
    }
    fprintf(stderr, "\n");

    //REVERSE ITERATION TEST
    fprintf(stderr, "REVERSE ITERATION TEST\n");
    x = beginReverse(test);
    y = currentKey(test);
    fprintf(stderr, "Val: "VAL_FORMAT" Key: "KEY_FORMAT"\n", currentVal(test), y);

    x = prev(test);
    y = currentKey(test);
    while(x != VAL_UNDEF){
        fprintf(stderr, "Val: "VAL_FORMAT" Key: "KEY_FORMAT"\n", x, y);
        x = prev(test);
        y = currentKey(test);
    }
    fprintf(stderr, "\n");

    //EMPTY TEST
    fprintf(stderr, "EMPTY TEST\n");
    makeEmpty(test);
    fprintf(stderr, "Size: %d\n", size(test));

    //FREE TEST
    freeDictionary(&test);
}