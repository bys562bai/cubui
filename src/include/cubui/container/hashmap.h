#pragma once 
#include <cubui/ext/robin_hood.h>

namespace cubui{
    namespace _cubui_local_ieqnicxx{
        using namespace robin_hood;
        
        template <typename Key,
              typename T,
              typename Hash = hash<Key>,
              typename Pred = std::equal_to<Key> >
        using CUHashMap = unordered_map<Key, T, Hash, Pred>;
    }

    using _cubui_local_ieqnicxx::CUHashMap;
}