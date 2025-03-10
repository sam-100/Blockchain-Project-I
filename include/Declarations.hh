#ifndef DECLARATIONS_HH
#define DECLARATIONS_HH

#include <vector>


typedef double clock_time;
typedef double currency;


int random_int(int start, int end);
struct Node;
struct Event;
struct Transaction;


extern std::vector<Node> nodes;
extern float slow, low;
extern clock_time avg_send;
extern clock_time global_time;
extern int num_peers;

#endif