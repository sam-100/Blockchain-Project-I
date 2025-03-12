#ifndef BLOCK_HH
#define BLOCK_HH

#include "Declarations.hh"
#include <vector>
#include <ostream>

using namespace std;

struct Block
{
    int id;
    Block* prev_blk;
    int height;
    clock_time timestamp;
    vector<currency> balance;
    vector<Transaction*> txn_list;

    static int cnt;

    Block(Block* prev, int n_id);
    void add_txn(Transaction *txn);
    int prev_id() const;
    friend ostream &operator<<(ostream &os, const Block *blk);
};


#endif