/*
 *      Filename:       main.cpp
 *      Author:         Emanuel Aracena Beriguete
 *      Description:    Main routine file.
 * 	    Date Created:	  September 9, 2018
 */

#include "Tracer.h"

// Print mini help listing.
void print_usage()
{
        std::cout << "./dec [EXECUTABLE NAME]" << std::endl;
}

int main( int argc, char *argv[] )
{
        
        if ( argc < 2 ) {
                print_usage();
                return -1;
        }
        
        std::string target = argv[ 1 ];
        Tracer tracer( "tracer",  target );
        
        int wait_status;
        
        pid_t pid = tracer.fork_process();
        
        if (pid == 0) {
                //Child process
                tracer.attach_me();
                tracer.execute_target();
        } else {
                // Parent process
                tracer.set_status( pid );
                wait_status = tracer.get_status();
                tracer.wait_for_signal();
                
                tracer.attach_process( pid );
                
                while ( !WIFEXITED( wait_status ) && !WIFSIGNALED( wait_status ) ) {
                        
                        tracer.read_registers( pid );
                        tracer.print_registers();
                        tracer.print_exit_process( wait_status );
                        tracer.single_step( pid );
                
                        wait_status = tracer.get_status();
                        tracer.wait_for_signal();
                }
        }
        
        return 0;
}
