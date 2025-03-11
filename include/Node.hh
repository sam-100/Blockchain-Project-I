#ifndef NODE_HH
#define NODE_HH

#include "Declarations.hh"
#include <vector>
#include <unordered_set>
#include <ostream>

using namespace std;

#define MIN_DEGREE 3
#define MAX_DEGREE 6

extern std::vector<Node> nodes;
extern float low;
extern float slow;

struct Node
{
    int id;
    bool low;
    bool slow;
    std::unordered_set<int> peers;
    std::vector<currency> balance;
    std::unordered_set<int> visited_txns;
    std::vector<Transaction*> mem_pool;
    static int cnt;

    Node(bool low, bool slow);
    friend ostream &operator<<(ostream &out, const Node &node);
};

void create_topology(int);
void clear_network();
bool is_connected();
void print_network();
void connect_nodes(int node_this, int node_that);
void visit_dfs(int curr, vector<bool> &visited);
void print_edges();
void forward_txn(int n_id, Transaction *txn);

#endif
