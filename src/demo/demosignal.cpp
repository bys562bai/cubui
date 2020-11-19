#include <iostream>
#include <memory>

#include <cubui/cubui.h>
#include <cubui/util/logging_m.h>
#include <cubui/util/signal.h>


using namespace cubui;
using std::cout, std::endl;

void f(int x, int ) {

}

int main(){
    CUSignal sig;
    CUSignalDispatcher dispatcher;

    CUSignalCatcher catcher = [](CUSignal* sig) {
        cout << sig->id << endl;
    };

    {
    dispatcher.add(0, catcher);
    }

    dispatcher.dispatch(sig);


    return 0;
}
