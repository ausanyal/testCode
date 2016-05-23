#include <iostream>

using namespace std;

typedef struct tbl_ {
    int val;
} tbl_t;

void csco_hai_internal_l3_routing_thresholds_debug_isdirty_data_tcamthreshold(tbl_t *t)
{
    cout << __func__ << " : " << t->val << endl;
}

void csco_hai_internal_l3_routing_thresholds_debug_set_data_tcamthreshold(tbl_t *t, int val)
{
    cout << __func__ << " : " << t->val <<  " : " << val << endl;
}

void csco_hai_internal_l3_routing_thresholds_debug_get_data_tcamthreshold(tbl_t *t)
{
    cout << __func__ << " : " << t->val << endl;
}

void csco_hai_internal_l3_routing_thresholds_debug_get_data_triethreshold(tbl_t *t)
{
    cout << __func__ << " : " << t->val << endl;
}

void csco_hai_internal_l3_routing_thresholds_debug_isdirty_data_triethreshold(tbl_t *t)
{
    cout << __func__ << " : " << t->val << endl;
}

void csco_hai_internal_l3_routing_thresholds_debug_set_data_triethreshold(tbl_t *t, int val)
{
    cout << __func__ << " : " << t->val <<  " : " << val << endl;
}

#define L3_RTG_THRESHOLD(type, op, arg...) csco_hai_internal_l3_routing_thresholds_debug_ ##op## _data_ ##type## threshold(arg)

int main()
{
    tbl_t t, *pt;
    t.val = 10;
    pt = &t;
    L3_RTG_THRESHOLD(tcam, isdirty, pt);
    L3_RTG_THRESHOLD(tcam, get, pt);
    L3_RTG_THRESHOLD(tcam, set, pt, 1);
    L3_RTG_THRESHOLD(trie, isdirty, pt);
    L3_RTG_THRESHOLD(trie, get, pt);
    L3_RTG_THRESHOLD(trie, set, pt, 2);
    return 0;
}
