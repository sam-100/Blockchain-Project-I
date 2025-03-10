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
    friend ostream &operator<<(ostream &os, const Event *ev);
};

struct compare_events
{
    bool operator()(const Event *ev1, const Event *ev2);
};

struct TxnSendEvent : public Event 
{
    TxnSendEvent(clock_time timestamp, int n_id);
    friend ostream &operator<<(ostream &os, const TxnSendEvent *ev);
    void test();
};

struct TxnRecvEvent : public Event
{
    Transaction *txn;

    TxnRecvEvent(clock_time timestamp, int n_id, Transaction *txn);
    friend ostream &operator<<(ostream &os, const TxnRecvEvent *ev);
    void test();
};

// struct BlockMinedEvent : public Event 
// {
    
//     void test();
// };

// struct BlockRecvEvent : public Event
// {

//     void test();
// };


void process_event(Event*);

#endif