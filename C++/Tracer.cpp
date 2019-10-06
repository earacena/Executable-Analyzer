/*
 *      Filename:       Tracer.cpp
 *      Author:         Emanuel Aracena <earacenadev@gmail.com>
 *      Description:`   Implementation of Tracer class.
 */

#include "Tracer.h"

/*              Parameterized Constructor
 * param:       String with name of execuatable and pid_t of Child process.
 * post:        Private members containing string of executable name are initialized 
 *                      to the parameter values.
 */
Tracer::Tracer( std::string tracer_name, std::string& executable_name )
{
        std::cout << "TRACER:\tConstructing tracer..." << std::endl;
        tracer_name_ = tracer_name;
        target_executable_ = executable_name;
        logger.set_caller_name( tracer_name_ );
}

/*              Forks the process into a Parent (tracer) and Child (tracee) process.
 * post:        Process forked into Parent process and Child process.
 * return:      Returns Child process pid of type pid_t.
 */
pid_t Tracer::fork_process() const
{       
        std::cout << "MAIN:\tForking process..." << std::endl;
        std::string function_name = "fork";
        pid_t pid = fork();
        
        if ( pid < 0 )
                logger.log_error( function_name );
        
        return pid;
}

/*              Called by Parent process to attach and being tracing to Child process.
 * post:        Parent process attached to Child process.
 * return:      Returns false if there is a ptrace error, otherwise true.
 */
void Tracer::attach_process( const pid_t& child ) const
{
        std::cout << "TRACER:\tAttaching process..." << std::endl;
        std::string function_name = "ptrace [ATTACH_PROCESS]";
        if (ptrace( PTRACE_ATTACH, child, NULL, NULL )  < 0)
                logger.log_error( function_name );
}

/*              Attaches tracee process to tracer process.
 * post:        Process is attached
 * return:      Returns false if there is a ptrace error, otherwise true.
 */
void Tracer::attach_me() const
{
        std::cout << "TRACEE:\tAttaching to tracer..." << std::endl;
        std::string function_name = "ptrace [ATTACH_ME]";
        // Child proccess pid is zero in child process routine
        if ( ptrace( PTRACE_TRACEME, 0, 0, 0 )  < 0 ) 
                logger.log_error( function_name );
}

/*              Detaches the tracee process from tracer process, and restarts tracee.
 * post:        Tracee proccess detached from tracer.
 * return:      Returns false if there is a ptrace error, otherwise true. 
 */
void Tracer::detach_process( const pid_t& child ) const
{
        std::cout << "TRACER:\tDetaching process..." << std::endl;
        std::string function_name = "ptrace [DETACH_ME]";
        if ( ptrace( PTRACE_DETACH, child, NULL, NULL )  < 0 )
                logger.log_error( function_name );
}

/*              Executes the target executable.
 * post:        Tracee process executes target.
 * return:      Returns false if there is execv error, otherwise true.
 */
void Tracer::execute_target() const
{
        std::cout << "TRACEE:\tExecuting target executable..." << std::endl;
        std::string function_name = "execl [" + target_executable_ + "]";
        std::string path = "/home/nop/Work/Projects/dec/test_folder/test";
        if ( execl( path.c_str(), target_executable_.c_str(), NULL ) < 0 )
                logger.log_error( function_name );
}

/*              Reads the values of the registers into a struct.
 * post:        Struct is populated with register values.
 * return:      Returns false if there is a ptrace error, otherwise true.
 */
void Tracer::read_registers( const pid_t& child ) const
{
        std::cout << "TRACER:\tReading tracee registers..." << std::endl;
        std::string function_name = "ptrace [READ_REGISTERS]";
        if ( ptrace( PTRACE_GETREGS, child, registers_, NULL ) < 0 ) 
                logger.log_error( function_name );
}

/*              Prints register values.
 * post:        Register values are printed.
 */
void Tracer::print_registers() const
{
        std::cout << "TRACER: Registers:" << std::endl;
        std::cout << "\tRAX:\t" << std::hex << registers_.rax << std::endl
                  << "\tRBX:\t" << std::hex << registers_.rbx << std::endl
                  << "\tRCX:\t" << std::hex << registers_.rcx << std::endl
                  << "\tRDX:\t" << std::hex << registers_.rdx << std::endl
                  << "\tRSI:\t" << std::hex << registers_.rsi << std::endl
                  << "\tRDI:\t" << std::hex << registers_.rdi << std::endl
                  << "\tRIP:\t" << std::hex << registers_.rip << std::endl;
}

/*              Single-step a single instruction in tracee process.
 * post:        The tracee is started again for only one instruction then is stopped.
 * return:      Returns false if there is a ptrace error, otherwise true.
 */
void Tracer::single_step( const pid_t& child ) const
{
        std::cout << "TRACER:\tSingle stepping..." << std::endl;
        std::string function_name = "ptrace [SINGLESTEP]";
        if ( ptrace( PTRACE_SINGLESTEP, child, NULL, NULL )  < 0) 
                logger.log_error( function_name );
}

/*              Process the status of the tracee process and print status if applicable.
 * post:        Tracee status is printed if it has exited, stopped, killed, or continued.
 */
void Tracer::print_exit_process( int& wstatus ) const
{
        std::cout << "TRACER: Checking tracee status..." << std::endl;
        if ( WIFEXITED( wstatus ))
                std::cout << "\tExited, status= " << WEXITSTATUS( wstatus ) << std::endl;
        else if ( WIFSIGNALED( wstatus ) )
                std::cout << "\tKilled by signal " << WTERMSIG( wstatus ) << std::endl;
        else if ( WIFSTOPPED( wstatus ) )
                std::cout << "\tStopped by signal " << WSTOPSIG( wstatus ) << std::endl;
        else if ( WIFCONTINUED( wstatus ) )
                std::cout << "\tContinued" << std::endl;
}

/*              Waits for tracee to send signal.
 */
void Tracer::wait_for_signal()
{
        std::cout << "TRACER:\tWaiting for signal..." << std::endl;
        wait( &wstatus_ );
}

/*              Gets the Child process pid of type pid_t.
 * return:      Returns the child process pid of type pid_t.
 */
pid_t Tracer::get_child_pid() const
{
        return child_pid_;
}

/*              Gets the status of tracee process.
 * return:      Returns the status code for the tracee process.
 */
int Tracer::get_status() const
{
        return wstatus_;
}

/*              Sets the status of tracee process.
 * post:        Tracee status is set to argument.
 */
void Tracer::set_status( int& status )
{
        wstatus_ = status;
}

/*              Gets the name of the tracer.
 * return:      Returns the name of the tracer object in the form of a string.
 */
std::string Tracer::get_name() const
{
        return tracer_name_;
}
