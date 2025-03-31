#include <iostream>
#include <queue>
#include "Event.hh"
#include "Node.hh"
#include "Declarations.hh"
#include "utils.hh"
#include "log.hh"

using namespace std;

vector<Node> *nodes;
vector<int> mal_nodes;
float malicious;
clock_time avg_send, global_time, timeout_time;
int num_peers;
priority_queue<Event*, vector<Event*>, compare_events> event_queue;
vector<vector<double>> prop_delay;
unordered_map<int, unordered_map<int, clock_time>> prop_delay_m;
int ringmaster;
bool ringmaster_mining = false;
int malicious_block_cnt = 0;
int release_command_cnt = 0;
bool eclipse_attack;

int main(int argc, char **argv) {
    // 1. Process command line arguments
    if(argc != 6)
    {
        cout << "Usage: " << argv[0] << " " << "<num_of_peers> <malicious> <avg_send_time> <timeout_time> <eclipse>" << endl;
        return 1;
    }
    num_peers = atoi(argv[1]);
    malicious = atoi(argv[2]);
    avg_send = atof(argv[3]);
    timeout_time = atof(argv[4]);
    eclipse_attack = atoi(argv[5]);
    
    // 2. Create and initialize objects 
    prop_delay.resize(num_peers, vector<clock_time>(num_peers, 0));
    create_topology(num_peers);
    create_topology_m();
    ringmaster = mal_nodes[random_int(0, mal_nodes.size())];
    for(int i=0; i<nodes->size(); i++)
        event_queue.push(new TxnSendEvent(global_time + random_exp_float(avg_send), i));
    event_queue.push(new EndOfSimulationEvent(ringmaster));
    system("find log/* -type f -delete");   // clearing logs from previous execution 
    
    // 3. While Queue is not empty, do:
    //     3.1. pop first event from the queue
    //     3.2. process the first event
    while(!event_queue.empty())
    {
        Event *ev = event_queue.top();
        event_queue.pop();
        
        process_event(ev);
    }
    
    
    // 4. Log the result here.
    log_nodes();
    log_network_graph();
    log_topology();
    log_topology_m();
    log_mempools();
    log_blockchains();
    log_private_chains();
    log_blockchain_graphs();
    log_orphan_lists();
    log_statistics();
    cout << "Malicious nodes: " << mal_nodes << endl;
    cout << "Ringmaster Node: " << ringmaster << endl;
    cout << "Total " << Block::cnt << " blocks in the system." << endl;

    plot_graph("log/network-graph.gv", "network.png");
    plot_graph("log/blockchain-graph/node-"+to_string(ringmaster), "./ringmaster.png");
    
    return 0;
}

