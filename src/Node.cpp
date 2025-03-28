#include "Node.hh"
#include "utils.hh"
#include "Transaction.hh"
#include "Event.hh"
#include "Block.hh"
#include "log.hh"
#include <iostream>
#include <math.h>

using namespace std;


int Node::cnt = 0;

Node::Node(bool malicious, Block *gen) {
    id = cnt++;
    this->malicious = malicious;
    this->mining = false;
    balance = vector<currency>(num_peers, 100);
    genesis = gen;
    blockchain = new Blockchain(genesis);
    private_chain = new Blockchain();
}


/*
    >|----------------------Transaction management methods---------------------|<
*/
void Node::add_txn(Transaction *txn) {
    mem_pool.push_back(txn);
    balance[txn->sender] -= txn->amount;
    balance[txn->receiver] += txn->amount;
    visited_txns.insert(txn->id);
    return;
}

bool Node::visited(Transaction *txn) const {
    return visited_txns.find(txn->id) != visited_txns.end();
}

bool Node::is_valid(Transaction *txn) const {
    return balance[txn->sender] >= txn->amount;
}

void Node::broadcast(Transaction *txn) const {
    for(int peer : peers)
        send(txn, peer);
}

void Node::send(Transaction *txn, int peer) const {
    clock_time latency = get_latency(peer, pow(2, 10)*8);
    event_queue.push(new TxnRecvEvent(global_time + latency, id, txn));
}


/*
    >|----------------------Block management methods---------------------|<
*/
Block *Node::create_block(Block *prev) {
    Block *blk = new Block(prev, id, malicious);
    for(int i=0; i<BLOCK_SIZE; i++)
    {
        blk->add_txn(mem_pool.front());
        mem_pool.pop_front();
    }
    return blk;
}

void Node::add_block(Block *blk) {
    blockchain->insert(blk);
    if(blk == blockchain->get_last_blk())
        reset_mempool(blk);
    wait_list.erase(blk->get_hash());
}

void Node::add_block_m(Block *blk) {
    private_chain->insert(blk);
    reset_mempool(blk);
    wait_list_m.erase(blk->get_hash());
}

void Node::broadcast(Block *blk) const {
    for(int peer : peers)
        send(blk, peer);
}

void Node::send(Block *blk, int peer) const {
    clock_time latency = get_latency(peer, pow(2, 20)*8);
    event_queue.push(new BlockRecvEvent(global_time + latency, peer, blk));
}

void Node::send_m(Block *blk, int peer) const {
    clock_time latency = get_latency_m(peer, pow(2, 20)*8);
    event_queue.push(new BlockRecvEvent_M(global_time + latency, peer, blk));
}


/*
    >|----------------------Hash management methods---------------------|<
*/
void Node::broadcast(string hash) const {
    for(int peer : peers)
        send(hash, peer);
    for(int peer : peers_m)
        send(hash, peer);
}

void Node::broadcast_m(string hash) const {
    for(int peer : peers_m)
        send_m(hash, peer);
}

void Node::send(string hash, int peer) const {
    clock_time latency = get_latency(peer, 64);
    event_queue.push(new HashRecvEvent(global_time + latency, peer, hash, id));
}

void Node::send_m(string hash, int peer) const {
    clock_time latency = get_latency_m(peer, 64);
    event_queue.push(new HashRecvEvent_M(global_time + latency, peer, hash, id));
}

void Node::request(string hash, int sender) const {
    clock_time latency = get_latency(sender, 64);
    event_queue.push(new BlockGetReqEvent(global_time + latency, sender, hash, id));        // sending "get" request to the sender node
    event_queue.push(new TimeOutEvent(global_time + timeout_time, id, hash, sender));       // adding timeout time
}

void Node::request_m(string hash, int sender) const {
    clock_time latency = get_latency_m(sender, 64);
    event_queue.push(new BlockGetReqEvent_M(global_time + latency, sender, hash, id));        // sending "get" request to the sender node
    event_queue.push(new TimeOutEvent_M(global_time + timeout_time, id, hash, sender));       // adding timeout time
}


/*
    >|----------------------Event management methods---------------------|<
*/
void Node::txn_send_event() {
    float bal = balance[id];
    Transaction *txn = new Transaction(id, random_int(0, nodes->size()), random_float(0, bal));
    txn_recv_event(txn);
}

void Node::txn_recv_event(Transaction *txn) {
    if(!is_valid(txn) || visited(txn))
        return;
    add_txn(txn);
    broadcast(txn);
}

void Node::start_mining_event() {
    if(mining == true)
        return;
    mining = true;
    clock_time mining_time = random_exp_float(BLOCK_INTV_TIME/h_fraction());
    event_queue.push(new BlockMinedEvent(global_time+mining_time, id, blockchain->get_last_blk()));
}

void Node::start_mining_event_m() {
    if(ringmaster_mining == true)
        return;
    ringmaster_mining = true;
    clock_time mining_time = random_exp_float(BLOCK_INTV_TIME/h_fraction());
    Block *prev = private_chain->empty() ? blockchain->get_last_blk() : private_chain->get_last_blk();
    event_queue.push(new BlockMinedEvent_M(global_time+mining_time, id, prev));
    // cout << "Malicious node-" << id << " started mining on block " << prev->id << endl;
}

void Node::block_mined_event(Block *prev) {
    if(blockchain->get_last_blk() != prev)
    {
        mining = false;
        return;
    }

    Block *blk = create_block(prev);
    block_recv_event(blk);
    mining = false;
}

void Node::block_mined_event_m(Block *prev) {
    if(!private_chain->empty() && private_chain->get_last_blk() != prev)
    {
        ringmaster_mining = false;
        return;
    }

    Block *blk = create_block(prev);
    block_recv_event_m(blk);
    ringmaster_mining = false;
    // cout << "Malicious node-" << id << " successfully mined block " << blk->id << endl;
}

void Node::block_recv_event(Block *blk) {    
    if(blockchain->contains(blk))
    {
        // mining = false;
        return;
    }

    if(!blockchain->contains(blk->prev_blk))
    {
        orphan_blocks.insert(blk);
        // mining = false;
        return;
    }

    if(wait_list.find(blk->get_hash()) != wait_list.end())
        wait_list.erase(blk->get_hash());
    add_block(blk);
    update_orphan_list();
    broadcast(blk->get_hash());
    return;
}

void Node::block_recv_event_m(Block *blk) {
    if(private_chain->contains(blk) || blockchain->contains(blk))
        return;

    if(!private_chain->contains(blk->prev_blk) && !blockchain->contains(blk->prev_blk))
    {
        orphan_blocks_m.insert(blk);
        return;
    }

    add_block_m(blk);

    update_orphan_list_m();
    broadcast_m(blk->get_hash());
    return;
}

void Node::update_orphan_list() {
    while(true)
    {
        // 1. select a block from orphan list who's prev is in main blockchain
        Block *orphan = nullptr;
        for(Block *blk : orphan_blocks)
        {
            if(blockchain->contains(blk->prev_blk))
            {
                orphan = blk;
                break;
            }
        }
        if(orphan == nullptr)
            return;

        // 2. pop that block and insert it into the main blockchain
        orphan_blocks.erase(orphan);
        add_block(orphan);
    }
}

void Node::update_orphan_list_m() {
    while(true)
    {
        Block *orphan = nullptr;
        for(Block *blk : orphan_blocks_m)
        {
            if(private_chain->get_last_blk() == blk->prev_blk)
            {
                orphan = blk;
                break;
            }
        }
        if(orphan == nullptr)
            return;
        
        orphan_blocks_m.erase(orphan);
        add_block_m(orphan);
    }
}

void Node::block_get_event(int sender, string hash) {
    if(malicious && !nodes->at(sender).malicious)
        return;
    send(blockchain->get_blk(hash), sender);
}

void Node::block_get_event_m(int sender, string hash) {
    send_m(private_chain->get_blk(hash), sender);
}

void Node::hash_recv_event(string hash, int sender) {
    // 1. If the corresponding block is received already, discard and return.
    if(blockchain->contains_hash(hash))
        return;

    // 2. If the time-out corresponding to the hash is running, add the node to pending_requests.
    if(wait_list.find(hash) != wait_list.end())
    {
        wait_list[hash].push_back(sender);
        return;
    }

    // 3. Else - add the hash to pending requests and send the BlockRecv request
    request(hash, sender);
    wait_list[hash].push_back(sender);
}

void Node::hash_recv_event_m(string hash, int sender) {
    // 1. If the corresponding block is received already, discard and return.
    if(blockchain->contains_hash(hash) || private_chain->contains_hash(hash))
        return;

    // 2. If the time-out corresponding to the hash is running, add the node to pending_requests.
    if(wait_list_m.find(hash) != wait_list_m.end())
    {
        wait_list_m[hash].push_back(sender);
        return;
    }

    // 3. Else - add the hash to pending requests and send the BlockRecv request
    request_m(hash, sender);
    wait_list_m[hash].push_back(sender);
}

void Node::timeout_event(string hash, int sender) {
    // 1. If block is received, then erase the queue and return.
    if(blockchain->contains_hash(hash))         
    {
        wait_list.erase(hash);
        return;
    }
    
    // 2. Pop the next peer from queue, and send it the get request
    if(wait_list[hash].empty())
    {
        wait_list.erase(hash);
        return;
    }
    int next = wait_list[hash].front();
    wait_list[hash].pop_front();
    request(hash, next);
}

void Node::timeout_event_m(string hash, int sender) {
    // 1. If block is received, then erase the queue and return.
    if(blockchain->contains_hash(hash) || private_chain->contains_hash(hash))         
    {
        wait_list_m.erase(hash);
        return;
    }
    
    // 2. Pop the next peer from queue, and send it the get request
    if(wait_list_m[hash].empty())
    {
        wait_list_m.erase(hash);
        return;
    }
    int next = wait_list_m[hash].front();
    wait_list_m[hash].pop_front();
    request_m(hash, next);
}


/*
    >|------------------------Miscelleneous methods----------------------|<
*/
bool Node::is_mining() const {
    if(malicious)
        return ringmaster_mining;
    return mining;
}

bool Node::can_mine() const {
    if(mem_pool.size() < BLOCK_SIZE)
        return false;
    if(malicious)
        return ringmaster_mining == false;
    return mining == false;
}

void Node::reset_mempool(Block *blk) {
    balance = blk->balance;
    list<Transaction*> buffer = mem_pool;
    mem_pool.clear();
    for(Transaction *txn : buffer)
        if(is_valid(txn))
            add_txn(txn);
}

clock_time Node::get_latency(int peer, int size) const {
    clock_time pd = prop_delay[id][peer];
    double link_speed = ((nodes->at(peer).malicious && malicious) ? 100 : 5)*pow(2, 20);
    clock_time q_delay = random_exp_float(96*pow(2, 10)/link_speed);
    return pd + size/link_speed + q_delay;
}

clock_time Node::get_latency_m(int peer, int size) const {
    clock_time pd = prop_delay_m[id][peer];
    double link_speed = ((nodes->at(peer).malicious && malicious) ? 100 : 5)*pow(2, 20);
    clock_time q_delay = random_exp_float(96*pow(2, 10)/link_speed);
    return pd + size/link_speed + q_delay;
}


double Node::h_fraction() const {
    if(malicious)
        return (double)mal_nodes.size()/(double)num_peers;
    return (double)1/(double)num_peers;
}




/* Network control and other functions */
void create_topology(int node_cnt) {
    nodes = new vector<Node>();
    Block *genesis = new Block(nullptr, -1);
    for(int i=0; i<node_cnt; i++)
    {
        Node node(random_int(0, 100) < malicious, genesis);
        nodes->push_back(node);
    }
    
    while(!is_connected())
    {
        clear_network();
        
        for(Node node : *nodes)
        {
            if(node.peers.size() >= MIN_DEGREE)
                continue;
            int peer = select_peer(node);
            connect_nodes(node.id, peer);
        }
    }
}

int select_peer(Node &node) {
    int peer;
    bool degree_max, is_self;
    do {
        peer = random_int(0, nodes->size());
        degree_max = nodes->at(peer).peers.size() >= MAX_DEGREE;
        is_self = peer == node.id;
    } while(degree_max || is_self);
    return peer;
}

int select_peer_m(Node &node) {
    int peer;
    bool degree_max, is_self, is_malicious;
    do {
        peer = random_int(0, nodes->size());
        degree_max = nodes->at(peer).peers_m.size() >= MAX_DEGREE;
        is_self = peer == node.id;
        is_malicious = nodes->at(peer).malicious;
    } while(degree_max || is_self || !is_malicious);
    return peer;
}


void create_topology_m() {    
    for(Node node : *nodes)
        if(node.malicious)
            mal_nodes.push_back(node.id);
    cout << "malicious nodes: " << mal_nodes << endl;

    while(!is_connected_m())
    {
        clear_network_m();
        
        for(Node node : *nodes)
        {
            if(node.malicious == false)
                continue;
            if(node.peers_m.size() >= MIN_DEGREE)
                continue;

            int peer = select_peer_m(node);
            connect_nodes_m(node.id, peer);
        }
    }
}

bool is_connected() {
    vector<bool> visited(nodes->size(), false);

    visit_dfs(0, visited);
    
    for(int i=0; i<visited.size(); i++)
        if(visited[i] == false)
            return false;
    return true;
}

bool is_connected_m() {
    unordered_set<int> visited;    
    visit_dfs_m(mal_nodes[0], visited);
    if(visited.size() == mal_nodes.size())
        return true;
    return false;
}

void visit_dfs(int curr, vector<bool> &visited) {
    visited[curr] = true;
    for(int peer : nodes->at(curr).peers)
        if(!visited[peer])
            visit_dfs(peer, visited);
    return;
}

void visit_dfs_m(int curr, unordered_set<int> &visited) {
    visited.insert(curr);
    for(int peer : nodes->at(curr).peers_m)
        if(visited.find(peer) == visited.end())
            visit_dfs_m(peer, visited);
    return;
}

void clear_network() {
    for(int i=0; i<nodes->size(); i++)
        nodes->at(i).peers.clear();
}

void clear_network_m() {
    for(int i=0; i<nodes->size(); i++)
        nodes->at(i).peers_m.clear();
}

void connect_nodes(int i, int j) {
    nodes->at(i).peers.insert(j);
    nodes->at(j).peers.insert(i);
    
    clock_time pd = random_float(0.010, 0.500);
    prop_delay[i][j] = prop_delay[j][i] = pd;
}

void connect_nodes_m(int i, int j) {
    nodes->at(i).peers_m.insert(j);
    nodes->at(j).peers_m.insert(i);
    
    clock_time pd = random_float(0.001, 0.010);
    prop_delay_m[i][j] = prop_delay_m[j][i] = pd;
}


void print_network() {
    for(Node node : *nodes) {
        cout << node.id << ": ";
        for(int peer : node.peers)
            cout << peer << " | ";
        cout << endl;
    }
}

void print_edges() {
    for(Node node : *nodes)
        for(int peer : node.peers)
            cout << node.id << " " << peer << endl;
}

ostream &operator<<(ostream &out, const Node &node) {
    out << "Node {" << ", " << endl;
    out << "\tid : " << node.id << ", " << endl;
    out << "\tmalicious : " << btoa(node.malicious) << ", " << endl;
    out << "\tpeers : " << node.peers << ", " << endl;
    out << "\tbalance : " << node.blockchain->get_last_blk()->balance.at(node.id) << ", " << endl;
    out << "\tlast-block: " << node.blockchain->get_last_blk()->id << ", " << endl;
    out << "\tforks: " << node.blockchain->tail_blks.size() << ", " << endl;
    out << "}" << endl;
    return out;
}



// 1. update and validate mem-pool
// 2. re-evaluate balance vector

/*
1. 45
2. 54
3. 23 
4. 17

23, 54

*/