#ifndef LOG_HH
#define LOG_HH

#include "Declarations.hh"

void log_topology();
void log_topology_m();
void log_nodes();
void log_mempools();
void log_blockchains();
void log_private_chains();
void log_blockchain_graphs();
void log_statistics();
void log_network_graph();
void log_network_graph_m();

void log(const char* file, const char* msg);
void log_event(Event *ev);
void log_orphan_lists();


vector<int> eclipsed_nodes();
#endif