#include "Event.hh"
#include <typeinfo>
#include "Transaction.hh"
#include "Node.hh"
#include "utils.hh"
#include <iostream>
#include "log.hh"

using namespace std;

/* Event */
Event::Event(clock_time ts, int n) : timestamp(ts), n_id(n) {};

bool compare_events::operator()(const Event *ev1, const Event *ev2) {
    return ev1->timestamp > ev2->timestamp;
}

ostream &operator<<(ostream &os, const Event *ev) {
    ev->to_string(os);
    return os;
}

/* Transaction Send Event */
TxnSendEvent::TxnSendEvent(clock_time timestamp, int n_id) : Event(timestamp, n_id) {}

ostream &operator<<(ostream &os, const TxnSendEvent *ev) {
    
    return os;
}

void TxnSendEvent::test() {}

void TxnSendEvent::to_string(ostream &os) const {
    os << "TxnSendEvent " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp: " << timestamp << " ";
    os << "}";
}


/* Transaction Receive Event */
TxnRecvEvent::TxnRecvEvent(clock_time timestamp, int n_id, Transaction *txn) : Event(timestamp, n_id) {
    this->txn = txn;
}

void TxnRecvEvent::test() {};

void TxnRecvEvent::to_string(ostream &os) const {
    os << "TxnRecvEvent " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp: " << timestamp << ", ";
    os << "txn : " << *txn << " ";
    os << "}";
}

/* Block Mined Event */
BlockMinedEvent::BlockMinedEvent(clock_time t, int n, Block *p) : Event(t, n) {
    prev = p;
}

void BlockMinedEvent::to_string(ostream &os) const {
    os << "BlockMinedEvent " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp: " << timestamp << ", ";
    os << "prev_id: " << prev->id << " ";
    os << "}";
}

void BlockMinedEvent::test() {};


/* Block Receive Event */
BlockRecvEvent::BlockRecvEvent(clock_time t, int n, Block *blk) : Event(t, n) {
    this->blk = blk;
}

void BlockRecvEvent::test() {};
void BlockRecvEvent::to_string(ostream &os) const {
    os << "BlockRecvEvent " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp: " << timestamp << ", ";
    os << "blk_id: " << blk->id << " ";
    os << "}";
}

/* Time Out Event */
TimeOutEvent::TimeOutEvent(clock_time t, int n, string h) : Event(t, n) {
    hash = h;
}
void TimeOutEvent::test() {}

/* Hash Received Event */
HashRecvEvent::HashRecvEvent(clock_time t, int n, string h) : Event(t, n) {
    hash = h;
}
void HashRecvEvent::test() {}

/* Block Get Request */
BlockGetRequest::BlockGetRequest(clock_time t, int n, string h) : Event(t, n) {
    hash = h;
}
void BlockGetRequest::test() {}


/* Process event function */

void process_event(Event *ev) {
    global_time = ev->timestamp;
    Node &node = nodes->at(ev->n_id);
    if(typeid(*ev) == typeid(TxnSendEvent))
    {
        TxnSendEvent *s_ev = (TxnSendEvent*)ev;

        /* Create a new Transaction and broadcast it to peer nodes */
        transaction_send(s_ev->n_id);

        /* Adding next send event to the queue.*/
        event_queue.push(new TxnSendEvent(global_time + random_exp_float(avg_send), ev->n_id));
        return;
    }
    if(typeid(*ev) == typeid(TxnRecvEvent))
    {
        TxnRecvEvent *s_ev = (TxnRecvEvent*)ev;
        
        /* If txn is new, mark it visited and forward to peer nodes */
        transaction_recv(s_ev->n_id, s_ev->txn);
        return;
    }
    if(typeid(*ev) == typeid(BlockMinedEvent))
    {
        BlockMinedEvent *s_ev = (BlockMinedEvent*)ev;
        log_event(s_ev);

        /* If the mined block is still valid, add it to blockchain and forward to peer nodes */
        node.mine_block(s_ev->prev);
        return;
    }
    if(typeid(*ev) == typeid(BlockRecvEvent))
    {
        BlockRecvEvent *s_ev = (BlockRecvEvent*)ev;
        log_event(s_ev);

        /* If received block is valid, add it to blockchain and forward to peer nodes */
        node.block_recv(s_ev->blk);
        return;
    }
    if(typeid(*ev) == typeid(HashRecvEvent))
    {
        HashRecvEvent *s_ev = (HashRecvEvent*)ev;
        log_event(s_ev);

        return;
    }
    if(typeid(*ev) == typeid(BlockGetRequest))
    {
        BlockGetRequest *s_ev = (BlockGetRequest*)ev;
        log_event(s_ev);

        return;
    }
    if(typeid(*ev) == typeid(TimeOutEvent))
    {
        TimeOutEvent *s_ev = (TimeOutEvent*)ev;
        log_event(s_ev);

        return;
    }
}