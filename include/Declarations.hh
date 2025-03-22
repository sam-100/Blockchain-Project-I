#ifndef DECLARATIONS_HH
#define DECLARATIONS_HH

#include <vector>
#include <queue>

#define BLOCK_SIZE 1024
#define BLOCK_REWARD 50
#define BLOCK_INTV_TIME 2
#define STOP_PARAMETER 50

using namespace std;

typedef double clock_time;
typedef double currency;


int random_int(int start, int end);
struct Node;
struct Event;
struct Transaction;
struct compare_events;
struct Block;
struct Blockchain;


extern priority_queue<Event*, vector<Event*>, compare_events> event_queue;
extern vector<Node> *nodes;
extern float malicious;
extern clock_time avg_send, global_time, timeout_time; 
extern int num_peers;
extern vector<vector<double>> prop_delay;


#endif