#include "Block.hh"
#include "Transaction.hh"
#include "Node.hh"
#include "utils.hh"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "picosha256.h"

int Block::cnt = 0;

Block::Block(Block* prev, int n_id) {
    id = cnt++;
    #ifdef DEBUG
        cout << cnt << " blocks created." << endl;
    #endif

    if(prev == nullptr && n_id == -1)                           // genesis block
    {
        height = 0;
        prev_blk = nullptr;
        prev_hash = "null";
        balance = vector<currency>(num_peers, 100);
        timestamp = global_time;
        return;
    }
    
    height = prev->height + 1;
    prev_blk = prev;
    prev_hash = prev_blk->get_hash();
    balance = prev->balance;
    txn_list.push_back(new Transaction(-1, n_id, BLOCK_REWARD));          // coinbase txn
    balance[n_id] += BLOCK_REWARD;
    timestamp = global_time;
}

void Block::add_txn(Transaction *txn) {
    txn_list.push_back(txn);
    balance[txn->sender] -= txn->amount;
    balance[txn->receiver] += txn->amount;
}

int Block::prev_id() const {
    return prev_blk != nullptr ? prev_blk->id : -1;
}

ostream &operator<<(ostream &os, const Block *blk) {
    os << "Block " << "{ ";
    os << "id : " << blk->id << ", ";
    os << "prev : " << blk->prev_id() << ", ";
    os << "timestamp : " << blk->timestamp << ", ";
    os << "height : " << blk->height << ", "; // ", "<< endl;
    // os << "hash : " << blk->get_hash() << " ";
    // os << "\tbalance : " << blk->balance << " ";
    os << "}";
    return os;
}

string Block::get_hash() const {
    stringstream ss;
    ss << id << height << timestamp << balance << txn_list;
    string str = ss.str();
    return picosha2::hash256_hex_string(str);
}

