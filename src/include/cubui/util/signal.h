#pragma once
#include <stdint.h>
#include <functional>

#include <cubui/common/cutype.h>
#include <cubui/common/usefultype.h>
#include <cubui/container/string.h>
#include <cubui/container/listnode.h>
#include <cubui/container/hashmap.h>


namespace cubui{
    struct CUSignal{
        uint64_t id = 0;
        CUType* sender = nullptr;
        void* exData = nullptr;   //other data
    };

    //It will auto unlink when it is destroyed
    struct CUSignalCatcher : public CUListNode{
        CUSignalCatcher() = default;
        CUSignalCatcher(const CUSignalCatcher& c) {
            *this = c;
        }
        template<typename T>
        CUSignalCatcher(T f) {
            callback = f;
        }

        CUSignalCatcher& operator =(const CUSignalCatcher& c) {
            callback =  c.callback;
            return *this;
        }

        template<typename T>
        CUSignalCatcher& operator = (T f) {
            callback = f;
            return *this;
        }

        std::function<void (CUSignal*)> callback;


    private:

        template<typename T>
        void demoSetFunc(std::function<void(T*)> f) {
            using F = std::function<void(T*)>;
            auto pCallBack = (F*)&callback;
            *pCallBack = f;
        }
    };

    struct CUSignalDispatcher{

        void add(uint64_t sigGroupID, CUSignalCatcher& catcher) {
            m_u64SigGroups[sigGroupID].push_front(catcher);
        }

        void add(CUString& sigGroupID, CUSignalCatcher& catcher) {
            m_strSigGroups[sigGroupID].push_front(catcher);
        }

        void add_back(uint64_t sigGroupID, CUSignalCatcher& catcher) {
            m_u64SigGroups[sigGroupID].push_back(catcher);
        }

        void add_back(CUString& sigGroupID, CUSignalCatcher& catcher) {
            m_strSigGroups[sigGroupID].push_back(catcher);
        }

        //dispatch by sig id
        void dispatch(CUSignal& sig) {
            dispatch(sig.id, sig);
        }

        void dispatch(uint64_t sigGroupID, CUSignal& sig) {
            auto it = m_u64SigGroups.find(sigGroupID);
            if (it != m_u64SigGroups.end()) {
                auto& l = it->second;
                for (auto& catcher : l) {
                    catcher.callback(&sig);
                }
            }
        }

        void dispatch(CUString& sigGroupID, CUSignal& sig) {
            auto& l = m_strSigGroups.at(sigGroupID);
            for (auto& catcher : l) {
                catcher.callback(&sig);
            }
        }

        void rdispatch(uint64_t sigGroupID, CUSignal& sig)  {
            auto& l = m_u64SigGroups.at(sigGroupID);
            auto it = l.rbegin();
            auto end = l.rend();
            while (it != end) {
                it->callback(&sig);
            }
        }

        void rdispatch(CUString& sigGroupID, CUSignal& sig) {
            auto& l = m_strSigGroups.at(sigGroupID);
            auto it = l.rbegin();
            auto end = l.rend();
            while (it != end) {
                it->callback(&sig);
            }
        }

    protected:
        using SigList = CUListNodes<CUSignalCatcher>;
        CUHashMap<uint64_t, SigList> m_u64SigGroups;
        CUHashMap<CUString, SigList> m_strSigGroups;
    };
}