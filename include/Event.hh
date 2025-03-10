#ifndef EVENT_HH
#define EVENT_HH

#include "Declarations.hh"

struct Event 
{
    int n_id;
    clock_time timestamp;

    Event(clock_time t, int n);
    bool operator<(const Event &other) const;
};


struct TxnSendEvent : public Event 
{
    TxnSendEvent(clock_time timestamp, int n_id);
};

struct TxnRecvEvent : public Event
{
    Transaction *txn;

    TxnRecvEvent(clock_time timestamp, int n_id, Transaction *txn);
};

struct BlockMinedEvent : public Event 
{
    
};

struct BlockRecvEvent : public Event
{

};


void process_event(Event*);

#endif