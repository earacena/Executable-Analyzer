/*
 *      Filename:       Logger.cpp
 *      Author:         Emanuel Aracena
 *      Description:    Implementation of Logger class. Used for handling return values and error
 *                      codes.
 */

#include "Logger.h"

void Logger::log_error( std::string& function_name) const
{
        perror( function_name.c_str() );
}

void Logger::set_caller_name( std::string caller_name )
{
        caller_name_ = caller_name;
}

std::string Logger::get_caller_name() const
{
        return caller_name_;
}
