/*
 *      Filename:       Logger.h
 *      Author:         Emanuel Aracena <earacenadev@gmail.com>
 *      Description:`   Interface of Logger class. Used for handling return values and error
 *                              codes.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger 
{
public:
        void log_error( std::string& function_name ) const;
        void set_caller_name( std::string caller_name );
        std::string get_caller_name() const;
private:
        // Name of object logger is attached to.
        std::string caller_name_;
};

#endif
