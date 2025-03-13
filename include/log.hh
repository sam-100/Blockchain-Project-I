#ifndef LOG_HH
#define LOG_HH

#include "Declarations.hh"

void log_topology();
void log_nodes();
void log_mempools();
void log_blockchains();
void log_statistics();

void log(const char* file, const char* msg);
void log_event(Event *ev);
#endif