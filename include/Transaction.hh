#ifndef TRANSACTION_HH
#define TRANSACTION_HH

#include "Declarations.hh"
#include <ostream>
#include <fstream>

using namespace std;

struct Transaction
{
    int id;
    int sender, receiver;
    currency amount;
    static int cnt;

    Transaction(int sender, int receiver, currency amount);
    friend ostream &operator<<(ostream &out, const Transaction &txn);
    // friend ofstream &operator<<(ofstream &out, const Transaction &txn);
};

#endif