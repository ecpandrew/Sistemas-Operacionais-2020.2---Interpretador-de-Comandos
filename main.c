#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>



char *gline() {
    char *buffer;

    const size_t step = 10;
    size_t size = step;

    if(!(buffer = malloc(size))) goto CLEANUP;

    char *s = buffer;

    // step-1 is not strictly necessary, but some implementations of fgets
    // are buggy. That's why it's step-1 instead of just step
    while(fgets(s, step-1, stdin)) {
        if(strchr(s, '\n')) break;
        size += step;
        if(!(buffer = realloc(buffer, size))) goto CLEANUP;
        s = &buffer[strlen(buffer)];
    }

    buffer[strcspn(buffer, "\n")] = 0;
    return buffer;

    CLEANUP:
    free(buffer);
    return NULL;
}


void tokenizer(char *str){

    char delim[] = " ";

    char *lista[10]; // use const because we're pointing to literals
    int index = 0;

    char *ptr = strtok(str, delim);

    while(ptr != NULL)
    {
        lista[index] = ptr;
        index++;
        ptr = strtok(NULL, delim);
    }


    for(int i = 0 ; i < index; i++)
    {
        printf("\n%s ", lista[i]);
    }



}



void* readCommand(void* p){

    char *s = gline();
    char *error = "error";

    if(!s){
        pthread_exit(&error);
    }else{
        pthread_exit(s);
    }
}

int main(void){
    // Declare variable for thread's ID:
    pthread_t id;

    int stop = 0;
    while (!stop){
        printf("\n> ");
        pthread_create(&id, NULL, readCommand, &stop);

        char* ptr[120];

        // Wait for foo() and retrieve value in ptr;
        pthread_join(id, (void**)&ptr);

        printf("\n>>>>>> %s\n", *ptr);

    }

}


void prinfHelp(){
    printf("------ Command Interpreter ------");
    printf("\nHELP");
    printf("\nCLEAR");
    printf("\nPWD");
    printf("\nCD <path>");
    printf("\nLS");
    printf("\nMORE <file>");
    printf("\nGREP <string>");
    printf("\n---------------------------------");
}
