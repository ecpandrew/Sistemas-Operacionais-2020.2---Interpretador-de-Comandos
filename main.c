#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>


int pwd();
int ls();
void printHelp();
void handlePipe(char str);
char *gline() {
    char *buffer;

    const size_t step = 10;
    size_t size = step;

    if(!(buffer = malloc(size))) goto CLEANUP;

    char *s = buffer;


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


#define READ 0
#define WRITE 1

int executePipe(char** args1,char** args2){
    int pipettes[2];
    pid_t p1, p2;

    //Chama o pipe
    if(pipe(pipettes) < 0){
        perror("simple_shell");
        return 0;
    }

    p1 = fork();
    if(p1 < 0){
        perror("simple_shell");
    }else if(p1 == 0){
        //Processo filho executando
        close(pipettes[READ]);
        dup2(pipettes[WRITE], STDOUT_FILENO);
        close(pipettes[WRITE]);

        if(execvp(args1[0], args1) < 0){
            perror("simple_shell");
        }
    }else{
        //Processo pai executando
        p2 = fork();

        if(p2 < 0){
            perror("simple_shell");
        }else if(p2 == 0){
            //Filho 2 executando
            close(pipettes[WRITE]);
            dup2(pipettes[READ], STDIN_FILENO);
            close(pipettes[READ]);
            if(execvp(args2[0], args2) < 0){
                perror("simple_shell");
                return 0;
            }else{
                return 1;
            }
        }else{
            //Pai executando
            wait(NULL);
        }
    }

    return 0;

}






int tokenizer(char *str){
    char copy_str[150];
    strcpy(copy_str, str);

    char delim[] = " ";
    char *lista[20]; // use const because we're pointing to literals

    int index = 0;

    char *ptr = strtok(str, delim);

    while(ptr != NULL)
    {
        lista[index] = ptr;
        index++;
        ptr = strtok(NULL, delim);
        if(ptr == NULL){
            lista[index] = NULL;
        }

    }

    int loop_status = 1;

    char* init_command = lista[0];
    char* args = lista[1];
    char* pipe = lista[2];
    char* command2 = lista[3];
    char* args2 = lista[4];


    char* second_command = strchr(copy_str, '|');

    if(second_command != NULL){
        char *comando1[20];
        char *comando2[20];

        int index1 = 0;
        int index2 = 0;


        char *ptr2 = strtok(copy_str, delim);

        while(ptr2 != NULL)
        {
            if (strcmp(ptr2, "|") == 0){
                ptr2 = strtok(NULL, delim);

                while(ptr2 != NULL){
                    comando2[index2] = ptr2;
                    index2++;
                    ptr2 = strtok(NULL, delim);

                }

                break;
            }
            comando1[index1] = ptr2;
            index1++;
            ptr2 = strtok(NULL, delim);

        }

        executePipe(comando1, comando2);
        loop_status = 1;

//        handlePipe([cd, ..], [ls])
//        handlePipe([cd, ..], [])

    }else{
        printf("\n not pipe");

        if(strcmp(init_command, "pwd") == 0){
            pwd();
            loop_status = 1;
        }else if(strcmp(init_command, "exit") == 0){
            loop_status = 0;

        }else if(strcmp(init_command, "help") == 0){
            printHelp();
            loop_status = 1;
        }else if(strcmp(init_command, "cd") == 0){
            if(args == NULL){
                printf("\n>>>> Expected argument <PATH> not found.");

            }else {
                chdir(args);
            }
        }else if(strcmp(init_command, "ls") == 0 |
                    strcmp(init_command, "grep") == 0 |
                    strcmp(init_command, "more") == 0){

                pid_t pid, wpid;
                pid = fork();
                int status;
                if(pid == 0){ // success
                    int temp = execvp(lista[0], lista);
                    if(temp ==-1){
                        printf("error");
                    }
                    exit(EXIT_FAILURE);
                }else if (pid < 0){
                    printf("error no fork");

                }
                else{
                    do{

                        wpid = waitpid(pid, &status, WUNTRACED);
                    }while(!WIFEXITED(status) && !WIFSIGNALED(status));

                }

            }else{
                printf("\n>>>> command '%s' not recognized by this interpreter",lista[0] );




        }

    }
    return loop_status;


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

int pwd(){
    char dir[450];
    getcwd(dir, 450);
    printf("\n>>>> %s\n", dir);
    return 1;
}

int print_current_dir(){
    char dir[450];

    getcwd(dir, 450);
    printf("\n %s > ", dir);
    return 1;
}

int main(void){
    // Declare variable for thread's ID:
    pthread_t id;

    int stop = 0;
    while (!stop){
        print_current_dir();


        pthread_create(&id, NULL, readCommand, &stop);

        char* command[120];

        // Wait for foo() and retrieve value in ptr;
        pthread_join(id, (void**)&command);

        if(strcmp(*command, "error") == 0){
            printf("\n>>>>>> Error: %s", *command);

        }else{
            int status = tokenizer(*command);
            if(status == 0){
                printf("\n>>>>>> Exiting Interpreter");
                break;
            }
        }


    }

    return 0;
}


void printHelp(){
    printf("\n------ Command Interpreter ------");
    printf("\nHELP");
    printf("\nEXIT");
    printf("\nPWD");
    printf("\nLS");
    printf("\nCD <path>");
    printf("\nMORE <file>");
    printf("\nGREP <string>");
    printf("\n---------------------------------");
}
