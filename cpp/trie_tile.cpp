#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <ctime>

using namespace std;

#define NUM_ENTRIES         10
#define THRESHOLD_WEIGHT    500

typedef struct entry_info_ {
    int prefix_len;
    int weight;
} entry_info_t;

entry_info_t entry_arr[NUM_ENTRIES];

struct comparator {
    bool operator() ( const entry_info_t &i, const entry_info_t &j) {
        return (i.weight < j.weight);
    }
};

int main(int argc, char *argv[])
{
    srand(time(0));

    std::set<entry_info_t, comparator> max_heap;

    entry_info_t test;
    for (int ent_num = 0; ent_num < NUM_ENTRIES; ent_num++) {
        entry_arr[ent_num].prefix_len = rand() % 32;
        //entry_arr[ent_num].weight = rand() % 3000;
        entry_arr[ent_num].weight = 3000;
        max_heap.insert(entry_arr[ent_num]);
        if (ent_num == 0) {
            test.prefix_len = entry_arr[ent_num].prefix_len;  
            test.weight = entry_arr[ent_num].weight; 
        }
    }


    //entry_info_t test2 = max_heap.find(test);
    //cout << test2.weight << endl;

    std::set<entry_info_t>::iterator iter;
    while(!max_heap.empty()) {
        iter = max_heap.end();
        --iter;
        cout << "prefix_len: " << iter->prefix_len << "\t weight: " << iter->weight << endl;
        max_heap.erase(iter);
    }

    return 0;
}
