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
    void start_mining();
    bool visited(Transaction *txn) const;
    bool is_valid(Transaction *txn) const;
    void broadcast(Transaction *txn) const;
    void send(Transaction *txn, int peer) const;
    void reset_mempool();

    Block *create_block();
    void add_block(Block *blk);
    void broadcast(Block *blk) const;
    void send(Block *blk, int peer) const;
    void mine_block(Block* prev);
    void block_recv(Block *blk);

    void broadcast(string hash) const;
    void send(string hash, int peer) const;
    void hash_recv(string hash, int sender);
    void timeout(string hash, int sender);
    void send_blk(int peer, string hash);

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


void transaction_send(int n_id);
void transaction_recv(int n_id, Transaction *txn);

#endif
