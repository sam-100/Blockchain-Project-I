#include "Event.hh"
#include <typeinfo>
#include "Transaction.hh"
#include "Node.hh"
#include "utils.hh"

bool Event::operator<(const Event &other) const {
    return this->timestamp < other.timestamp;
}


Event::Event(clock_time ts, int n) : timestamp(ts), n_id(n) {};

TxnSendEvent::TxnSendEvent(clock_time timestamp, int n_id) : Event(timestamp, n_id) {}

TxnRecvEvent::TxnRecvEvent(clock_time timestamp, int n_id, Transaction *txn) : Event(timestamp, n_id) {
    this->txn = txn;
}


void process_event(Event *ev) {
    Node &node = nodes[ev->n_id];
    if(typeid(ev) == typeid(TxnSendEvent))
    {
        /* Create a new Transaction and broadcast it to peer nodes */
        float bal = node.balance[ev->n_id];
        Transaction *txn = new Transaction(ev->n_id, random_int(0, nodes.size()), random_float(0, bal));
        return;
    }
    if(typeid(ev) == typeid(TxnRecvEvent))
    {
        /* If txn is new, mark it visited and forward to peer nodes */

        return;
    }
    if(typeid(ev) == typeid(BlockMinedEvent))
    {
        /* If the mined block is still valid, add it to blockchain and forward to peer nodes */
        return;
    }
    if(typeid(ev) == typeid(BlockRecvEvent))
    {
        /* If received block is valid, add it to blockchain and forward to peer nodes */
        return;
    }
}