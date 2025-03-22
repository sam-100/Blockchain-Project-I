#include <iostream>
#include <queue>
#include "Event.hh"
#include "Node.hh"
#include "Declarations.hh"
#include "utils.hh"
#include "log.hh"

using namespace std;

vector<Node> *nodes;
float slow, low;
clock_time avg_send;
clock_time global_time;
int num_peers;
priority_queue<Event*, vector<Event*>, compare_events> event_queue;
vector<vector<double>> prop_delay;

int main(int argc, char **argv) {
    // 1. Process command line arguments
    if(argc != 5)
    {
        cout << "Usage: " << argv[0] << " " << "<num_of_peers> <slow> <low> <avg_send>" << endl;
        return 1;
    }
    num_peers = atoi(argv[1]);
    low = atof(argv[2]);
    slow = atof(argv[3]);
    avg_send = atof(argv[4]);

    // 2. Create and initialize objects 
    prop_delay.resize(num_peers, vector<clock_time>(num_peers, 0));
    create_topology(num_peers);
    for(int i=0; i<nodes->size(); i++)
        event_queue.push(new TxnSendEvent(global_time + random_exp_float(avg_send), i));
    
    /* Logging the network information */
    
    // 3. While Queue is not empty, do:
    //     3.1. pop first event from the queue
    //     3.2. process the first event
    while(!event_queue.empty() && Block::cnt < STOP_PARAMETER)
    {
        Event *ev = event_queue.top();
        event_queue.pop();
        
        process_event(ev);
    }
    
    // 4. Log the result here.
    log_nodes();
    log_topology();
    log_mempools();
    log_blockchains();
    log_statistics();

    return 0;
}

