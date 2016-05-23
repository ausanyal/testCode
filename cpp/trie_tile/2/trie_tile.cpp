
#include "trie_tile.h"

using namespace std;

max_heap_l &get_myheapl(inet_addr_family_t addr_family)
{
    if (addr_family == AF_V4) {
        return myheapl_v4;
    } else {
        return myheapl_v6;
    }
}

max_heap_w &get_myheapw(inet_addr_family_t addr_family)
{
    if (addr_family == AF_V4) {
        return myheapw_v4;
    } else {
        return myheapw_v6;
    }
}

int get_hw_weight(inet_addr_family_t af)
{
    if (af == AF_V4) { 
        return NUM_HW_ENTRIES_V4;
    } else if (af == AF_V6) { 
        return NUM_HW_ENTRIES_V6;
    } 

    return 0;
}

void init_dep_wt_map(inet_addr_family_t af)
{
    int num_prioritized, idx, i;

    if (af == AF_V4) {

        memset(dep_wt_map_v4, 0, PREFIX_LEN_V4_MAX * sizeof(int)); 
        num_prioritized = sizeof(prio_dep_weight_v4)/sizeof(prio_dep_weight_v4[0]);

        for (i = 0; i < num_prioritized; i++) {
            idx = prio_dep_weight_v4[i][0];
            dep_wt_map_v4[idx] = prio_dep_weight_v4[i][1];
        }

        for (i = 0; i < PREFIX_LEN_V4_MAX; i++) {
            if (!dep_wt_map_v4[i]) {
                dep_wt_map_v4[i] = 1;
            }
        }
    }
    else if (af == AF_V6) {
        memset(dep_wt_map_v6, 0, PREFIX_LEN_V6_MAX * sizeof(int)); 
        num_prioritized = sizeof(prio_dep_weight_v6)/sizeof(prio_dep_weight_v6[0]);

        for (i = 0; i < num_prioritized; i++) {
            idx = prio_dep_weight_v6[i][0];
            dep_wt_map_v6[idx] = prio_dep_weight_v6[i][1];
        }

        for (i = 0; i < PREFIX_LEN_V6_MAX; i++) {
            if (!dep_wt_map_v6[i]) {
                dep_wt_map_v6[i] = 1;
            }
        }

    }
}

/* Init the static map per address family.
 * For each prefix_length(say x), set the value for prefix x:x-3 as follows:
 * x: 1 ; x-1: 2 ; x-2: 4 ; x-3: 8
 */
void init_static_map(inet_addr_family_t af)
{
    int blen, plen, idx;

    if (af == AF_V4) {
        for (plen = 0; plen < PREFIX_LEN_V4_MAX; plen++) {
            for (blen = plen; (blen >= 0) && (blen > (plen-4)); blen--) {
                idx = plen-blen;
                if (plen == blen) {
                    static_map_v4[plen][idx] = 1;
                } else {
                    static_map_v4[plen][idx] = static_map_v4[plen][idx-1] * 2;
                }
                TRIE_TILE_EVLOG(": v4 plen/blen(" << idx << ")" << ": " << plen 
                            << "/" << blen << ": " << static_map_v4[plen][idx]); 
            }
        }
    } 
    else if (af == AF_V6) {
        for (plen = 0; plen < PREFIX_LEN_V6_MAX; plen++) {
            for (blen = plen; (blen >= 0) && (blen > (plen-4)); blen--) {
                idx = plen-blen;
                if (plen == blen) {
                    static_map_v6[plen][idx] = 1;
                } else {
                    static_map_v6[plen][idx] = static_map_v6[plen][idx-1] * 2;
                }
                TRIE_TILE_EVLOG(": v6 plen/blen(" << idx << ")" << ": " << plen 
                            << "/" << blen << ": " << static_map_v6[plen][idx]); 
            }
        }

    }

}

int get_computed_weight(inet_addr_family_t af, int prefix_len, 
                        int base_len, int count)
{
    int computed_wt = 0, dep_wt = 0, trie_wt = 0, idx;

    idx = prefix_len - base_len;
    if (af == AF_V4) {
        dep_wt  = dep_wt_map_v4[prefix_len-1]; 
        trie_wt = static_map_v4[prefix_len-1][idx];
    } else if (af == AF_V6) {
        dep_wt  = dep_wt_map_v6[prefix_len-1]; 
        trie_wt = static_map_v6[prefix_len-1][idx];
    } else {
        return -1;
    }

    computed_wt = count * dep_wt * trie_wt * get_hw_weight(af); 

    TRIE_TILE_EVLOG(" prefix_len: " << prefix_len << " base_len(" << idx << ")" 
                    << ": " << base_len << " computed_wt: " << computed_wt);

    return computed_wt;
}

void update_weight(inet_addr_family_t af, int base_len, 
                   int weight, bool del_if_0w)
{
    max_heap_l  &myheapl = get_myheapl(af);
    max_heap_w  &myheapw = get_myheapw(af);
    max_heap_l::iterator iterl;

    /* Lookup entry in heapl(using base_len) and fetch the weight to 
     * lookup in heapw filter entries returned from heapw based on prefix_len
     */
    iterl = search_heapl(myheapl, base_len);
    if (iterl != myheapl.end()) {

        std::pair<It, It> x = search_heapw(myheapw, iterl->weight);
        for (It it=x.first; it != x.second; ++it) {
            TRIE_TILE_ERRLOG(it->prefix_len << " : " << iterl->prefix_len);
            if (it->prefix_len == iterl->prefix_len) {

                TRIE_TILE_EVLOG(": Found " << it->prefix_len << ";" 
                                << it->weight << " new wt: " << weight);
                /* Update weight in both the multi-sets. 
                 * But delete the entries first
                 */
                myheapl.erase(iterl);
                myheapw.erase(it);
                // Update with new weight
                if ((weight > 0) || (del_if_0w == 0)) {
                    entry_info_t test_entry;
                    test_entry.prefix_len   = it->prefix_len;
                    test_entry.weight       = weight;
                    myheapw.insert(test_entry); 
                    myheapl.insert(test_entry); 
                }
                break;

            }
        }
    } else {
        TRIE_TILE_ERRLOG("Error: base_len: " << base_len 
                         << " could not be found in heapl ");
    }

    return;
}

void add_route(inet_addr_family_t af, int prefix_len, int count) 
{
    int                     weight = 0, base_len = 0;
    entry_info_t            entry = {0};
    max_heap_l::iterator    iterl;
    max_heap_l              &myheapl = get_myheapl(af);
    max_heap_w              &myheapw = get_myheapw(af);

    iterl = search_heapl(myheapl, prefix_len);

    // Found entry. Update its current weight
    for (base_len = prefix_len; 
            (base_len > 0) && (base_len > (prefix_len-4)); 
            base_len--) {

        iterl = search_heapl(myheapl, base_len);
        if (iterl != myheapl.end()) {

            weight = get_computed_weight(af, prefix_len, 
                                         base_len, count);

            weight += iterl->weight;

            TRIE_TILE_EVLOG("updating wt(" << af << "): prefix_len: " 
                    << prefix_len << " base_len: " << base_len << " old wt: " 
                    << iterl->weight << " new wt: " << weight);

            update_weight(af, base_len, weight, 0);

        } else {

            weight = get_computed_weight(af, prefix_len, base_len, count);
            entry.prefix_len        = base_len;
            entry.weight            = weight;

            TRIE_TILE_EVLOG("Adding wt: prefix_len: " << prefix_len 
                    << " base_len: " << base_len << " wt: " << weight);

            myheapw.insert(entry); 
            myheapl.insert(entry); 

        }

    }

    print_multiset_nzw_heapl(AF_V4);

    return;
}

void del_route(inet_addr_family_t af, int prefix_len, int count)
{
    int                     weight = 0, base_len = 0;
    max_heap_l              &myheapl = get_myheapl(af);
    max_heap_l::iterator    iterl;

    /* Lookup entry in heapl and fetch the weight to 
     * lookup in heapw filter entries returned from heapw based on prefix_len
     */
    for (base_len = prefix_len; 
            (base_len > 0) && (base_len > (prefix_len-4)); 
            base_len--) {

        iterl = search_heapl(myheapl, base_len);
        if (iterl != myheapl.end()) {

            weight = get_computed_weight(af, prefix_len, 
                                         base_len, count);

            weight = iterl->weight - weight;

            TRIE_TILE_EVLOG("updating wt(" << af << "): prefix_len: " 
                    << prefix_len << " base_len: " << base_len << " old wt: " 
                    << iterl->weight << " new wt: " << weight);

            update_weight(af, base_len, weight, 1);

        } 

    }

    print_multiset_nzw_heapl(AF_V4);
}

void test_init_entries()
{
    int                 prefix_len, count;
    string              route_op, af;
    inet_addr_family_t  addr_family;

#ifdef TEST_MODE_FILE
    std::ifstream testfile("testinput.txt");

    std::string line;
    while(std::getline(testfile, line)) {
        std::istringstream iss(line);
        if (!(iss >> af >> route_op >> prefix_len >> count)) {
            TRIE_TILE_EVLOG("Error");
            break;
        }
        TRIE_TILE_EVLOG("Adding/deleting route prefix_len: " 
                        << prefix_len << " count: " << count);
        if (af.compare("v4") == 0) {
            addr_family = AF_V4;
        } else {
            addr_family = AF_V6;

        }
        if (!route_op.compare("add")) {
            add_route(addr_family, prefix_len, count);
        } else {
            del_route(addr_family, prefix_len, count);

        }
    }

#else
    int ent_num, plen = 0;

    srand(time(0));

    // v4
    for (ent_num = 0; ent_num < TEST_ENTRIES/2; ent_num++) {

        prefix_len = rand() % 32;
        TRIE_TILE_EVLOG(ent_num << " Adding v4 route prefix_len: " << prefix_len);
        add_route(AF_V4, prefix_len, 1);
    }
    // v6  
    for (ent_num = 0; ent_num < TEST_ENTRIES/2; ent_num++) {

        prefix_len = rand() % 128;
        TRIE_TILE_EVLOG(ent_num << " Adding v6 route prefix_len: " << prefix_len);
        add_route(AF_V6, prefix_len, 1);
    }
#endif

    string hyphen(20, '_');
    string star(20, '*');

    TRIE_TILE_EVLOG(hyphen);
    print_multiset_nzw_heapl(AF_V4);
    print_multiset_nzw_heapl(AF_V6);
    TRIE_TILE_EVLOG(star);
    print_multiset_nzw_heapw(AF_V4);
    print_multiset_nzw_heapw(AF_V6);
    TRIE_TILE_EVLOG(hyphen);

    return;

}

// Max 1 entry expected. Hence using find
max_heap_l::iterator search_heapl(max_heap_l &myheapl_v4, int prefix_len)
{
    entry_info_t            test;
    max_heap_l::iterator    iter;

    test.prefix_len = prefix_len; 
    iter            = myheapl_v4.find(test);

    return iter;

}

// More than 1 entry can be returned. Hence using equal_range
std::pair<It, It> search_heapw(max_heap_w &myheapw_v4, int weight)
{
    entry_info_t        test;
    std::pair<It, It>   x;

    test.weight = weight; 
    x           = myheapw_v4.equal_range(test);

    return x;

}

void test_search_heapl(max_heap_l &myheapl_v4)
{
    max_heap_l::iterator iterl;

    // Search max_heapl
    iterl = search_heapl(myheapl_v4, 20);
    if(iterl != myheapl_v4.end()) {
        TRIE_TILE_EVLOG(": Found " << iterl->prefix_len << ";" << iterl->weight);
    }
    else {
        TRIE_TILE_EVLOG("Not found");
    }
}

void test_search_heapw(max_heap_w &myheapw_v4)
{
    // Search max_heapw
    std::pair<It, It> x = search_heapw(myheapw_v4, 1000);
    for (It it=x.first; it != x.second; ++it) {
        TRIE_TILE_EVLOG(": Found " << it->prefix_len << ";" << it->weight);
    }
}

void test_delete_heapw(max_heap_w &myheapw_v4, max_heap_l &myheapl_v4)
{
    max_heap_w::iterator  iterw;

    while(!myheapw_v4.empty()) {
        iterw = myheapw_v4.end();
        --iterw;
        TRIE_TILE_EVLOG(": prefix_len: " << iterw->prefix_len 
                         << "  weight: " << iterw->weight);
        myheapw_v4.erase(iterw);

        entry_info_t test_entry;
        test_entry.prefix_len   = iterw->prefix_len;
        test_entry.weight       = iterw->weight;
        myheapl_v4.erase(test_entry);
    }
}

void test_delete_heapl(max_heap_l &myheapl_v4, max_heap_w &myheapw_v4)
{
    max_heap_l::iterator  iterl;

    while(!myheapl_v4.empty()) {
        iterl = myheapl_v4.end();
        --iterl;
        TRIE_TILE_EVLOG(": prefix_len: " << iterl->prefix_len 
                        << "  weight: " << iterl->weight);

        myheapl_v4.erase(iterl);

        entry_info_t test_entry;
        test_entry.prefix_len   = iterl->prefix_len;
        test_entry.weight       = iterl->weight;
        myheapw_v4.erase(test_entry);
    }
}

void print_multiset_nzw_heapl(inet_addr_family_t addr_family)
{
    max_heap_l  &myheapl = get_myheapl(addr_family);
    string      af;

    if (addr_family == AF_V4) {
        af = "v4";
    } else {
        af = "v6";
    }

    for (max_heap_l::const_iterator j(myheapl.begin()), end(myheapl.end()); 
         j != end ; 
         ++j) {
        if (j->weight) {
            TRIE_TILE_EVLOG("myheapl (" << af << ")-> prefix_len: " 
                            << j->prefix_len << "  weight: " << j->weight);
        }
    }
    //TRIE_TILE_EVLOG("___________________________________________");

}

void print_multiset_nzw_heapw(inet_addr_family_t addr_family)
{
    max_heap_w  &myheapw = get_myheapw(addr_family);
    string      af;

    if (addr_family == AF_V4) {
        af = "v4";
    } else {
        af = "v6";
    }

    for (max_heap_w::const_iterator j(myheapw.begin()), end(myheapw.end()) ; 
         j != end ; 
         ++j) {
        if (j->weight) {
            TRIE_TILE_EVLOG("myheapw (" << af << ")-> prefix_len: " 
                            << j->prefix_len << "  weight: " << j->weight);
        }
    }
    TRIE_TILE_EVLOG("___________________________________________");

}

bool get_best_entry(inet_addr_family_t af, entry_info_t *entry)
{
    max_heap_w  &myheapw = get_myheapw(af);

    max_heap_w::const_iterator it = myheapw.begin();

    if (it != myheapw.end()) {
        entry->prefix_len = it->prefix_len;
        entry->weight = it->weight;
        return 1;
    } else {
        TRIE_TILE_ERRLOG("ERROR: Could not find best entry");
    }

    return 0; 

}

int main(int argc, char *argv[])
{
    max_heap_w::iterator    iterw;
    entry_info_t            entry;

    init_static_map(AF_V4);
    init_static_map(AF_V6);
    init_dep_wt_map(AF_V4);
    init_dep_wt_map(AF_V6);
    test_init_entries();

    if (get_best_entry(AF_V4, &entry)) {
        cout << "Max V4 prefix_len: " << entry.prefix_len 
            << " weight: " << entry.weight << endl; 
    }
    if (get_best_entry(AF_V6, &entry)) {
        cout << "Max V6 prefix_len: " << entry.prefix_len 
            << " weight: " << entry.weight << endl; 
    }

    return 0;
}

void run_test_apis(max_heap_w &myheapw_v4, max_heap_l &myheapl_v4)
{
    //test_search_heapl(myheapl_v4);
    //test_search_heapw(myheapw_v4);
    //test_delete_heapw(myheapw_v4, myheapl_v4);
    //test_delete_heapl(myheapl_v4, myheapw_v4);
}

