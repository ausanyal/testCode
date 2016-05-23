#include <iostream>

using namespace std;

typedef struct node_ {
    int data;
} node_t;

void test_ref(node_t &a, int flag = 1)
{
    for (int i = 0; i < flag; i++) {
        a[i].data = i+20;
    }
}

void caller1()
{
    int num_nodes = 2;

    cout << __func__ << endl;

    node_t c1_a[num_nodes];

    test_ref(c1_a, num_nodes);

    for (int i = 0; i < num_nodes ; i++) {
        cout << __func__ << " : " << c1_a[i].data;
    }
}

void caller2()
{
    cout << __func__ << endl;
    node_t c2_a;

    test_ref(c2_a);

    cout << __func__ << " : " << c2_a.data;
}

int main()
{

    caller1();
    caller2();

    return 0;
}