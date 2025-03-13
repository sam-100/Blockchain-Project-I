#include "Blockchain.hh"
#include "Node.hh"
#include <iostream>

Blockchain::Blockchain(Block *genesis) {
    insert(genesis);         // genesis block 
}

void Blockchain::insert(Block *blk) {
    blocks.insert(blk);
    if(tail_blks.find(blk->prev_blk) != tail_blks.end())
        tail_blks.erase(blk->prev_blk);
    tail_blks.insert(blk);
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