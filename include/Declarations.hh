#ifndef DECLARATIONS_HH
#define DECLARATIONS_HH

#include <vector>
#include <queue>
#include <unordered_map>

#define BLOCK_SIZE 1024     
#define BLOCK_REWARD 50     
#ifndef BLOCK_INTV_TIME     
    #define BLOCK_INTV_TIME 2   
#endif
#ifndef STOP_PARAMETER      
    #define STOP_PARAMETER 50   
#endif

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
extern vector<int> mal_nodes;
extern float malicious;
extern clock_time avg_send, global_time, timeout_time; 
extern int num_peers;
extern vector<vector<double>> prop_delay;
extern unordered_map<int, unordered_map<int, clock_time>> prop_delay_m;
extern bool ringmaster_mining;
extern int ringmaster, malicious_block_cnt;


#endif