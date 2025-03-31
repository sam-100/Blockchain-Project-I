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
    virtual void to_string(ostream &os) const;
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

struct BlockMinedEvent_M : public Event
{
    Block *prev;

    BlockMinedEvent_M(clock_time t, int n, Block *p);
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

struct BlockRecvEvent_M : public Event
{
    Block *blk;
    
    BlockRecvEvent_M(clock_time t, int n, Block *blk);
    void test();
    void to_string(ostream &os) const;
};

struct TimeOutEvent : public Event
{
    string hash;
    int sender;

    TimeOutEvent(clock_time t, int n, string hash, int sender);
    void test();
    void to_string(ostream &os) const;
};

struct TimeOutEvent_M : public Event
{
    string hash;
    int sender;

    TimeOutEvent_M(clock_time t, int n, string hash, int sender);
    void test();
    void to_string(ostream &os) const;
};

struct HashRecvEvent : public Event
{
    string hash;
    int sender;

    HashRecvEvent(clock_time t, int n, string hash, int sender);
    void test();
    void to_string(ostream &os) const;
};
struct HashRecvEvent_M : public Event
{
    string hash;
    int sender;

    HashRecvEvent_M(clock_time t, int n, string hash, int sender);
    void test();
    void to_string(ostream &os) const;
};

struct BlockGetReqEvent : public Event 
{
    string hash;
    int sender;

    BlockGetReqEvent(clock_time t, int n, string hash, int sender);
    void test();
    void to_string(ostream &os) const;
};

struct BlockGetReqEvent_M : public Event 
{
    string hash;
    int sender;

    BlockGetReqEvent_M(clock_time t, int n, string hash, int sender);
    void test();
    void to_string(ostream &os) const;
};

struct StartMiningEvent : public Event
{
    StartMiningEvent(clock_time t, int n);
    void test();
    void to_string(ostream &os) const;
};

struct ReleasePrivateChainEvent : public Event
{
    int msg_no;
    static int cnt;

    ReleasePrivateChainEvent(clock_time ts, int n, int msg_no);
    void test();
    void to_string(ostream &os) const;
};

struct EndOfSimulationEvent : public Event 
{
    EndOfSimulationEvent(int n_id);
    void test();
    void to_string(ostream &os) const;
};

void process_event(Event*);

#endif