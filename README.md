# Executable-Analyzer
Command-line program that allows viewing the content of registers and memory allocated by executable.

## Author(s)

  Emanuel Aracena

## Date Created

(C) September 8, 2018
(C++) September 9, 2018

## Target Platform(s)

  Linux 64-bit

## Dependencies / Libraries / API / Languages / Frameworks

  C/C++, ptrace
 
## Description:

  The program allows the contents of the processor's registers and allocated memory addresses to be viewed during the execution of the "tracee" executable. Has a C version in top-level repo directory, and C++ version.

## Build instructions

  Go into src directory, and type: make
  
## Usage

  Take 64-bit Linux executable, denoted by "test", and type ./dec test
  
## Issues

* Undefined reference resulting from some distributions of Linux
* Stepping currently not working as intended
* Dumps the registers at the beginning/end of execution only
