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
    int r_cnt;
    clock_time p_delay;
    unordered_set<int> visited_msg;
    unordered_set<int> peers;
    unordered_set<int> peers_m;
    vector<currency> balance;
    unordered_set<int> visited_txns;
    unordered_map<string, list<int>> wait_list, wait_list_m;
    list<Transaction*> mem_pool;
    Blockchain *blockchain, *private_chain;
    Block *genesis;
    unordered_set<Block*> orphan_blocks, orphan_blocks_m;
    
    static int cnt;

    Node(bool malicious, Block *gen);

    /* Transaction management methods */
    void add_txn(Transaction *txn);
    bool visited(Transaction *txn) const;
    bool is_valid(Transaction *txn) const;
    void broadcast(Transaction *txn) const;
    void send(Transaction *txn, int peer) const;
    
    /* Block management methods */
    Block *create_block(Block *prev);
    void add_block(Block *blk);
    void add_block_m(Block *blk);
    void broadcast(Block *blk) const;
    void send(Block *blk, int peer) const;
    void send_m(Block *blk, int peer) const;
    void update_orphan_list();
    void update_orphan_list_m();
    
    /* Hash management methods */
    void broadcast(string hash) const;
    void broadcast_m(string hash) const;
    void send(string hash, int peer) const;
    void send_m(string hash, int peer) const;
    void request(string hash, int peer) const;
    void request_m(string hash, int peer) const;
    
    /* Event management methods */
    void txn_send_event();
    void txn_recv_event(Transaction *txn);
    void start_mining_event();
    void start_mining_event_m();
    void block_mined_event(Block* prev);
    void block_mined_event_m(Block* prev);
    void block_recv_event(Block *blk);
    void block_recv_event_m(Block *blk);
    void block_get_event(int peer, string hash);
    void block_get_event_m(int peer, string hash);
    void hash_recv_event(string hash, int sender);
    void hash_recv_event_m(string hash, int sender);
    void timeout_event(string hash, int sender);
    void timeout_event_m(string hash, int sender);
    void release_private_chain_event(int msg_no);
    
    bool can_mine() const;
    bool is_mining() const;
    void reset_mempool(Block *blk);
    clock_time get_latency(int peer, int size) const;
    clock_time get_latency_m(int peer, int size) const;
    double h_fraction() const;
    bool alert() const;

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
