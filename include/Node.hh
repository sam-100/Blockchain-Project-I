#ifndef NODE_HH
#define NODE_HH

#include <vector>
#include <unordered_set>
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
    bool low;
    bool slow;
    bool mining;
    clock_time p_delay;
    std::unordered_set<int> peers;
    std::vector<currency> balance;
    std::unordered_set<int> visited_txns;
    std::list<Transaction*> mem_pool;
    Blockchain *blockchain;
    Block *genesis;
    
    static int cnt;

    Node(bool low, bool slow, Block *gen);
    void add_txn(Transaction *txn);
    void start_mining();
    bool visited(Transaction *txn) const;
    bool is_valid(Transaction *txn) const;
    void forward(Transaction *txn) const;
    void reset_mempool();

    Block *create_block();
    void add_block(Block *blk);
    void forward(Block *blk) const;

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
void mine_block(int n_id, Block* prev);
void block_recv(int n_id, Block *blk);
void forward_blk(int n_id, Block* blk);

void transaction_send(int n_id);
void transaction_recv(int n_id, Transaction *txn);

#endif
