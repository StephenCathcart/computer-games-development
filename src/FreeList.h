#ifndef FREELIST_H
#define FREELIST_H

#include <queue>
using std::queue;

template<class T>
class FreeList {
public :
	FreeList (unsigned int startCount = 0) {
		for (unsigned int i = 0; i < startCount; ++ i) {
			contents.push(new T());
		}
		m_handleOwnDeletion = true;
	}
	~FreeList (void) { clear(); }

	T* getNewObject () {
		if(contents.empty ()) { return new T(); }
		T* object = contents.front();
		contents.pop();
		return object;
	}

	void discardObject(T* object) {
		contents.push(object);
	}

	void clear () {
		while (!contents.empty() && m_handleOwnDeletion) {
			T* object = contents.front();
			contents.pop();
			delete object;
		}
	}

	void push(T* object) {
		contents.push(object);
	}

	void setHandleOwnDeletion(bool handleOwnDeletion) { m_handleOwnDeletion = handleOwnDeletion; }

protected :
	queue<T*> contents;
	bool	  m_handleOwnDeletion;
};

#endif
