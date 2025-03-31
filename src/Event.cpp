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
void Event::to_string(ostream &os) const {
    os << this << endl;
}
bool compare_events::operator()(const Event *ev1, const Event *ev2) {
    if(ev1->timestamp == -1)
        return true;
    if(ev2->timestamp == -1)
        return false;
    return ev1->timestamp > ev2->timestamp;
}
ostream &operator<<(ostream &os, const Event *ev) {
    ev->to_string(os);
    return os;
}

/* Transaction Send Event */
TxnSendEvent::TxnSendEvent(clock_time timestamp, int n_id) : Event(timestamp, n_id) {}
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
    os << "prev: " << prev->id << " ";
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

/* Block Receive Event in the overlay network */
BlockRecvEvent_M::BlockRecvEvent_M(clock_time t, int n, Block *blk) : Event(t, n) {
    this->blk = blk;
}
void BlockRecvEvent_M::test() {};
void BlockRecvEvent_M::to_string(ostream &os) const {
    os << "$BlockRecvEvent_M " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp: " << timestamp << ", ";
    os << "blk_id: " << blk->id << " ";
    os << "}";
}

/* Time Out Event */
TimeOutEvent::TimeOutEvent(clock_time t, int n, string h, int s) : Event(t, n) {
    hash = h;
    sender = s;
}
void TimeOutEvent::test() {}
void TimeOutEvent::to_string(ostream &os) const {
    os << "TimeOutEvent " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp : " << timestamp << ", ";
    os << "blk : " << nodes->at(sender).blockchain->get_blk(hash)->id << ", ";
    // os << "hash : " << hash << ", ";
    os << "sender : " << sender << " ";
    os << "}";
}

/* Time Out Event in the Overlay network */
TimeOutEvent_M::TimeOutEvent_M(clock_time t, int n, string h, int s) : Event(t, n) {
    hash = h;
    sender = s;
}
void TimeOutEvent_M::test() {}
void TimeOutEvent_M::to_string(ostream &os) const {
    os << "TimeOutEvent_M " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp : " << timestamp << ", ";
    // os << "blk : " << nodes->at(sender).blockchain->get_blk(hash)->id << ", ";
    // os << "hash : " << hash << ", ";
    os << "sender : " << sender << " ";
    os << "}";
}


/* Hash Received Event */
HashRecvEvent::HashRecvEvent(clock_time t, int n, string h, int s) : Event(t, n) {
    hash = h;
    sender = s;
}
void HashRecvEvent::test() {}
void HashRecvEvent::to_string(ostream &os) const {
    os << "HashRecvEvent " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp : " << timestamp << ", ";
    os << "blk : " << nodes->at(sender).blockchain->get_blk(hash)->id << ", ";
    // os << "hash : " << hash << ", ";
    os << "sender : " << sender << " ";
    os << "}";
}

/* Hash Received Event in overlay network */
HashRecvEvent_M::HashRecvEvent_M(clock_time t, int n, string h, int s) : Event(t, n) {
    hash = h;
    sender = s;
}
void HashRecvEvent_M::test() {}
void HashRecvEvent_M::to_string(ostream &os) const {
    os << "HashRecvEvent_M " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp : " << timestamp << ", ";
    // os << "blk : " << nodes->at(sender).blockchain->get_blk(hash)->id << ", ";
    // os << "hash : " << hash << ", ";
    os << "sender : " << sender << " ";
    os << "}";
}


/* Block Get Request */
BlockGetReqEvent::BlockGetReqEvent(clock_time t, int n, string h, int s) : Event(t, n) {
    hash = h;
    sender = s;
}
void BlockGetReqEvent::test() {}
void BlockGetReqEvent::to_string(ostream &os) const {
    os << "BlockGetReqEvent " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp : " << timestamp << ", ";
    os << "blk : " << nodes->at(n_id).blockchain->get_blk(hash)->id << ", ";
    // os << "hash : " << hash << ", ";
    os << "sender : " << sender << " ";
    os << "}";
}

/* Block Get Request in the Overlay network*/
BlockGetReqEvent_M::BlockGetReqEvent_M(clock_time t, int n, string h, int s) : Event(t, n) {
    hash = h;
    sender = s;
}
void BlockGetReqEvent_M::test() {}
void BlockGetReqEvent_M::to_string(ostream &os) const {
    os << "BlockGetReqEvent_M " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp : " << timestamp << ", ";
    // os << "blk : " << nodes->at(n_id).blockchain->get_blk(hash)->id << ", ";
    // os << "hash : " << hash << ", ";
    os << "sender : " << sender << " ";
    os << "}";
}

/* Malicious Block Mined in the Overlay network */
BlockMinedEvent_M::BlockMinedEvent_M(clock_time t, int n, Block *p) : Event(t, n) {
    prev = p;
}
void BlockMinedEvent_M::to_string(ostream &os) const {
    os << "BlockMinedEvent_M " << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp: " << timestamp << ", ";
    os << "prev: " << prev->id << " ";
    os << "}";
}
void BlockMinedEvent_M::test() {}

/* Start Mining Event */
StartMiningEvent::StartMiningEvent(clock_time t, int n) : Event(t, n) {}
void StartMiningEvent::test() {}
void StartMiningEvent::to_string(ostream &os) const {
    os << "StartMiningEvent" << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp : " << timestamp << " ";
    os << "}";
}

/* Release Private Chain Event */
int ReleasePrivateChainEvent::cnt = 0;
ReleasePrivateChainEvent::ReleasePrivateChainEvent(clock_time ts, int n, int msg_no) : Event(ts, n) {
    this->msg_no = msg_no;
}
void ReleasePrivateChainEvent::test() {};
void ReleasePrivateChainEvent::to_string(ostream &os) const {
    os << "ReleasePrivateChainEvent" << "{";
    os << "n_id : " << n_id << ", ";
    os << "timestamp : " << timestamp << ", ";
    os << "seq_no : " << msg_no << " ";
    os << "}";
}

/* End of Simulation Event */
EndOfSimulationEvent::EndOfSimulationEvent() : Event(-1, 0) {}
void EndOfSimulationEvent::test() {};
void EndOfSimulationEvent::to_string(ostream &os) const {
    os << "EndOfSimulationEvent" << "{}";
}


/* Process event function */

void process_event(Event *ev) {
    global_time = ev->timestamp;
    Node &node = nodes->at(ev->n_id);
    if(typeid(*ev) == typeid(TxnSendEvent))
    {
        TxnSendEvent *s_ev = (TxnSendEvent*)ev;

        /* Create a new Transaction and broadcast it to peer nodes */
        node.txn_send_event();

        /* Adding next send event to the queue.*/
        if(Block::cnt < STOP_PARAMETER)
            event_queue.push(new TxnSendEvent(global_time + random_exp_float(avg_send), ev->n_id));
        return;
    }
    if(typeid(*ev) == typeid(TxnRecvEvent))
    {
        TxnRecvEvent *s_ev = (TxnRecvEvent*)ev;
        
        /* If txn is new, mark it visited and forward to peer nodes */
        node.txn_recv_event(s_ev->txn);
        if(node.can_mine())
            event_queue.push(new StartMiningEvent(global_time, node.id));
        return;
    }
    if(typeid(*ev) == typeid(StartMiningEvent))
    {
        StartMiningEvent *s_ev = (StartMiningEvent*)ev;
        log_event(ev);
        
        if(node.malicious)
            node.start_mining_event_m();
        else
            node.start_mining_event();
        return;
    }
    if(typeid(*ev) == typeid(BlockMinedEvent))
    {
        BlockMinedEvent *s_ev = (BlockMinedEvent*)ev;
        log_event(s_ev);

        node.block_mined_event(s_ev->prev);
        return;
    }
    if(typeid(*ev) == typeid(BlockMinedEvent_M))
    {
        BlockMinedEvent_M *s_ev = (BlockMinedEvent_M*)ev;
        log_event(s_ev);

        node.block_mined_event_m(s_ev->prev);
        return;
    }
    if(typeid(*ev) == typeid(HashRecvEvent))
    {
        HashRecvEvent *s_ev = (HashRecvEvent*)ev;
        log_event(s_ev);

        node.hash_recv_event(s_ev->hash, s_ev->sender);
        return;
    }
    if(typeid(*ev) == typeid(HashRecvEvent_M))
    {
        HashRecvEvent_M *s_ev = (HashRecvEvent_M*)ev;
        log_event(s_ev);

        node.hash_recv_event_m(s_ev->hash, s_ev->sender);
        return;
    }
    if(typeid(*ev) == typeid(BlockGetReqEvent))
    {
        BlockGetReqEvent *s_ev = (BlockGetReqEvent*)ev;
        log_event(s_ev);
        
        node.block_get_event(s_ev->sender, s_ev->hash);
        return;
    }
    if(typeid(*ev) == typeid(BlockGetReqEvent_M))
    {
        BlockGetReqEvent_M *s_ev = (BlockGetReqEvent_M*)ev;
        log_event(s_ev);
        
        node.block_get_event_m(s_ev->sender, s_ev->hash);
        return;
    }
    if(typeid(*ev) == typeid(BlockRecvEvent))
    {
        BlockRecvEvent *s_ev = (BlockRecvEvent*)ev;
        log_event(s_ev);

        node.block_recv_event(s_ev->blk);
        if(node.id == ringmaster && node.alert())
        {
            event_queue.push(new ReleasePrivateChainEvent(global_time, node.id, release_command_cnt++));
        }
        return;
    }
    if(typeid(*ev) == typeid(BlockRecvEvent_M))
    {
        BlockRecvEvent_M *s_ev = (BlockRecvEvent_M*)ev;
        log_event(s_ev);

        node.block_recv_event_m(s_ev->blk);
        return;
    }
    if(typeid(*ev) == typeid(TimeOutEvent))
    {
        TimeOutEvent *s_ev = (TimeOutEvent*)ev;
        log_event(s_ev);

        node.timeout_event(s_ev->hash, s_ev->sender);
        return;
    }
    if(typeid(*ev) == typeid(TimeOutEvent_M))
    {
        TimeOutEvent_M *s_ev = (TimeOutEvent_M*)ev;
        log_event(s_ev);

        node.timeout_event_m(s_ev->hash, s_ev->sender);
        return;
    }
    if(typeid(*ev) == typeid(ReleasePrivateChainEvent))
    {
        ReleasePrivateChainEvent *s_ev = (ReleasePrivateChainEvent*)ev;
        log_event(s_ev);

        node.release_private_chain_event(s_ev->msg_no);
        return;
    }
    if(typeid(*ev) == typeid(EndOfSimulationEvent))
    {
        EndOfSimulationEvent *s_ev = (EndOfSimulationEvent*)ev;
        log_event(s_ev);

        event_queue.push(new ReleasePrivateChainEvent(global_time, ringmaster, release_command_cnt++));
        return;
    }
}

