#ifndef NODE_HH
#define NODE_HH

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <ostream>
#include <list>
#include "Declarations.hh"
#include "Blockchain.hh"

using namespace std;

#define MIN_DEGREE 3
#define MAX_DEGREE 6

struct Node
{
    int id;
    bool malicious;
    bool mining;
    clock_time p_delay;
    std::unordered_set<int> peers;
    std::unordered_set<int> peers_m;
    std::vector<currency> balance;
    std::unordered_set<int> visited_txns;
    std::unordered_map<string, list<int>> wait_list;
    std::list<Transaction*> mem_pool;
    Blockchain *blockchain;
    Block *genesis;
    Block *private_chain;
    unordered_set<Block*> orphan_blocks;
    
    static int cnt;

    Node(bool malicious, Block *gen);
    void add_txn(Transaction *txn);
    bool visited(Transaction *txn) const;
    bool is_valid(Transaction *txn) const;
    void broadcast(Transaction *txn) const;
    void send(Transaction *txn, int peer) const;
    
    
    Block *create_block();
    void add_block(Block *blk);
    void broadcast(Block *blk) const;
    void send(Block *blk, int peer) const;
    void update_orphan_list();
    
    void broadcast(string hash) const;
    void send(string hash, int peer) const;
    void request(string hash, int peer) const;
    
    void txn_send_event();
    void txn_recv_event(Transaction *txn);
    void mine_block_event(Block* prev);
    void block_recv_event(Block *blk);
    void block_get_event(int peer, string hash);
    void hash_recv_event(string hash, int sender);
    void timeout_event(string hash, int sender);
    
    void start_mining();
    void reset_mempool();
    clock_time get_latency(int peer, int size) const;
    double h_fraction() const;

    void mine_block_event_m(Block *prev);

    friend ostream &operator<<(ostream &out, const Node &node);
};

void create_topology(int);
int select_peer(Node &node);
void clear_network();
bool is_connected();
void print_network();
void connect_nodes(int node_this, int node_that);
void visit_dfs(int curr, vector<bool> &visited);
void print_edges();

void create_topology_m();
int select_peer_m(Node &node);
void clear_network_m();
bool is_connected_m();
void visit_dfs_m(int, unordered_set<int>&);
void connect_nodes_m(int i, int j);


#endif
