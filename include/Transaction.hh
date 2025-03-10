#ifndef TRANSACTION_HH
#define TRANSACTION_HH

#include "Declarations.hh"


struct Transaction
{
    int id;
    int sender, receiver;
    currency amount;
    static int cnt;

    Transaction(int sender, int receiver, currency amount);
};

#endif