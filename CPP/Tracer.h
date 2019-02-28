/*
 *      Filename:       Tracer.h
 *      Author:         Emanuel Aracena <earacenadev@gmail.com>
 *      Description:`   Interface of Tracer class.
 */

#ifndef DEC_H
#define DEC_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/signal.h>

#include "Logger.h"

class Tracer 
{
public:
        /*              Parameterized Constructor
         * param:       String with name of execuatable and pid_t of Child process.
         * post:        Private members containing string of executable name are initialized to the
         *                      parameter values.
         */
        Tracer( std::string tracer_name, std::string& executable_name );
        
        /*              Forks the process into a Parent (tracer) and Child (tracee) process.
         * post:        Process forked into Parent process and Child process.
         * return:      Returns Child process pid of type pid_t.
         */
        pid_t fork_process() const;
        
        /*              Called by Parent process to attach and being tracing to Child process.
         * post:        Parent process attached to Child process.
         * return:      Returns false if there is a ptrace error, otherwise true.
         */
        void attach_process( const pid_t& child ) const;
        
        /*              Attaches tracee process to tracer process.
         * post:        Process is attached
         * return:      Returns false if there is a ptrace error, otherwise true.
         */
        void attach_me() const;
        
        /*              Detaches the tracee process from tracer process, and restarts tracee.
         * post:        Tracee proccess detached from tracer.
         * return:      Returns false if there is a ptrace error, otherwise true. 
         */
        void detach_process( const pid_t& child ) const;
        
        /*              Executes the target executable.
         * post:        Tracee process executes target.
         * return:      Returns false if there is execv error, otherwise true.
         */
        void execute_target() const;
        
        /*              Reads the values of the registers into a struct.
         * post:        Struct is populated with register values.
         * return:      Returns false if there is a ptrace error, otherwise true.
         */
        void read_registers( const pid_t& child ) const;
        
        /*              Prints register values.
         * post:        Register values are printed.
         */
        void print_registers() const;
        
        /*              Single-step a single instruction in tracee process.
         * post:        The tracee is started again for only one instruction then is stopped.
         * return:      Returns false if there is a ptrace error, otherwise true.
         */
        void single_step( const pid_t& child ) const;
        
        /*              Process the status of the tracee process and print status if applicable.
         * post:        Tracee status is printed if it has exited, stopped, killed, or continued.
         */
        void print_exit_process( int& wstatus ) const;
        
        /*              Waits for tracee to send signal.
         */
        void wait_for_signal();
        
        /*              Gets the Child process pid of type pid_t.
         * return:      Returns the child process pid of type pid_t.
         */
        pid_t get_child_pid() const;
        
        /*              Gets the status of tracee process.
         * return:      Returns the status code for the tracee process.
         */
        int get_status() const;
        
        /*              Sets the status of tracee process.
         * post:        Tracee status is set to argument.
         */
        void set_status( int& status );
        
        /*              Gets the name of the tracer.
         * return:      Returns the name of the tracer object in the form of a string.
         */
        std::string get_name() const;
                
private:
        // Status of tracee process
        int wstatus_;
        
        // Pid of Child process
        pid_t child_pid_;
        
        // Name of executable, used to execute program.
        std::string target_executable_;
        
        // Name of Tracer class object, used for logging purposes.
        std::string tracer_name_ = "Default Tracer";
        
        // Struct that holds format for tracee register information.
        struct user_regs_struct registers_;
        
        // Logger 
        Logger logger;
};

#endif
