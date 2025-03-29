#ifndef BLOCKCHAIN_HH
#define BLOCKCHAIN_HH

#include "Block.hh"
#include <unordered_set>
#include <unordered_map>
#include <ostream>
#include <string>

using namespace std;

struct Blockchain
{
    unordered_set<Block*> blocks;
    unordered_set<Block*> tail_blks;
    unordered_map<string, Block*> visited_hash;

    Blockchain();
    Blockchain(Block *genesis);
    void insert(Block *blk);
    void clear();
    Block *get_last_blk();
    Block *get_blk(string hash);
    bool contains(Block *blk) const;
    bool contains_hash(string hash) const;
    int height();
    string to_edge_string() const;
    bool empty() const;
    friend ostream &operator<<(ostream &os, const Blockchain *bc);
};

#endif