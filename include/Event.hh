#ifndef EVENT_HH
#define EVENT_HH

#include "Declarations.hh"
#include <ostream>

using namespace std;

struct Event 
{
    int n_id;
    clock_time timestamp;

    Event(clock_time t, int n);
    virtual void test() = 0;
    virtual void to_string(ostream &os) const = 0;
    friend ostream &operator<<(ostream &os, const Event *ev);
};

struct compare_events
{
    bool operator()(const Event *ev1, const Event *ev2);
};

struct TxnSendEvent : public Event 
{
    TxnSendEvent(clock_time timestamp, int n_id);
    void test();
    void to_string(ostream &os) const;
};

struct TxnRecvEvent : public Event
{
    Transaction *txn;

    TxnRecvEvent(clock_time timestamp, int n_id, Transaction *txn);
    void test();
    void to_string(ostream &os) const;
};

struct BlockMinedEvent : public Event 
{
    Block *prev;
    
    BlockMinedEvent(clock_time t, int n, Block *p);
    void test();
    void to_string(ostream &os) const;
};

struct BlockRecvEvent : public Event
{
    Block *blk;
    
    BlockRecvEvent(clock_time t, int n, Block *blk);
    void test();
    void to_string(ostream &os) const;

};


void process_event(Event*);

#endif