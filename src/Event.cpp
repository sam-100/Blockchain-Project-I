#include "Event.hh"
#include <typeinfo>
#include "Transaction.hh"
#include "Node.hh"
#include "utils.hh"
#include <iostream>

using namespace std;


Event::Event(clock_time ts, int n) : timestamp(ts), n_id(n) {};

bool compare_events::operator()(const Event *ev1, const Event *ev2) {
    return ev1->timestamp > ev2->timestamp;
}

ostream &operator<<(ostream &os, const Event *ev) {
    os << "Event " << "{";
    os << "n_id : " << ev->n_id << ", ";
    os << "timestamp: " << ev->timestamp << " ";
    os << "}";
    return os;
}

TxnSendEvent::TxnSendEvent(clock_time timestamp, int n_id) : Event(timestamp, n_id) {}

ostream &operator<<(ostream &os, const TxnSendEvent *ev) {
    os << "TxnSendEvent " << "{";
    os << "n_id : " << ev->n_id << ", ";
    os << "timestamp: " << ev->timestamp << " ";
    os << "}";
    return os;
}

void TxnSendEvent::test() {}

TxnRecvEvent::TxnRecvEvent(clock_time timestamp, int n_id, Transaction *txn) : Event(timestamp, n_id) {
    this->txn = txn;
}

void TxnRecvEvent::test() {};

ostream &operator<<(ostream &os, const TxnRecvEvent *ev) {
    os << "TxnRecvEvent " << "{";
    os << "n_id : " << ev->n_id << ", ";
    os << "timestamp: " << ev->timestamp << ", ";
    os << "txn : " << *ev->txn << " ";
    os << "}";
    return os;
}


void process_event(Event *ev) {
    global_time = ev->timestamp;
    
    Node &node = nodes[ev->n_id];
    if(typeid(*ev) == typeid(TxnSendEvent))
    {
        TxnSendEvent *s_ev = (TxnSendEvent*)ev;
        cout << s_ev << endl;

        /* Create a new Transaction and broadcast it to peer nodes */
        float bal = node.balance[ev->n_id];
        Transaction *txn = new Transaction(ev->n_id, random_int(0, nodes.size()), random_float(0, bal));
        forward_txn(ev->n_id, txn);

        /* Adding next send event to the queue.*/
        event_queue.push(new TxnSendEvent(global_time + random_exp_float(avg_send), ev->n_id));
        return;
    }
    if(typeid(*ev) == typeid(TxnRecvEvent))
    {
        TxnRecvEvent *s_ev = (TxnRecvEvent*)ev;
        cout << s_ev << endl;
        
        /* If txn is new, mark it visited and forward to peer nodes */
        forward_txn(s_ev->n_id, s_ev->txn);
        return;
    }
    // if(typeid(*ev) == typeid(BlockMinedEvent))
    // {
    //     BlockMinedEvent *s_ev = (BlockMinedEvent*)ev;
    //     // cout << s_ev << endl;

    //     /* If the mined block is still valid, add it to blockchain and forward to peer nodes */
    //     return;
    // }
    // if(typeid(*ev) == typeid(BlockRecvEvent))
    // {
    //     BlockRecvEvent *s_ev = (BlockRecvEvent*)ev;
    //     // cout << s_ev << endl;

    //     /* If received block is valid, add it to blockchain and forward to peer nodes */
    //     return;
    // }
}