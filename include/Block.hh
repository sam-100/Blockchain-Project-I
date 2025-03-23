#ifndef BLOCK_HH
#define BLOCK_HH

#include "Declarations.hh"
#include <vector>
#include <ostream>
#include <string>

using namespace std;

struct Block
{
    int id, miner_id;
    Block* prev_blk;
    string prev_hash;
    int height;
    bool malicious;
    clock_time timestamp;
    vector<currency> balance;
    vector<Transaction*> txn_list;

    static int cnt;

    Block(Block* prev, int n_id, bool m = false);
    void add_txn(Transaction *txn);
    int prev_id() const;
    string get_hash() const;
    friend ostream &operator<<(ostream &os, const Block *blk);
};


#endif