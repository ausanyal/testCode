#include  <iostream>

class t {
public:
    //inline void setL3_entries(int x) {
    inline void setL3_entries(int x) {
        this->l3_entries = x;
    }

    inline int get_l3_entries() {
        return this->l3_entries;
    }

private:
    int l3_entries;
};

// healthStatsEvSptr-> ##op ##attr(arg) 

#define HEALTH_STATS_EV(attr1, attr2) {\
    healthStatsEvSptr->set ##attr1 (healthStatsEvSptr->get_ ##attr2()); \
}

int main()
{
    t a, *healthStatsEvSptr;
    int i = 10;

    healthStatsEvSptr = &a;
    //healthStatsEvSptr->set(i);
    HEALTH_STATS_EV(L3_entries, l3_entries);
    
    return 0;
}
