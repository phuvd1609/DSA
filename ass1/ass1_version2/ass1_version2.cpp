#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;

#ifndef ILIST_H
#define ILIST_H

template <class T>
class IList
{
public:
	virtual void add(const T& element) = 0;
	virtual void add(int index, const T& element) = 0;
	virtual T removeAt(int index) = 0;
	virtual bool removeItem(const T& item) = 0;
	virtual bool empty() = 0;
	virtual int size() = 0;
	virtual void clear() = 0;
	virtual T get(int index) = 0;
	virtual void set(int index, const T& element) = 0;
	virtual int indexOf(const T& item) = 0;
	virtual bool contains(const T& item) = 0;
	virtual string toString() = 0;
};

#endif

// STUDENT ANSWER
#ifndef FRAGMENT_LINKED_LIST
#define FRAGMENT_LINKED_LIST
template <class T>
class FragmentLinkedList : IList<T>
{
public:
	class Node;
	class Iterator;
	Node* head = NULL;
	Node* tail = NULL;

protected:
	Node** fragmentPointers;
	int fragmentMaxSize;
	int count;
	int countFPointers;

public:
	FragmentLinkedList(int fragmentMaxSize = 5)
	{
		this->count = 0;
		this->fragmentMaxSize = fragmentMaxSize;
		this->fragmentPointers = new Node * [2];
		this->fragmentPointers[0] = NULL;
		this->fragmentPointers[1] = NULL;
		this->countFPointers = 2;
	}
	virtual ~FragmentLinkedList()
	{
		this->clear();
		delete[] fragmentPointers;
	}
	virtual void add(const T& element);
	virtual void add(int index, const T& element);
	virtual T removeAt(int index);
	virtual bool removeItem(const T& item);
	virtual bool empty();
	virtual int size();
	virtual void clear();
	virtual T get(int index);
	virtual void set(int index, const T& element);
	virtual int indexOf(const T& item);
	virtual bool contains(const T& item);
	virtual string toString();
	Iterator begin(int index = 0)
	{
		Iterator it = Iterator(index, this, 1);
		if (index == 0)
		{
			it = Iterator(this, 1);
		}
		/*else
		{
			it = Iterator(index, this, 1);
		}*/
		return it;
	}
	Iterator end(int index = -1)
	{
		Iterator it = Iterator(index, this, 0);
		if (index == -1)
		{
			it = Iterator(this, 0);
		}
		/*else
		{
			it = Iterator(index, this, 0);
		}*/
		return it;
	}

public:
	class Node
	{
	private:
		T data;
		Node* next;
		Node* prev;
		friend class FragmentLinkedList<T>;

	public:
		Node()
		{
			next = 0;
			prev = 0;
		}
		Node(Node* next, Node* prev)
		{
			this->next = next;
			this->prev = prev;
		}
		Node(T data, Node* next, Node* prev)
		{
			this->data = data;
			this->next = next;
			this->prev = prev;
		}
	};

	class Iterator
	{
	private:
		FragmentLinkedList<T>* pList;
		Node* pNode;
		int index;

	public:
		Iterator(FragmentLinkedList<T>* pList = 0, bool begin = true);
		Iterator(int fragmentIndex, FragmentLinkedList<T>* pList = 0, bool begin = true);
		Iterator& operator=(const Iterator& iterator);
		T& operator*();
		bool operator!=(const Iterator& iterator);
		void remove();
		void set(const T& element);
		Iterator& operator++();
		Iterator operator++(int);
		Node* get()
		{
			return (this->pNode);
		}
	};
};

template <class T>
string FragmentLinkedList<T>::toString()
{
	stringstream ss;
	ss << "[";
	Node* ptr = this->fragmentPointers[0];

	if (this->count == 0)
		ss << "]";

	// TODO
	while (ptr != NULL)
	{
		if (ptr->next != NULL)
		{
			ss << ptr->data << ", ";
			ptr = ptr->next;
		}
		else
		{
			ss << ptr->data << "]";
			ptr = NULL;
		}
	}
	// END: TODO

	return ss.str();
}

template<class T>
void FragmentLinkedList<T>::add(const T& element)
{
	Node* p = new Node;
	p->data = element;
	if (this->head == NULL)
	{
		head = p;
		tail = p;
		this->count++;
		this->fragmentPointers[0] = p;
		this->fragmentPointers[1] = p;
	}
	else
	{
		p->next = NULL;
		p->prev = this->tail;
		this->tail->next = p;
		this->tail = p;
		this->count++;
		if (((count - 1) / fragmentMaxSize) > (countFPointers - 2))
		{
			countFPointers++;
			fragmentPointers[countFPointers - 2] = p;
			Node** fragmentPointersnew = new Node * [countFPointers];
			for (int i = 0; i < countFPointers - 1; i++)
			{
				fragmentPointersnew[i] = fragmentPointers[i];
			}
			fragmentPointersnew[countFPointers - 1] = p;
			/*delete[] fragmentPointers;

			fragmentPointers = fragmentPointersnew;*/
			delete[] this->fragmentPointers;
			this->fragmentPointers = new Node * [countFPointers];
			this->fragmentPointers = fragmentPointersnew;
		}
		else
		{
			fragmentPointers[countFPointers - 1] = p;
		}
	}
}

template<class T>
void FragmentLinkedList<T>::add(int index, const T& e)
{
	Node* p = new Node;
	p = this->head;
	Node* ptemp = new Node;
	ptemp->data = e;
	if (index < 0)
	{
		throw std::out_of_range("The index is out of range!");
	}
	else if (index == 0)
	{
		if (this->head == NULL)
		{
			this->head = ptemp;
			this->tail = ptemp;
			ptemp->next = NULL;
			ptemp->prev = NULL;
			this->count++;
			this->fragmentPointers[0] = ptemp;
			this->fragmentPointers[1] = ptemp;
		}
		else
		{
			ptemp->next = this->head;
			ptemp->prev = NULL;
			this->head->prev = ptemp;
			this->head = ptemp;
			this->count++;
			this->fragmentPointers[0] = ptemp;

			for (int i = 1; i < countFPointers - 1; i++)
			{
				Iterator it = Iterator(i - 1, this, 1);
				for (int j = 1; j <= (this->fragmentMaxSize); j++)
				{
					it++;
				}
				fragmentPointers[i] = it.get();
			}

			if (((count - 1) / fragmentMaxSize) > (countFPointers - 2))
			{
				countFPointers++;
				Node** fragmentPointersnew = new Node * [countFPointers];
				for (int i = 0; i < countFPointers - 1; i++)
				{
					fragmentPointersnew[i] = fragmentPointers[i];
				}
				fragmentPointersnew[countFPointers - 1] = this->tail;
				/*delete[] fragmentPointers;

				fragmentPointers = fragmentPointersnew;*/
				delete[] this->fragmentPointers;
				this->fragmentPointers = new Node * [countFPointers];
				this->fragmentPointers = fragmentPointersnew;
			}

		}

	}
	else if (index == (this->size()))
	{
		this->add(e);
	}
	else if (index > (this->size()))
	{
		throw std::out_of_range("The index is out of range!");
	}
	else
	{
		int k = 1;
		while (p != NULL && k != index)
		{
			p = p->next;
			k++;
		}
		if (k != index)
		{
			throw std::out_of_range("The index is out of range!");
		}
		else
		{
			ptemp->next = p->next;
			ptemp->next->prev = ptemp;
			p->next = ptemp;
			ptemp->prev = p;
			this->count++;

			int divide = index / fragmentMaxSize, modul = index % fragmentMaxSize;
			if (modul == 0)
			{
				int i = divide;
				fragmentPointers[divide] = ptemp;

				i++;
				for (; i < countFPointers - 1; i++)
				{
					Iterator it = FragmentLinkedList<T>::end(i - 1);
					fragmentPointers[i] = it.get();
				}

				if (((count - 1) / fragmentMaxSize) > (countFPointers - 2))
				{
					countFPointers++;
					Node** fragmentPointersnew = new Node * [countFPointers];
					for (int i = 0; i < countFPointers - 1; i++)
					{
						fragmentPointersnew[i] = fragmentPointers[i];
					}
					fragmentPointersnew[countFPointers - 1] = this->tail;

					delete[] this->fragmentPointers;
					this->fragmentPointers = new Node * [countFPointers];
					this->fragmentPointers = fragmentPointersnew;
				}
			}
			else
			{
				int i = divide + 1;
				for (; i < countFPointers - 1; i++)
				{
					Iterator it = FragmentLinkedList<T>::end(i - 1);
					fragmentPointers[i] = it.get();
				}
				if (((count - 1) / fragmentMaxSize) > (countFPointers - 2))
				{
					countFPointers++;
					Node** fragmentPointersnew = new Node * [countFPointers];
					for (int i = 0; i < countFPointers - 1; i++)
					{
						fragmentPointersnew[i] = fragmentPointers[i];
					}
					fragmentPointersnew[countFPointers - 1] = this->tail;

					delete[] this->fragmentPointers;
					this->fragmentPointers = new Node * [countFPointers];
					this->fragmentPointers = fragmentPointersnew;
				}

			}
		}
	}
}

template<class T>
T FragmentLinkedList<T>::removeAt(int index)
{
	Node* p = new Node;
	p = this->head;
	Node* ptemp = new Node;
	if (index < 0 || index >= this->size())
	{
		throw std::out_of_range("The index is out of range!");
	}
	else if (index == 0)
	{
		ptemp->data = this->head->data;
		if (this->head != this->tail)
		{
			this->head = this->head->next;
			this->head->prev = NULL;
		}
		else
		{
			this->head = NULL;
			this->tail = NULL;
			this->fragmentPointers[0] = NULL;
			this->fragmentPointers[1] = NULL;
		}
		this->count--;

		fragmentPointers[0] = this->head;
		for (int i = 1; i < countFPointers - 1; i++)
		{
			Iterator it = FragmentLinkedList<T>::end(i - 1);
			fragmentPointers[i] = it.get();
			if (i == countFPointers - 2 && fragmentPointers[i] == NULL)
			{
				fragmentPointers[i] = this->tail;
			}
		}
		if (((count - 1) / fragmentMaxSize) < (countFPointers - 2))
		{
			countFPointers--;
			Node** fragmentPointersnew = new Node * [countFPointers];
			for (int i = 0; i < countFPointers; i++)
			{
				fragmentPointersnew[i] = this->fragmentPointers[i];
			}
			/*delete[] fragmentPointers;

			fragmentPointers = fragmentPointersnew;*/
			delete[] this->fragmentPointers;
			this->fragmentPointers = new Node * [countFPointers];
			this->fragmentPointers = fragmentPointersnew;
		}
		delete p;
	}
	else if (index == this->size() - 1)
	{
		int k = 0;
		while (p != NULL && k != index)
		{
			p = p->next;
			k++;
		}
		if (k != index)
		{
			throw std::out_of_range("The index is out of range!");
		}
		else
		{
			ptemp->data = p->data;
			p->prev->next = p->next;
			if (p->next != NULL)
			{
				p->next->prev = p->prev;
			}
			else
			{
				this->tail = p->prev;
				this->tail->next = NULL;
				this->fragmentPointers[countFPointers - 1] = this->tail;
			}
			this->count--;
			int divide = index / fragmentMaxSize;
			int modul = index % fragmentMaxSize;
			if (modul == 0)
			{
				for (int i = divide; i < countFPointers - 1; i++)
				{
					Iterator it = FragmentLinkedList<T>::end(i - 1);
					fragmentPointers[i] = it.get();
					if (countFPointers > 2)
					{
						if (i == countFPointers - 2 && fragmentPointers[i] == NULL)
						{
							fragmentPointers[i] = this->tail;
						}
					}
					else
					{
						if (i == countFPointers - 1 && fragmentPointers[i] == NULL)
						{
							fragmentPointers[i] = this->tail;
						}
					}
				}
				if (((count - 1) / fragmentMaxSize) < (countFPointers - 2))
				{
					/*countFPointers--;*/
					if (this->countFPointers > 2)
					{
						countFPointers--;
						Node** fragmentPointersnew = new Node * [countFPointers];
						for (int i = 0; i < countFPointers; i++)
						{
							fragmentPointersnew[i] = fragmentPointers[i];
						}
						delete[] this->fragmentPointers;
						this->fragmentPointers = new Node * [countFPointers];
						this->fragmentPointers = fragmentPointersnew;
					}
					else
					{
						countFPointers--;
						this->fragmentPointers[countFPointers - 1] = this->tail;
					}
				}
			}
			else
			{
				for (int i = divide + 1; i < countFPointers - 1; i++)
				{
					Iterator it = FragmentLinkedList<T>::end(i - 1);
					fragmentPointers[i] = it.get();
					if (countFPointers > 2)
					{
						if (i == countFPointers - 2 && fragmentPointers[i] == NULL)
						{
							fragmentPointers[i] = this->tail;
						}
					}
					else
					{
						if (i == countFPointers - 1 && fragmentPointers[i] == NULL)
						{
							fragmentPointers[i] = this->tail;
						}
					}
				}
				if (((count - 1) / fragmentMaxSize) < (countFPointers - 2))
				{
					/*countFPointers--;*/
					if (this->countFPointers > 2)
					{
						countFPointers--;
						Node** fragmentPointersnew = new Node * [countFPointers];
						for (int i = 0; i < countFPointers; i++)
						{
							fragmentPointersnew[i] = fragmentPointers[i];
						}
						delete[] this->fragmentPointers;
						this->fragmentPointers = new Node * [countFPointers];
						this->fragmentPointers = fragmentPointersnew;
					}
					else
					{
						countFPointers--;
						this->fragmentPointers[countFPointers - 1] = this->tail;
					}
				}
			}
			delete p;
		}
	}
	else
	{
		int k = 0;
		while (p != NULL && k != index)
		{
			p = p->next;
			k++;
		}
		if (k != index)
		{
			throw std::out_of_range("The index is out of range!");
		}
		else
		{
			ptemp->data = p->data;
			p->prev->next = p->next;
			if (p->next != NULL)
			{
				p->next->prev = p->prev;
			}
			else
			{
				this->tail = p->prev;
				this->tail->next = NULL;
				this->fragmentPointers[countFPointers - 1] = this->tail;
			}
			this->count--;
			int divide = index / fragmentMaxSize;
			int modul = index % fragmentMaxSize;
			if (modul == 0)
			{
				for (int i = divide; i < countFPointers - 1; i++)
				{
					Iterator it = FragmentLinkedList<T>::end(i - 1);
					fragmentPointers[i] = it.get();
					if (countFPointers > 2)
					{
						if (i == countFPointers - 2 && fragmentPointers[i] == NULL)
						{
							fragmentPointers[i] = this->tail;
						}
					}
					else
					{
						if (i == countFPointers - 1 && fragmentPointers[i] == NULL)
						{
							fragmentPointers[i] = this->tail;
						}
					}
				}
				if (((count - 1) / fragmentMaxSize) < (countFPointers - 2))
				{
					/*countFPointers--;*/
					if (this->countFPointers > 2)
					{
						countFPointers--;
						Node** fragmentPointersnew = new Node * [countFPointers];
						for (int i = 0; i < countFPointers; i++)
						{
							fragmentPointersnew[i] = fragmentPointers[i];
						}
						delete[] this->fragmentPointers;
						this->fragmentPointers = new Node * [countFPointers];
						this->fragmentPointers = fragmentPointersnew;
					}
					else
					{
						countFPointers--;
						this->fragmentPointers[countFPointers - 1] = this->tail;
					}
				}
			}
			else
			{
				for (int i = divide + 1; i < countFPointers - 1; i++)
				{
					Iterator it = FragmentLinkedList<T>::end(i - 1);
					fragmentPointers[i] = it.get();
					if (countFPointers > 2)
					{
						if (i == countFPointers - 2 && fragmentPointers[i] == NULL)
						{
							fragmentPointers[i] = this->tail;
						}
					}
					else
					{
						if (i == countFPointers - 1 && fragmentPointers[i] == NULL)
						{
							fragmentPointers[i] = this->tail;
						}
					}
				}
				if (((count - 1) / fragmentMaxSize) < (countFPointers - 2))
				{
					/*countFPointers--;*/
					if (this->countFPointers > 2)
					{
						countFPointers--;
						Node** fragmentPointersnew = new Node * [countFPointers];
						for (int i = 0; i < countFPointers; i++)
						{
							fragmentPointersnew[i] = fragmentPointers[i];
						}
						delete[] this->fragmentPointers;
						this->fragmentPointers = new Node * [countFPointers];
						this->fragmentPointers = fragmentPointersnew;
					}
					else
					{
						countFPointers--;
						this->fragmentPointers[countFPointers - 1] = this->tail;
					}
				}
			}
			delete p;
		}
	}
	return ptemp->data;
}

template<class T>
int FragmentLinkedList<T>::indexOf(const T& item)
{
	Node* p = new Node;
	p = this->head;
	int k = 0;
	while (p != NULL)
	{
		if (p->data == item)
		{
			return k;
		}
		k++;
		p = p->next;
	}
	return -1;
}

template<class T>
bool FragmentLinkedList<T>::removeItem(const T& item)
{
	int k = indexOf(item);
	if (k != -1)
	{
		removeAt(k);
		return true;
	}
	else
		return false;
}

template<class T>
bool FragmentLinkedList<T>::empty()
{
	return (head == NULL);
}

template<class T>
int FragmentLinkedList<T>::size()
{
	Node* pHead = new Node;
	pHead = this->head;
	Node* pTail = new Node;
	pTail = this->tail;
	int i = 0;
	if (pHead != NULL)
		i = 1;
	while (pHead != NULL)
	{
		if (pHead == pTail)
			break;
		pHead = pHead->next;
		i++;
		if (pHead == pTail)
			break;
		pTail = pTail->prev;
		i++;
	}
	return i;
}

template<class T>
void FragmentLinkedList<T>::clear()
{
	if (this->head != NULL)
	{
		Node* p = new Node;
		p = this->head;
		while (p != NULL)
		{
			this->removeAt(0);
			p = this->head;
		}
		this->head = NULL;
		this->tail = NULL;
		this->count = 0;
		delete[] fragmentPointers;
		this->fragmentPointers = new Node * [2];
		fragmentPointers[0] = NULL;
		fragmentPointers[1] = NULL;
	}
}

template<class T>
T FragmentLinkedList<T>::get(int index)
{
	if (index < 0 || index >= this->size())
		throw std::out_of_range("The index is out of range!");
	int i = 0;
	Node* p = new Node;
	p = this->head;
	while (p != NULL && i != index)
	{
		p = p->next;
		i++;
	}
	return (p->data);
}

template<class T>
bool FragmentLinkedList<T>::contains(const T& item)
{
	int x = (this->indexOf(item));
	if (x != -1)
		return true;
	else return false;
}

template<class T>
void FragmentLinkedList<T>::set(int index, const T& element)
{
	if (index < 0 || index >= this->size())
	{
		throw std::out_of_range("The index is out of range!");
	}
	else
	{
		int i = 0;
		Node* p = new Node;
		p = this->head;
		while (i != index)
		{
			p = p->next;
			i++;
		}
		p->data = element;
	}
}

template <class T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T>* pList, bool begin)
{
	this->pList = pList;
	if (begin == true)
	{
		if (pList != NULL)
		{
			this->pNode = pList->head;
			index = 0;
		}
		else
		{
			this->pNode = NULL;
			index = -1;
		}
	}
	else
	{
		this->pNode = NULL;
		if (pList != NULL)
		{
			index = pList->count;
		}
		else
		{
			index = 0;
		}
	}
}

template <class T>
FragmentLinkedList<T>::Iterator::Iterator(int fragmentIndex, FragmentLinkedList<T>* pList, bool begin)
{
	this->pList = pList;
	if (begin == true)
	{
		Node* ptemp = new Node;
		ptemp = this->pList->head;
		int i = 0;
		if (fragmentIndex == (this->pList->countFPointers) - 1)
		{
			this->pNode = this->pList->tail;
			this->index = this->pList->count - 1;
		}
		else
		{
			while ((i != fragmentIndex * this->pList->fragmentMaxSize) && (ptemp != NULL))
			{
				ptemp = ptemp->next;
				i++;
			}
			if (i == fragmentIndex * this->pList->fragmentMaxSize)
			{
				this->pNode = ptemp;
				this->index = i;
			}
			else
			{
				throw std::out_of_range("The index is out of range!");
			}
		}
	}
	else
	{
		Node* ptemp = new Node;
		ptemp = this->pList->head;
		int i = 0;
		while ((i != (fragmentIndex + 1) * this->pList->fragmentMaxSize) && (ptemp != NULL))
		{
			ptemp = ptemp->next;
			i++;
		}
		if (i == (fragmentIndex + 1) * this->pList->fragmentMaxSize)
		{
			this->pNode = ptemp;
			this->index = i;
		}
		else
		{
			if (this->pList->head->next == this->pList->tail || this->pList->head == this->pList->tail)
			{
				this->pNode = ptemp;
				this->index = i;
			}
			else
			{
				throw std::out_of_range("The index is out of range!");
			}
		}
	}

}

template <class T>
typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T>::Iterator::operator=(const Iterator& iterator)
{
	this->pNode = iterator.pNode;
	this->pList = iterator.pList;
	this->index = iterator.index;
	return *this;
}

template <class T>
void FragmentLinkedList<T>::Iterator::set(const T& e)
{
	if (this->pNode != NULL)
	{
		this->pNode->data = e;
	}
	else
	{
		throw std::out_of_range("Segmentation fault!");
	}
}

template<class T>
T& FragmentLinkedList<T>::Iterator::operator*()
{
	if (this->pNode != NULL)
	{
		return (this->pNode->data);
	}
	else
	{
		throw std::out_of_range("Segmentation fault!");
	}
}

template<class T>
void FragmentLinkedList<T>::Iterator::remove()
{
	if (this->pNode == NULL)
	{
		throw std::out_of_range("Segmentation fault!");
	}
	else
	{
		if (this->index == 0)
		{
			this->pList->removeAt(0);
			this->pNode = NULL;
			this->index = -1;
		}
		else
		{
			Node* ptemp = new Node;
			ptemp = this->pList->head;
			int i = 0;
			while (ptemp != this->pNode)
			{
				ptemp = ptemp->next;
				i++;
			}
			this->index--;
			this->pNode = this->pNode->prev;
			this->pList->removeAt(i);
		}
	}
}

template<class T>
bool FragmentLinkedList<T>::Iterator::operator!=(const FragmentLinkedList::Iterator& iterator)
{
	if ((this->pNode == iterator.pNode) && (this->index == iterator.index))
		return false;
	else
		return true;
}

template<class T>
typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T>::Iterator::operator++()
{
	if (this->pNode != NULL)
	{
		pNode = pNode->next;
		this->index = this->pList->count;
		return *this;
	}
	else
	{
		if (this->index == -1)
		{
			this->pNode = this->pList->head;
			this->index = 0;
			return *this;
		}
		else
		{
			throw std::out_of_range("Segmentation fault!");
		}
	}
}

template<class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::Iterator::operator++(int)
{
	Iterator ptemp = *this;
	if (this->pNode != NULL)
	{
		this->pNode = this->pNode->next;
		this->index = this->pList->count;
		return ptemp;
	}
	else
	{
		if (this->index == -1)
		{
			this->pNode = this->pList->head;
			this->index = 0;
			return *this;
		}
		else
		{
			throw std::out_of_range("Segmentation fault!");
		}
	}
}

#endif
// END: STUDENT ANSWER

int main()
{
	FragmentLinkedList<int> fList;
	for (int i = 0; i < 9; i++)
		fList.add(i, i * i);
	fList.add(4, 1000);

	FragmentLinkedList<int>::Iterator ittemp = fList.end(0);
	ittemp.set(100);
 	cout << *(ittemp) << endl;
	
	

	for (FragmentLinkedList<int>::Iterator it = fList.begin(); it != fList.end(); it++)
		cout << *it << " ";
	cout << endl;
	cout << fList.toString();

	/*FragmentLinkedList<int> list;
	cout << list.size();*/

	return 0;
}