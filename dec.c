/*      
 *      C debugger
 *
 *       
 *
 */
#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>

void print_usage()
{
        puts("Usage: dec [EXECUTABLE NAME]");
}

int main( int argc, char *argv[] )
{
        if ( argc < 2 ) {
                print_usage();
                return -1;
        }

        char *exec_args[]       = { NULL };
        char *exec_environ[]    = { NULL };

        if ( execve( argv[1], exec_args, exec_environ ) == -1 )
                perror( "execve" );

        return 0;
}
