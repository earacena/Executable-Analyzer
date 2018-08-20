/*      
 *      C debugger (DEC)
 *      Author: Emanuel Aracena <earacenadev@gmail.com>
 *      
 */
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <unistd.h>
#include <wait.h>

void attach_process( pid_t child )
{
        uint64_t result;
        result = ptrace( PTRACE_ATTACH, &child, NULL, NULL );
        if ( result < 0 ) {
                perror( "\n[-] ptrace (PTRACE_ATTACH)" );
                exit( EXIT_FAILURE );
        }
}

void attach_me( pid_t child )
{
       uint64_t result;
       result = ptrace( PTRACE_TRACEME, &child, NULL, NULL );
       if ( result < 0 ) {
                perror( "\n[-] ptrace (PTRACE_ATTACHME)" );
                exit( EXIT_FAILURE );
       }
}

void detach_process( pid_t child )
{
        uint64_t result;
        result = ptrace( PTRACE_DETACH, &child, NULL, NULL );
        if ( result < 0 ) {
                perror( "\n[-] ptrace (PTRACE_DETACH)" );
                exit( EXIT_FAILURE );
        }
}

void read_registers( pid_t pid, struct user_regs_struct *registers ) 
{
        uint64_t result;
        result = ptrace( PTRACE_GETREGS, &pid, &registers, NULL );
        if ( result < 0 ) {
                perror( "\n[-] ptrace (PTRACE_GETREGS) ");
                exit( EXIT_FAILURE );
        }
}

void print_registers( struct user_regs_struct *regs )
{
        printf( "RAX:\t0x%.8X\nRBX:\t0x%.8X\nRCX:\t0x%.8X\nRDX:\t0x%.8X\n",
                regs->rax, regs->rbx, regs->rcx, regs->rdx );

        printf("RSI:\t0x%.8X\nRDI:\t0x%.8X\n", regs->rsi, regs->rdi);
}

/*
 * void write_reg()
 * {}
 *
 * void read_mem()
 * {}
 * 
 * void write_mem()
 * {}
 */

void print_usage()
{
        puts(" Usage: dec [EXECUTABLE NAME]" );
}

int main( int argc, char *argv[] )
{
        if ( argc < 2 ) {
                print_usage();
                return -1;
        }
        
        struct user_regs_struct reg;
        int result = 0;
        pid_t pid;
        pid = fork();

        if ( pid < 0 ) {
                perror(" \n[-] fork" );
                exit( EXIT_SUCCESS );
        }

        if ( pid == 0 ) {
                // Child process
                attach_me( pid );
                result = execv( argv[1], NULL );
                if ( result < 0 ) {
                        perror( "\n[-] execv" );
                        exit( EXIT_FAILURE );
                }


        } else {
                // Parent process
                int status;
                wait(&status);
                attach_process( pid );
                read_registers( pid, &reg );
                print_registers( &reg );
                //detach_process( pid );
        }

        return 0;
}
