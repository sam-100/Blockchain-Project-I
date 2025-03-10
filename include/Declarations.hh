#ifndef DECLARATIONS_HH
#define DECLARATIONS_HH

#include <vector>
#include <queue>

using namespace std;

typedef double clock_time;
typedef double currency;


int random_int(int start, int end);
struct Node;
struct Event;
struct Transaction;
struct compare_events;

extern priority_queue<Event*, vector<Event*>, compare_events> event_queue;
extern vector<Node> nodes;
extern float slow, low;
extern clock_time avg_send;
extern clock_time global_time;
extern int num_peers;

#endif