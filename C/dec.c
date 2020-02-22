/*      
 *      C process analyzer (dec)
 *      Author: Emanuel Aracena Beriguete
 */
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/syscall.h>
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

void attach_me()
{
        uint64_t result;
        result = ptrace( PTRACE_TRACEME, 0, 0, 0 );
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

void read_registers( pid_t child, struct user_regs_struct *registers ) 
{
        uint64_t result;
        result = ptrace( PTRACE_GETREGS, &child, &registers, NULL );
        if ( result < 0 ) {
                perror( "\n[-] ptrace (PTRACE_GETREGS) ");
                exit( EXIT_FAILURE );
        }
}

void print_registers( struct user_regs_struct *regs )
{/*
        printf( "[*]\tRAX:\t0x%.8X\n[*]\tRBX:\t0x%.8X\n[*]\tRCX:\t0x%.8X\n",
                regs->rax, regs->rbx, regs->rcx );
        
        printf("[*]\tRDX:\t0x%.8X\n[*]\tRSI:\t0x%.8X\n[*]\tRDI:\t0x%.8X\n", regs->rdx, 
               regs->rsi, regs->rdi);
               
 */
        printf( "[*]\tRIP:\t0x%.8X\n", regs->rip );
}


void single_step( pid_t child )
{
        uint64_t result;
        result = ptrace( PTRACE_SINGLESTEP, &child, NULL, NULL );
        if ( result < 0) {
                perror( "[-] ptrace (PTRACE_SINGLESTEP)" );
                exit( EXIT_FAILURE );
        }
}

void print_process_exit( int wstatus )
{
        if ( WIFEXITED( wstatus ))
                printf( "[-]\tExited, status=%d\n", WEXITSTATUS( wstatus ) );
        else if ( WIFSIGNALED( wstatus ) )
                printf( "[-]\tKilled by signal %d\n", WTERMSIG( wstatus ) );
        else if ( WIFSTOPPED( wstatus ) )
                printf( "[*]\tStopped by signal %d\n", WSTOPSIG( wstatus ) );
        else if ( WIFCONTINUED( wstatus ) )
                puts("[*] Continued");
        
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
        puts("[*] Forking...");
        pid = fork();
        
        if ( pid < 0 ) {
                perror(" \n[-] fork" );
                exit( EXIT_SUCCESS );
        }
        
        if ( pid == 0 ) {
                puts("[+] CHILD: routine started...");
                // Child process
                
                attach_me();
                
                puts("[+] CHILD: Executing target executable...");
                result = execl( "/bin/ls", "ls", NULL );
                if ( result < 0 ) {
                        perror( "\n[-] execl" );
                        exit( EXIT_FAILURE );
                }
                
                
        } else {
                puts("[+] PARENT: routine started...");
                
                // Parent process
                int status = pid;
                wait(&status);
                
                puts("[*] PARENT: Attaching process...");
                attach_process( pid );
                
                while ( !WIFEXITED( status ) && !WIFSIGNALED( status ) ) {
                        
                        
                        puts("[*] PARENT: Reading registers...");
                        read_registers( pid, &reg );
                        
                        puts("[*] PARENT: Printing registers...");
                        print_registers( &reg );
                
                        puts("[-] PARENT: Child process:");
                        print_process_exit( status );
                        
                        puts("[*] PARENT: Single stepping...");
                        single_step( pid );
                        
                        //printf( "Status: %d", status );
                
                        
                }
        }
        
        return 0;
}
