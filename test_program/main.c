#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __USE_MISC 1
#include <math.h>
#include "eval.h"

#define BUFF_SIZE 100

int main(int argc, char *argv[])
{
    printf("Welcome to REPL for Evaluate Mathematic Expression v%s\nType \".exit\" to exit, \".help\" to more information.\n", VERSION);

    char *input = (char *)calloc(BUFF_SIZE, sizeof(char));
    Constant_s **constants = (Constant_s **)malloc((argc + 2) * sizeof(Constant_s *));
    Status_s *status = (Status_s *)malloc(sizeof(Status_s));
    size_t constants_count = 0;

    if (constants == NULL || input == NULL || status == NULL)
    {
        puts("memory allocation error.");
        exit(1);
    }

    constants[constants_count++] = new_constant("e", M_E);
    constants[constants_count++] = new_constant("pi", M_PI);

    for (size_t i = 1; i < argc; i++)
    {
        char *current = argv[i];

        if (current[0] == '-') 
        {
            switch (current[1])
            {
            case 'd':
            {
                char *copy = (char *)malloc(strlen(argv[i] + 2) * sizeof(char)), *name;

                if (copy == NULL)
                {
                    puts("memory allocation error.");
                    exit(1);
                }

                strcpy(copy, argv[i] + 2);
                name = strtok(copy, "=");

                if (name == NULL)
                {
                    puts("invalid -d flag use.");
                    exit(1);
                }

                double value = atof(strtok(NULL, "="));

                constants[constants_count] = new_constant(name, value);
                constants[constants_count]->name = name;
                constants[constants_count]->value = value;

                constants_count++;
                break;
            }
            }
        }
    }

    constants[constants_count] = new_constant("", 0);

    while (1)
    {
        memset(input, 0, BUFF_SIZE);
        memset(status, 0, sizeof(Status_s));
        status->code = sc_ok;

        printf("> ");
        fgets(input, BUFF_SIZE, stdin);     

        if (strcmp(input, "\n") == 0)
            continue;

        if (input[0] == '.')
        {
            input[strlen(input) - 1] = '\0';

            if (strcmp(input + 1, "exit") == 0) break;
            else if (strcmp(input + 1, "help") == 0) printf("Git repository: https://github.com/DemetryF/evaluate-mathematic-expressions\n\n7 operators: +, -, /, *, %%, \\, ^.\nBrackets: \"()\"\nDouble numbers: 2.1\nFunctions: sin(12). use .funcs in REPL to check functions list.\n");
            else if (strcmp(input + 1, "funcs") == 0)
            {
                FILE *fptr = fopen("eval/functions_list.txt", "r");
                char *text = (char *)calloc(400, sizeof(char));

                if (fptr == NULL || text == NULL)
                {
                    printf("an error occurred while opening the file\n");
                    exit(1);
                }

                fread(text, 400, 200, fptr);
                fclose(fptr);
                printf("%s", text);
                free(text);
            }
            else printf("'%s' is not REPL command\n", input + 1);

            continue;
        }

        double result = eval(input, status, constants);

        switch (status->code)
        {
        case sc_unexped_token:
            printf("Unexped token '%c' at %ld\n", status->data3, status->data1 + 1);
            break;
        
        case sc_brackets_error:
            printf("Brackets error\n");
            break;

        case sc_is_not_defined:
            printf("'%s' is not defined\n", status->data2);
            break;

        case sc_is_not_a_function:
            printf("'%s' is not a function\n", status->data2);
            break;

        case sc_unexped_end_of_line:
            printf("Unexped end of input\n");
            break;

        case sc_ok:
            printf("%g\n", result);
            break;
        }
    }

    free(input);
    return 0;
}