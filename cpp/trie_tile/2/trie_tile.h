
#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string.h>
//#include <boost/container/set.hpp>
//g++ trie_tile.cpp -I /auto/antares/tools/boost/boost-1.56.0/usr/include

#define THRESHOLD_WEIGHT    500
#define PREFIX_LEN_V4_MAX   (32)
#define PREFIX_LEN_V6_MAX   (128) // 128
#define TEST_ENTRIES        10
#define NUM_HW_ENTRIES_V4   1
#define NUM_HW_ENTRIES_V6   4

#define TEST_MODE_FILE 0

typedef enum {
    AF_V4,
    AF_V6
} inet_addr_family_t;

typedef struct entry_info_ {
    int prefix_len;
    int weight;
} entry_info_t;

enum {
    DETAIL_LOG,
    EVENT_LOG,
    TRACE_LOG,
    ERR_LOG,
    MAX_LOG
};

#define DEBUG_LEVEL     EVENT_LOG
//#define DEBUG_LEVEL     DETAIL_LOG

#define TRIE_TILE_LOG(lvl, msg) {\
    if (lvl >= DEBUG_LEVEL) {\
        std::cout << __func__ << "(" << __LINE__ << "): " << msg << std::endl;\
    }\
}

#define TRIE_TILE_DLOG(msg) \
    TRIE_TILE_LOG(DETAIL_LOG, msg)

#define TRIE_TILE_EVLOG(msg) \
    TRIE_TILE_LOG(EVENT_LOG, msg)

#define TRIE_TILE_TRLOG(msg) \
    TRIE_TILE_LOG(TRACE_LOG, msg)

#define TRIE_TILE_ERRLOG(msg) \
    TRIE_TILE_LOG(ERR_LOG, msg)

struct comparator_weight {
    bool operator() ( const entry_info_t &i, const entry_info_t &j ) {
        return (i.weight > j.weight);
    }
};

struct comparator_prefix_len {
    bool operator() ( const entry_info_t &i, const entry_info_t &j ) {
        return (i.prefix_len < j.prefix_len);
    }
};

int prio_dep_weight_v4[][2] = {
};

int prio_dep_weight_v6[][2] = {
};

int static_map_v4[PREFIX_LEN_V4_MAX][4];
int static_map_v6[PREFIX_LEN_V6_MAX][4];
int dep_wt_map_v4[PREFIX_LEN_V4_MAX];
int dep_wt_map_v6[PREFIX_LEN_V6_MAX];

typedef std::multiset<entry_info_t, comparator_weight> max_heap_w;
typedef std::multiset<entry_info_t, comparator_prefix_len> max_heap_l;
typedef std::multiset<entry_info_t>::iterator It;

max_heap_w myheapw_v4, myheapw_v6;
max_heap_l myheapl_v4, myheapl_v6;

void print_multiset(max_heap_w &myheapw, max_heap_l &myheapl);
max_heap_l::iterator search_heapl(max_heap_l &myheapl, int prefix_len);
std::pair<It, It> search_heapw(max_heap_w &myheapw, int weight);

void test_search_heapl(max_heap_l &myheapl);
void test_search_heapw(max_heap_w &myheapw);
void test_delete_heapw(max_heap_w &myheapw, max_heap_l &myheapl);
void test_delete_heapl(max_heap_l &myheapl, max_heap_w &myheapw);
void print_multiset_nzw_heapl(inet_addr_family_t addr_family);
void print_multiset_nzw_heapw(inet_addr_family_t addr_family);
max_heap_l &get_myheapl(inet_addr_family_t addr_family);


