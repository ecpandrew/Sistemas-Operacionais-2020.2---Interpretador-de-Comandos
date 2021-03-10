#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define LINE_MAX_LENGTH 200



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


void readCommand(){
    printf("\n> ");
    char *s = gline();
    if(!s)
        puts("Error reading line");
    else{
        tokenizer(s);
    }
    free(s);
}

int main(int argc, char *argv[]) {

    int stop = 0;
//    char dir[120];
//    char command;
    printf("------ Command Interpreter ------");
    printf("\nHELP");
    printf("\nCLEAR");
    printf("\nPWD");
    printf("\nCD <path>");
    printf("\nLS");
    printf("\nMORE <file>");
    printf("\nGREP <string>");
    printf("\n---------------------------------");

    while (!stop){

        readCommand();

        stop = 1;
    }
    return 0;

}



//int main(int argc, char *argv[]) {
//
//    int number_of_threads = 2;
//    pthread_t threads[number_of_threads];
//    int status, i;
//
//    for (i=0; i < number_of_threads; i++){
//        printf("Main here. Creating thread %d", i);
//        status = pthread_create(&threads[i], NULL,print_hello_thread, (void *)i );
//
//        if(status != 0){
//            printf("Oops. pthread_create returned error code %d", status);
//            exit(-1);
//        }
//    }
//
//
//    exit(NULL);
//}

//int main(){
//    int p, f;
//    int rw_setup[2];
//    char message[20];
//
//    p = pipe(rw_setup);
//    if(p < 0){
//        printf("An error occured. Could not create the pipe.");
//        _exit(1);
//    }
//    f = fork();
//    if(f > 0){
//        write(rw_setup[1], "Hi from Parent", 15);
//    }
//    else if(f == 0){
//        int r_return = read(rw_setup[0],message,15);
//        printf("%s %d\n", message, r_return);
//    }
//    else{
//        printf("Could not create the child process");
//    }
//    return 0;
//
//}

