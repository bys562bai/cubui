#pragma once 
#include <unordered_map>

namespace cubui{
    namespace _cubui_local_ieqnicxx{
        using namespace std;
        
        template <class Key,
              class T,
              class Hash = hash<Key>,
              class Pred = equal_to<Key>>
        using HashMap = unordered_map<Key, T, Hash, Pred>;
    }

    using _cubui_local_ieqnicxx::HashMap;
}