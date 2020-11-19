#pragma once
#include <algorithm>

namespace cubui {
	struct CUListNodeBase {

		template<typename Node = CUListNodeBase>
		struct Iterator{
			Iterator(Node* n) {
				cur = (CUListNodeBase * ) (void*)n;
			}

			Iterator() = default;
			Iterator(const Iterator&) = default;
			Iterator(Iterator&&) = default;

			Iterator& operator = (Iterator&) = default;
			Iterator& operator = (Iterator&&) = default;

			Node* operator ->() const {
				return (Node*)cur;
			}

			bool operator != (Iterator& it) const {
				return cur != it.cur;
			}

			void operator++ () {
				cur = cur->m_next;
			}

			void operator--() {
				cur = cur->m_prev;
			}

			Node& operator*() {
				return * (Node*) cur;
			}


		protected:
			CUListNodeBase* cur;
		};

		template<typename Node = CUListNodeBase>
		struct RIterator :Iterator<Node> {
			using BASE = Iterator<Node>;
			using BASE::BASE;

			void operator++ () {
				BASE::cur = BASE::cur->m_prev;
			}

			void operator-- (){
				BASE::cur = BASE::cur->m_next;
			}
		};

		//assume n is front, otherwise, n will unlink_before
		void insert_before(CUListNodeBase& n) {
			if (m_prev) {
				m_prev->insert_after(n);
			}
			else {
				//m_prev == nullptr
				auto& nlast = n.back();
				nlast.m_next = this;
				m_prev = &nlast;
			}
		}

		void insert_after(CUListNodeBase& n) {
			if (m_next) {
				auto& nlast = n.back();
				nlast.m_next = m_next;
				m_next->m_prev = &nlast;
			}
			if (n.m_prev) {
				n.m_prev->m_next = nullptr;
			}
			n.m_prev = this;
			m_next = &n;
		}

		void link_before(CUListNodeBase& n) {
			if (m_prev) {
				m_prev->m_next = nullptr;
			}
			m_prev = &n;

			if (n.m_next) {
				n.m_next->m_prev = nullptr;
			}
			n.m_next = this;
		}

		void link_after(CUListNodeBase& n) {
			if (m_next) {
				m_next->m_prev = nullptr;
			}
			m_next = &n;
			if (n.m_prev) {
				n.m_prev->m_next = nullptr;
			}
			n.m_prev = this;
		}

		void unlink_before() {
			if (m_prev) {
				m_prev->m_next = nullptr;
				m_prev = nullptr;
			}
		}

		void unlink_after() {
			if (m_next) {
				m_next->m_prev = nullptr;
				m_next = nullptr;
			}
		}

		void unlink() {
			if (m_prev) {
				m_prev->m_next = m_next;
			}
			if (m_next) {
				m_next->m_prev = m_prev;
			}
			m_prev = nullptr;
			m_next = nullptr;
		}

		CUListNodeBase& front() {
			CUListNodeBase* p = this;
			CUListNodeBase* prev;
			for (; prev = p->m_prev;) {
				if (prev) {
					p = prev;
				}
				else {
					break;
				}
			}
			return *p;
		}

		CUListNodeBase& back(){
			CUListNodeBase* p = this;

			CUListNodeBase* next;
			for (; next = p->m_next;) {
				if (next) {
					p = next;
				}
				else {
					break;
				}
			}
			
			return *p;
		}

		void swap(CUListNodeBase& n) {
			if (n.m_prev) {
				n.m_prev->m_next = this;
			}
			if (n.m_next) {
				n.m_next->m_prev = this;
			}
			if (m_prev) {
				m_prev->m_next = &n;
			}
			if (m_next) {
				m_next->m_prev = &n;
			}
			std::swap(m_prev, n.m_prev);
			std::swap(m_next, n.m_next);
		}

		Iterator<CUListNodeBase> begin() {
			return Iterator<CUListNodeBase>( (CUListNodeBase*) this);
		}

		Iterator<CUListNodeBase> end() {
			return Iterator<CUListNodeBase>( (CUListNodeBase*) nullptr);
		}

		CUListNodeBase* next() const {
			return m_next;
		}

		CUListNodeBase* prev() const {
			return m_prev;
		}

		bool is_linked() const{
			return m_prev || m_next;
		}

		virtual ~CUListNodeBase() {
			unlink();
		}

	protected:
		CUListNodeBase* m_next = nullptr;
		CUListNodeBase* m_prev = nullptr;

		template<typename Node>
		friend struct CUListNodes;
		friend void unlink_slice(CUListNodeBase& a, CUListNodeBase& b);
	};


	//ensure that a and b are in the same list
	inline void unlink_slice(CUListNodeBase& a, CUListNodeBase& b) {
		if(a.m_prev)
			a.m_prev->link_after(*b.m_next);
	}

	struct CUListNode : private CUListNodeBase {
		using CUListNodeBase::unlink;
		using CUListNodeBase::is_linked;

		void insert_after(CUListNode& n) {
			CUListNodeBase::insert_after(n);
		}

		void insert_before(CUListNode& n) {
			CUListNodeBase::insert_before(n);
		}

		void swap(CUListNode& n) {
			CUListNodeBase::swap(n);
		}

		CUListNode& front() {
			CUListNode* p = this;
			CUListNodeBase* prev;
			for (; prev = p->m_prev;) {
				if (prev) {
					p = (CUListNode*)prev;
				}
				else {
					break;
				}
			}
			CUListNode* next = (CUListNode*) p->m_next;
			if (next)
				return *next;
			else
				return *p;
		}
		
		CUListNode& back() {
			CUListNode* p = this;
			CUListNodeBase* next;
			for (; next = p->m_next;) {
				if (next) {
					p = (CUListNode*)next;
				}
				else {
					break;
				}
			}

			CUListNode* prev = (CUListNode*)p->m_prev;
			if (prev)
				return *prev;
			else
				return *p;
		}

		CUListNode* next() {
			return (CUListNode*)m_next;
		}

		CUListNode* prev() {
			return (CUListNode*)m_prev;
		}

		bool is_head() {
			return m_prev == nullptr;
		}

		bool is_end() {
			return m_next == nullptr;
		}

		template<typename Node>
		friend struct CUListNodes;
	};





	template<typename Node = CUListNode>
	struct CUListNodes {
		CUListNodes() {
			m_head.link_after(m_end);
		}

		CUListNodes(const CUListNodes&) = default;
		CUListNodes(CUListNodes&&) = default;

		CUListNodes& operator = (const CUListNodes&) = default;
		CUListNodes& operator = (CUListNodes&&) = default;

		Node& front() {
			auto front = (Node*) m_head.next();
			if (front != &m_end)
				return *front;
			
			assert(false);//The container is empty!
		}
		
		const Node& front() const {
			auto front = (Node*)m_head.next();
			if (front != &m_end)
				return *front;

			assert(false);//The container is empty!
		}

		Node& back() {
			auto back = (Node*)m_end.prev();
			if (back != &m_head) {
				return *back;
			}

			assert(false);//The container is empty!
		}

		const Node& back() const {
			auto back = (Node*) m_end.prev();
			if (back != &m_head) {
				return *back;
			}

			assert(false);//The container is empty!
		}

		void clear() {
			m_head.link_after(m_end);
		}

		bool empty() { return m_head.next() == &m_end; }

		//O(n) time complexity
		size_t size() const {
			size_t size = 0;
			for (const auto& n : *this) {
				++size;
			}
			return size;
		}

		void push_front(CUListNode& n) {
			m_head.insert_after(n);
		}

		void push_back(CUListNode& n) {
			m_end.insert_before(n);
		}

		void erase_back() {
			if (!empty()) {
				m_end.prev()->unlink();
			}
		}

		void erase_front() {
			if (!empty()) {
				m_head.front()->unlink();
			}
		}

		Node& pop_back() {
			assert(!empty());
			auto& n = *(Node*)m_end.prev();
			n.unlink();
			return n;
		}

		Node& pop_front() {
			assert(!empty());
			auto& n = * (Node*)m_head.next();
			n.unlink();
			return n;
		}

		using Iterator = CUListNodeBase::Iterator<Node>;
		using RIterator = CUListNodeBase::RIterator<Node>;

		Iterator begin() {
			return Iterator((Node*)m_head.next());
		}

		Iterator end() {
			return Iterator((Node*)&m_end);
		}

		Iterator cbegin() const {
			return Iterator((Node*)m_head.next());
		}

		Iterator cend() const {
			return Iterator((Node*)&m_end);
		}


		Iterator rbegin() {
			return RIterator((Node*) m_end.prev());
		}

		Iterator rend() {
			return RIterator((Node*) &m_head);
		}

		Iterator crbegin() const{
			return RIterator((Node*)m_end.prev());
		}

		Iterator crend() const {
			return RIterator((Node*)&m_head);
		}
	private:
		CUListNodeBase m_head;
		CUListNodeBase m_end;
	};
}