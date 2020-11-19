#include <iostream>
#include <memory>

#include <cubui/cubui.h>
#include <cubui/util/logging_m.h>
#include <cubui/container/listnode.h>

using namespace cubui;
using std::cout, std::endl;

static int count = 1;

struct MyNode : CUListNodeBase{
    int x;
    MyNode() {
        x = count;
        count++;
    }

    ~MyNode() {
        cout << prev() << endl;
        cout << next() << endl;
        cout << "destroy" << endl;
        cout << x << endl;
        cout << endl;
    }
};

struct MyNodes :CUListNodes<MyNode> {

};

int main(){
    {
        MyNode ns[10]{};
        MyNodes l;

        for (auto& n : ns) {
            l.push_back(n);
        }

        for (auto& n : l) {
            cout << n.x;
        }

        cout << endl;
    }
    

    return 0;
}