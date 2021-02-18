//Matthew Atmadja
//matmadja
//PA5

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Dictionary.h"

int main(int argc, char * argv[]){
    //Condition Check
    if(argc != 3){
        fprintf(stderr, "Usage: Lex <file in> <file out>\n");
        exit(1);
    }

    //File Opening & File Check
    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");

    if(in == NULL){
        fprintf(stderr, "Unable to read from file %s\n", argv[1]);
        exit(1);
    }
    if(out == NULL){
        fprintf(stderr, "Unable to read from file %s\n", argv[2]);
        exit(1);
    }
    
    //Counting Number  of lines
    char c;
    int lines = 0;

    while((c = fgetc(in)) != EOF){
        if(c == '\n'){
            lines++;
        }
    }

    char** words;
    words = malloc(lines * sizeof(char*));
    
    for(int i = 0; i<lines; ++i){
        words[i] = malloc(300 * sizeof(char));
    }

    rewind(in);
    for(int j = 0; j<lines; ++j){
        fgets(words[j], 300, in);
    }
    
    Dictionary lex = newDictionary(0);

    for(int indexA = 0; indexA < lines; indexA++){
        insert(lex, words[indexA], indexA);
    }

    printDictionary(out, lex);

    freeDictionary(&lex);
    lex = NULL;

    for(int k = 0; k<lines; ++k){
        free(words[k]);
        words[k] = NULL;
    }
    free(words);
    words = NULL;

    //File Closing
    fclose(in);
    fclose(out);
}