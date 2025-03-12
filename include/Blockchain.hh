#ifndef BLOCKCHAIN_HH
#define BLOCKCHAIN_HH

#include "Block.hh"
#include <unordered_set>
#include <ostream>

using namespace std;

struct Blockchain
{
    unordered_set<Block*> blocks;
    unordered_set<Block*> tail_blks;

    Blockchain(Block *genesis);
    void insert(Block *blk);
    Block *get_last_blk();
    bool contains(Block *blk) const;
    friend ostream &operator<<(ostream &os, const Blockchain *bc);
};

#endif