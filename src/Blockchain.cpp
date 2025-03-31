#include "Blockchain.hh"
#include "Node.hh"
#include <iostream>
#include <sstream>

Blockchain::Blockchain(Block *genesis) {
    insert(genesis);         // genesis block 
}

Blockchain::Blockchain() {

}

void Blockchain::insert(Block *blk) {
    blocks.insert(blk);
    visited_hash[blk->get_hash()] = blk;
    if(tail_blks.find(blk->prev_blk) != tail_blks.end())
        tail_blks.erase(blk->prev_blk);
    tail_blks.insert(blk);
}

void Blockchain::clear() {
    blocks.clear();
    tail_blks.clear();
    visited_hash.clear();
}

Block *Blockchain::get_blk(string hash) {
    return visited_hash[hash];
}
Block *Blockchain::get_last_blk() {
    Block *last_blk = nullptr;
    for(Block *blk : tail_blks)
    {
        if(last_blk == nullptr)
            last_blk = blk;
        else if(blk->height > last_blk->height)
            last_blk = blk;
        else if(blk->height == last_blk->height && blk->timestamp < last_blk->timestamp)
            last_blk = blk;
    }
    return last_blk;        
}

bool Blockchain::contains(Block *blk) const {
    return blocks.find(blk) != blocks.end();
}

bool Blockchain::contains_hash(string hash) const {
    return visited_hash.find(hash) != visited_hash.end();
}

int Blockchain::height() {
    if(empty())
        return 0;
    return get_last_blk()->height;
}

string Blockchain::to_edge_string() const {
    stringstream ss;
    for(Block *blk : blocks)
        ss << blk->id << " " << blk->prev_id() << endl;
    return ss.str();
}

bool Blockchain::empty() const {
    return blocks.empty();
}

string Blockchain::to_list() {
    string str;
    unordered_set<Block*> visited;
    for(Block *blk : tail_blks)
    {
        str += '[';
        while(blk && visited.find(blk) == visited.end() && blocks.find(blk) != blocks.end())
        {
            str += to_string(blk->id) + " -> ";
            visited.insert(blk);
            blk = blk->prev_blk;
        }
        str += "] ";
    }
    return str;
}

string Blockchain::graph() {
    stringstream ss;
    unordered_set<Block*> visited;
    for(Block *blk : tail_blks)
    {
        ss << blk->id;
        while(blk->prev_blk && visited.find(blk) == visited.end())
        {
            visited.insert(blk);
            ss << " -> " << blk->prev_id();
            blk = blk->prev_blk;
        }
        ss << ";" << endl;
    }
    for(Block *blk : blocks)
    {
        if(blk->malicious)
            ss << blk->id << "[color=red, style=filled]" << endl;
        else
            ss << blk->id << "[color=green, style=filled]" << endl;
    }
    
    return ss.str();
}

ostream &operator<<(ostream &os, const Blockchain *bc) {
    for(Block *tail : bc->tail_blks)
    {
        Block *curr = tail;
        while(curr != nullptr)
        {
            os << curr << endl;
            curr = curr->prev_blk;
        }
        os << endl;
    }
    return os;
}

int Blockchain::size() const {
    return blocks.size();
}

int Blockchain::mal_cnt() const {
    int cnt = 0;
    for(Block *blk : blocks)
        cnt += blk->malicious;
    return cnt;
}

int Blockchain::mal_cnt_at_longest() {
    Block *blk = get_last_blk();
    int cnt = 0;
    while(blk)
    {
        cnt += blk->malicious;
        blk = blk->prev_blk;
    }
    cout << "mal_cnt_at_longest = " << cnt << endl;
    return cnt;
}