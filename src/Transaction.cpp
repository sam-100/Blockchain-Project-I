#include "Transaction.hh"

int Transaction::cnt = 0;

Transaction::Transaction(int s, int r, currency a) : sender(s), receiver(r), amount(a) {
    id = cnt++;
};

ostream &operator<<(ostream &out, const Transaction &txn) {
    out << "Transaction " << "{";
    out << "id : " << txn.id << ", ";
    out << "sender : " << txn.sender << ", ";
    out << "receiver : " << txn.receiver << ", ";
    out << "amount : " << txn.amount << " ";
    out << "}";
    return out;
}

// ofstream &operator<<(ofstream &out, const Transaction &txn) {
//     out << "Transaction " << "{";
//     out << "id : " << txn.id << ", ";
//     out << "sender : " << txn.sender << ", ";
//     out << "receiver : " << txn.receiver << ", ";
//     out << "amount : " << txn.amount << " ";
//     out << "}";
//     return out;
// }

