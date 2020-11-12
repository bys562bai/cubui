#include <cubui/cubui.h>
#include <cubui/util/logging_m.h>
#include <iostream>

using namespace cubui;

int main(){
    using std::cout, std::endl;

    int arr[] = { 1,2,3,4,5 };
    auto size = sizeof(arr) / sizeof(int);

    ArrayRef<int> arrRef(arr, size);
    cout << arrRef << endl;
    
    for (auto x : arrRef) {
        cout << x;
    }
    cout << endl;

    cout << arrRef.slice(0, 3) << endl;

    arrRef.copy(ArrayRef<int>(arr + 2, size - 2));
    cout << arrRef << endl;

    arrRef.set(0);
    cout << arrRef << endl;

    arrRef.reset();
    cout << arrRef << endl;

    return 0;
}