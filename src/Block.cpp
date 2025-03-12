#include "Block.hh"
#include "Transaction.hh"
#include "Node.hh"
#include <iostream>

int Block::cnt = 0;

Block::Block(Block* prev, int n_id) {
    id = cnt++;

    if(prev == nullptr && n_id == -1)                           // genesis block
    {
        height = 0;
        prev_blk = nullptr;
        balance = vector<currency>(nodes->size(), 100);
        timestamp = global_time;
        return;
    }
    
    height = prev->height + 1;
    prev_blk = prev;
    balance = prev->balance;
    txn_list.push_back(new Transaction(-1, n_id, 25));          // coinbase txn
    balance[n_id] += 25;
    timestamp = global_time;
}

void Block::add_txn(Transaction *txn) {
    txn_list.push_back(txn);
    balance[txn->sender] -= txn->amount;
    balance[txn->receiver] += txn->amount;
}