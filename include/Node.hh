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
    std::vector<currency> balance;
    std::unordered_set<int> visited_txns;
    std::unordered_map<string, list<int>> wait_list;
    std::list<Transaction*> mem_pool;
    Blockchain *blockchain;
    Block *genesis;
    
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

    friend ostream &operator<<(ostream &out, const Node &node);
};

void create_topology(int);
void clear_network();
bool is_connected();
void print_network();
void connect_nodes(int node_this, int node_that);
void visit_dfs(int curr, vector<bool> &visited);
void print_edges();



#endif
