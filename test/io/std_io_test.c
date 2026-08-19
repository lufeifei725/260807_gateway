#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{


    char input_name[20];
    fgets(input_name, 20, stdin);
    printf("%s", input_name);
    

    fputs("hello world\n", stdout);
    fputs("hello world\n", stderr);

    char input_name2[20];
    int len = read(STDIN_FILENO, input_name2, 20);
    printf("output %s len %d\n",input_name2, len);
    write(STDOUT_FILENO,input_name2 ,13);
    write(STDERR_FILENO,input_name2 ,13);


    return 0;
}
