#include <iostream>
#include <queue>
#include <vector>
#include <sstream>
using namespace std;

stringstream strList;

void printKey(int key, int value)
{
	cout << key << endl;
}

template <class K, class V>
class BKUTree
{
public:
	class AVLTree;
	class SplayTree;

	class Entry {
	public:
		K key;
		V value;

		Entry(K key, V value) : key(key), value(value) {}
	};


public:
	AVLTree* avl;
	SplayTree* splay;
	queue<K> keys;
	int maxNumOfKeys;
	vector<K> list;
	queue<K> keyList;

public:
	BKUTree(int maxNumOfKeys = 5)
	{
		this->maxNumOfKeys = maxNumOfKeys;
		this->avl = new AVLTree();
		this->splay = new SplayTree();
		//this->list.clear();
	}
	~BKUTree() { this->clear(); }

	void add(K key, V value);
	void remove(K key);
	V search(K key, vector<K>& traversedList);

	void getCorr();

	void traverseNLROnAVL(void (*func)(K key, V value));
	void traverseNLROnSplay(void (*func)(K key, V value));
	void clear();

	void checkNode(vector<K>& nodeAVL, vector<K>& nodeSPLAY) {
		if (avl == NULL) return;
		checkTrueNode(nodeAVL, nodeSPLAY, avl->root);
	}
	void checkTrueNode(vector<K>& nodeAVL, vector<K>& nodeSPLAY, typename AVLTree::Node* f) {
		if (!f)  return;
		typename SplayTree::Node* r = f->corr;
		if (r->entry->key != f->entry->key) {
			nodeAVL.push_back(f->entry->key);
			nodeSPLAY.push_back(r->entry->key);
		}
		checkTrueNode(nodeAVL, nodeSPLAY, f->left);
		checkTrueNode(nodeAVL, nodeSPLAY, f->right);
	}

	class SplayTree
	{
	public:
		class Node
		{
		public:
			Entry* entry;
			Node* left;
			Node* right;
			Node* parent;
			typename AVLTree::Node* corr;

			Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL, Node* parent = NULL)
			{
				this->entry = entry;
				this->left = left;
				this->right = right;
				this->parent = parent;
				this->corr = NULL;
			}
		};

	public:
		Node* root;
		Node* nodetoSearch = NULL;
		vector<K> splayList;

		SplayTree() : root(NULL) {};
		~SplayTree() { this->clear(); };


		void rightRotate(Node* x)
		{
			Node* y = x->left;
			x->left = y->right;
			if (y->right != NULL)
			{
				y->right->parent = x;
			}
			y->parent = x->parent;
			if (x->parent == NULL)
			{
				root = y;
			}
			else if (x == x->parent->right)
			{
				x->parent->right = y;
			}
			else
			{
				x->parent->left = y;
			}
			y->right = x;
			x->parent = y;
		}

		void leftRotate(Node* x)
		{
			Node* y = x->right;
			x->right = y->left;
			if (y->left != NULL)
			{
				y->left->parent = x;
			}
			y->parent = x->parent;
			if (x->parent == NULL)
			{
				root = y;
			}
			else if (x == x->parent->left)
			{
				x->parent->left = y;
			}
			else
			{ //x is right child
				x->parent->right = y;
			}
			y->left = x;
			x->parent = y;
		}

		void Splay(Node* n)
		{
			while (n->parent != NULL)
			{
				if (n->parent == root || n->parent->parent == NULL)
				{
					if (n == n->parent->left)
					{
						rightRotate(n->parent);
					}
					else
					{
						leftRotate(n->parent);
					}
				}
				else
				{
					Node* p = n->parent;
					Node* g = p->parent; //grandparent

					if (n->parent->left == n && p->parent->left == p)
					{
						rightRotate(g);
						rightRotate(p);
					}
					else if (n->parent->right == n && p->parent->right == p)
					{
						leftRotate(g);
						leftRotate(p);
					}
					else if (n->parent->right == n && p->parent->left == p)
					{
						leftRotate(p);
						rightRotate(g);
					}
					else if (n->parent->left == n && p->parent->right == p)
					{
						rightRotate(p);
						leftRotate(g);
					}
				}
			}
		}

		void insert(Node* rootNode, Node* n)
		{
			Node* y = NULL;
			Node* temp = rootNode;
			while (temp != NULL)
			{
				y = temp;
				if (n->entry->key < temp->entry->key)
					temp = temp->left;
				else if (n->entry->key > temp->entry->key)
					temp = temp->right;
				else
					throw ("Duplicate key");
			}
			n->parent = y;

			if (y == NULL)
				root = n;
			else if (n->entry->key < y->entry->key)
				y->left = n;
			else if (n->entry->key > y->entry->key)
				y->right = n;
			else
				throw ("Duplicate key");

			Splay(n);
		}

		Node* getSplayNode(Node* rootNode, K key)
		{
			while (rootNode != NULL)
			{
				if (key < rootNode->entry->key)
				{
					rootNode = rootNode->left;
				}
				else if (key > rootNode->entry->key)
				{
					rootNode = rootNode->right;
				}
				else
				{
					return rootNode;
				}
			}
			return rootNode;
		}

		void add(K key, V value)
		{
			Entry* ent = new Entry(key, value);
			Node* p = new Node(ent);
			insert(root, p);
		}
		void add(Entry* entry)
		{
			Node* p = new Node(entry);
			insert(root, p);
		}

		Node* maxValueNode(Node* node)
		{
			Node* current = node;
			while (current->right != NULL)
				current = current->right;

			return current;
		}

		void deleteNode(K key)
		{
			Node* del = this->root;

			while (del != NULL)
			{
				if (key < del->entry->key)
					del = del->left;
				else if (key > del->entry->key)
					del = del->right;
				else
					break;
			}

			if (del == NULL)
				throw ("Not found");

			Splay(del);
			this->root = NULL;
			Node* L = del->left;
			if (L != NULL) L->parent = NULL;
			Node* R = del->right;
			if (R != NULL) R->parent = NULL;

			delete del;

			if (L == NULL && R == NULL)
			{
				this->root = NULL;
			}
			else if (L == NULL)
			{
				R->parent = NULL;
				this->root = R;
			}
			else if (R == NULL)
			{
				Node* M = maxValueNode(L);
				Splay(M);
				M->right = NULL;
				this->root = M;
			}
			else
			{
				Node* M = maxValueNode(L);
				Splay(M);
				M->right = R;
				R->parent = M;
				M->parent = NULL;
				this->root = M;
			}
		}

		void remove(K key)
		{
			deleteNode(key);
		}

		void splayforSearch(Node* n)
		{
			if (n->parent != NULL)
			{
				if (n->parent == root || n->parent->parent == NULL)
				{
					if (n == n->parent->left)
					{
						rightRotate(n->parent);
					}
					else
					{
						leftRotate(n->parent);
					}
				}
				else
				{
					Node* p = n->parent;
					Node* g = p->parent; //grandparent

					if (n->parent->left == n && p->parent->left == p)
					{
						rightRotate(g);
						rightRotate(p);
					}
					else if (n->parent->right == n && p->parent->right == p)
					{
						leftRotate(g);
						leftRotate(p);
					}
					else if (n->parent->right == n && p->parent->left == p)
					{
						leftRotate(p);
						rightRotate(g);
					}
					else if (n->parent->left == n && p->parent->right == p)
					{
						rightRotate(p);
						leftRotate(g);
					}
				}
			}
		}

		V findforBKU(Node* rootNode, K key)
		{
			Node* temp = rootNode;
			while (temp != NULL)
			{
				if (temp->entry->key != key)
				{
					splayList.push_back(temp->entry->key);
				}
				if (key < temp->entry->key)
				{
					temp = temp->left;
				}
				else if (key > temp->entry->key)
				{
					temp = temp->right;
				}
				else
				{
					break;
				}
			}

			if (temp == NULL)
				throw ("Not found");

			splayforSearch(temp);
			return temp->entry->value;
		}

		V find(Node* rootNode, K key)
		{
			Node* temp = rootNode;
			while (temp != NULL)
			{
				if (temp->entry->key != key)
				{
					splayList.push_back(temp->entry->key);
				}
				if (key < temp->entry->key)
				{
					temp = temp->left;
				}
				else if (key > temp->entry->key)
				{
					temp = temp->right;
				}
				else
				{
					break;
				}
			}

			if (temp == NULL)
				throw ("Not found");

			Splay(temp);
			return temp->entry->value;
		}

		V search(K key)
		{
			return find(this->root, key);
		}

		void preOrder(void(*func)(K key, V value), Node* currentNode)
		{
			if (currentNode != NULL)
			{
				func(currentNode->entry->key, currentNode->entry->value);
			}
			else
			{
				return;
			}
			if (currentNode != NULL)
			{
				preOrder((*func), currentNode->left);
				preOrder((*func), currentNode->right);
			}
		}

		void traverseNLR(void(*func)(K key, V value))
		{
			preOrder((*func), this->root);
		}

		void makeEmpty(Node* temp)
		{
			if (temp == NULL)
				return;

			makeEmpty(temp->left);
			makeEmpty(temp->right);
			delete temp;
		}

		void deleteTree(Node** rootNode)
		{
			makeEmpty(*rootNode);
			*rootNode = NULL;
		}

		void deleteAdditionNode()
		{
			//delete nodetoSearch;
			this->nodetoSearch = NULL;
			this->splayList.clear();
		}

		void clear()
		{
			deleteTree(&(this->root));
			deleteAdditionNode();
		}
	};

	class AVLTree
	{
	public:
		class Node
		{
		public:
			Entry* entry;
			Node* left;
			Node* right;
			int balance;
			int height;
			typename SplayTree::Node* corr;

			Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL)
			{
				this->entry = entry;
				this->left = left;
				this->right = right;
				this->balance = 0;
				this->height = 1;
				this->corr = NULL;
			}
		};

	public:
		Node* root;
		Node* firstSearch = NULL;
		Node* secondSearch = NULL;
		Node* nodetoSearch = NULL;
		vector<K> AVLList;

		AVLTree() : root(NULL) {};
		~AVLTree() { this->clear(); };

		int getHeightRec(Node* node)
		{
			if (node == NULL)
				return 0;
			int lh = this->getHeightRec(node->left);
			int rh = this->getHeightRec(node->right);
			return (lh > rh ? lh : rh) + 1;
		}

		int max(int a, int b)
		{
			return (a > b) ? a : b;
		}

		Node* rightRotate(Node* y)
		{
			Node* x = y->left;
			Node* T2 = x->right;

			// Perform rotation  
			x->right = y;
			y->left = T2;

			y->balance = getBalance(y);
			x->balance = getBalance(x);

			return x;
		}

		Node* leftRotate(Node* x)
		{
			Node* y = x->right;
			Node* T2 = y->left;

			// Perform rotation  
			y->left = x;
			x->right = T2;

			x->balance = getBalance(x);
			y->balance = getBalance(y);

			return y;
		}

		int getBalance(Node* N)
		{
			if (N == NULL)
				return 0;
			return getHeightRec(N->left) - getHeightRec(N->right);
		}


		Node* getAVLNode(Node* rootNode, K key)
		{
			while (rootNode != NULL)
			{
				if (key < rootNode->entry->key)
				{
					rootNode = rootNode->left;
				}
				else if (key > rootNode->entry->key)
				{
					rootNode = rootNode->right;
				}
				else
				{
					return rootNode;
				}
			}
			if (rootNode == NULL)
			{
				throw ("Not found");
			}
			return rootNode;
		}

		Node* getAVLNodeforSearch(Node* rootNode, K key)
		{
			while (rootNode != NULL)
			{
				if (key != rootNode->entry->key)
				{
					this->AVLList.push_back(rootNode->entry->key);
				}
				if (key < rootNode->entry->key)
				{
					rootNode = rootNode->left;
				}
				else if (key > rootNode->entry->key)
				{
					rootNode = rootNode->right;
				}
				else
				{
					return rootNode;
				}

				if (rootNode == firstSearch)
				{
					//this->AVLList.push_back(rootNode->entry->key);
					throw ("Not found");
					return NULL;
				}

			}
			if (rootNode == NULL)
			{
				throw ("Not found");
				return NULL;
			}
			return rootNode;
		}

		Node* insert(Node* rootNode, Node* p)
		{
			if (rootNode == NULL)
				return p;

			if (p->entry->key < rootNode->entry->key)
				rootNode->left = insert(rootNode->left, p);
			else if (p->entry->key > rootNode->entry->key)
				rootNode->right = insert(rootNode->right, p);
			else // Equal keys are not allowed in BST  
				throw ("Duplicate key");

			int checkbalance = getBalance(rootNode);
			rootNode->balance = getBalance(rootNode);

			// Left Left Case  
			if (checkbalance > 1 && p->entry->key < rootNode->left->entry->key)
				return rightRotate(rootNode);

			// Right Right Case  
			if (checkbalance < -1 && p->entry->key > rootNode->right->entry->key)
				return leftRotate(rootNode);

			// Left Right Case  
			if (checkbalance > 1 && p->entry->key > rootNode->left->entry->key)
			{
				rootNode->left = leftRotate(rootNode->left);
				return rightRotate(rootNode);
			}

			// Right Left Case  
			if (checkbalance < -1 && p->entry->key < rootNode->right->entry->key)
			{
				rootNode->right = rightRotate(rootNode->right);
				return leftRotate(rootNode);
			}
			return rootNode;
		}

		void add(K key, V value)
		{
			Entry* ent = new Entry(key, value);
			Node* p = new Node(ent);
			this->root = insert(this->root, p);
		}

		void add(Entry* entry)
		{
			Node* p = new Node(entry);
			this->root = insert(this->root, p);
		}

		Node* maxValueNode(Node* node)
		{
			Node* current = node;
			while (current->right != NULL)
				current = current->right;

			return current;
		}

		Node* deleteNode(Node* rootNode, K key)
		{
			if (rootNode == NULL)
			{
				throw ("Not found");
			}

			if (key < rootNode->entry->key)
				rootNode->left = deleteNode(rootNode->left, key);
			else if (key > rootNode->entry->key)
				rootNode->right = deleteNode(rootNode->right, key);
			else
			{
				if ((rootNode->left == NULL) || (rootNode->right == NULL))
				{
					Node* temp = rootNode->left ? rootNode->left : rootNode->right;
					if (temp == NULL)
					{
						temp = rootNode;
						rootNode = NULL;
					}
					else
						*rootNode = *temp;
					free(temp);
				}
				else
				{
					Node* temp = maxValueNode(rootNode->left);
					rootNode->entry->key = temp->entry->key;
					rootNode->entry->value = temp->entry->value;
					rootNode->corr = temp->corr;

					rootNode->left = deleteNode(rootNode->left, temp->entry->key);
				}
			}

			if (rootNode == NULL)
				return rootNode;

			rootNode->balance = getBalance(rootNode);
			int balance = getBalance(rootNode);

			if (balance > 1 && getBalance(rootNode->left) >= 0)
				return rightRotate(root);

			// Left Right Case  
			if (balance > 1 && getBalance(rootNode->left) < 0)
			{
				rootNode->left = leftRotate(rootNode->left);
				return rightRotate(rootNode);
			}

			// Right Right Case  
			if (balance < -1 && getBalance(rootNode->right) <= 0)
				return leftRotate(rootNode);

			// Right Left Case  
			if (balance < -1 && getBalance(rootNode->right) > 0)
			{
				rootNode->right = rightRotate(rootNode->right);
				return leftRotate(rootNode);
			}

			return rootNode;
		}

		void remove(K key)
		{
			this->root = deleteNode(this->root, key);
		}

		V find(Node* rootNode, K key)
		{
			if (rootNode != root)
			{
				//firstSearch is the node r'
				firstSearch = rootNode;
			}
			Node* temproot = rootNode;
			try
			{
				Node* temp = rootNode;
				while (temp != NULL)
				{
					if (temp->entry->key != key)
					{
						AVLList.push_back(temp->entry->key);
					}
					if (key < temp->entry->key)
					{
						temp = temp->left;
					}
					else if (key > temp->entry->key)
					{
						temp = temp->right;
					}
					else
					{
						break;
					}
					if (temp == firstSearch) //in step 3d if we pass the node r' means "not found"
					{
						firstSearch = NULL;
						throw ("Not found");
					}
				}
				if (temp == NULL)
					throw ("Not found");

				this->firstSearch = temp;
				return temp->entry->value;
			}
			catch (...)
			{
				if (temproot == root) //for normal case when we start from root to search
				{
					throw ("Not found");
				}
				else
				{
					//find(this->root, key);

					Node* p1 = getAVLNodeforSearch(root, key);
					secondSearch = p1;
					return p1->entry->value;

					/*secondSearch = p1;
					return p1->entry->value;*/
				}
			}

		}

		V search(K key)
		{
			return find(root, key);
		}

		void preOrder(void(*func)(K key, V value), Node* currentNode)
		{
			if (currentNode != NULL)
			{
				func(currentNode->entry->key, currentNode->entry->value);
			}
			else
			{
				return;
			}
			if (currentNode != NULL)
			{
				preOrder((*func), currentNode->left);
				preOrder((*func), currentNode->right);
			}
		}

		void traverseNLR(void(*func)(K key, V value))
		{
			preOrder((*func), this->root);
		}

		void makeEmpty(Node* temp)
		{
			if (temp == NULL)
				return;

			makeEmpty(temp->left);
			makeEmpty(temp->right);
			delete temp;
		}

		void deleteTree(Node** rootNode)
		{
			makeEmpty(*rootNode);
			*rootNode = NULL;
		}

		void deleteAdditionNode()
		{
			//delete firstSearch;
			this->firstSearch = NULL;
			//delete secondSearch;
			this->secondSearch = NULL;
			//delete nodetoSearch;
			this->nodetoSearch = NULL;
			this->AVLList.clear();
		}

		void clear()
		{
			deleteTree(&(this->root));
			deleteAdditionNode();
		}
	};
};

template<class K, class V>
void BKUTree<K, V>::add(K key, V value)
{
	this->splay->add(key, value);
	this->avl->add(key, value);
	if (int(this->keys.size()) >= this->maxNumOfKeys)
	{
		this->keys.pop();
	}
	this->keys.push(key);
	this->keyList.push(key);
	this->avl->getAVLNode(this->avl->root, key)->corr = this->splay->getSplayNode(this->splay->root, key);
	this->splay->getSplayNode(this->splay->root, key)->corr = this->avl->getAVLNode(this->avl->root, key);
}

template<class K, class V>
void BKUTree<K, V>::getCorr()
{
	queue<K> tempkeylist;
	while (!this->keyList.empty())
	{
		K tempKey = this->keyList.front();
		this->avl->getAVLNode(this->avl->root, tempKey)->corr = this->splay->getSplayNode(this->splay->root, tempKey);
		this->splay->getSplayNode(this->splay->root, tempKey)->corr = this->avl->getAVLNode(this->avl->root, tempKey);
		tempkeylist.push(tempKey);
		this->keyList.pop();
	}
	while (!tempkeylist.empty())
	{
		this->keyList.push(tempkeylist.front());
		tempkeylist.pop();
	}
}

template<class K, class V>
void BKUTree<K, V>::remove(K key)
{
	this->splay->remove(key);
	this->avl->remove(key);

	int size = this->keys.size();
	queue<K> tempKeys;
	while (!this->keys.empty())
	{
		K tempkey = this->keys.front();
		if (tempkey != key)
		{
			tempKeys.push(tempkey);
		}
		this->keys.pop();
	}

	if (size != tempKeys.size())
	{
		while (!tempKeys.empty())
		{
			K tempkey = tempKeys.front();
			this->keys.push(tempkey);
			tempKeys.pop();
		}
		if (int(this->keys.size()) >= this->maxNumOfKeys)
		{
			this->keys.pop();
		}
		this->keys.push(this->splay->root->entry->key);
	}
	else
	{
		while (!tempKeys.empty())
		{
			this->keys.push(tempKeys.front());
			tempKeys.pop();
		}
		if (int(this->keys.size()) >= this->maxNumOfKeys)
		{
			this->keys.pop();
		}
		this->keys.push(this->splay->root->entry->key);
	}

	queue<K> tempkeylist;
	while (!this->keyList.empty())
	{
		K tempkey = this->keyList.front();
		if (tempkey != key)
		{
			tempkeylist.push(tempkey);
		}
		this->keyList.pop();
	}

	while (!tempkeylist.empty())
	{
		this->keyList.push(tempkeylist.front());
		tempkeylist.pop();
	}
	getCorr();
}

template<class K, class V>
V BKUTree<K, V>::search(K key, vector<K>& traversedList)
{
	if (key == this->splay->root->entry->key)	//If key is the root of splay tree
	{
		//traversedList.push_back(key);
		if (int(this->keys.size()) >= this->maxNumOfKeys)
		{
			this->keys.pop();
		}
		this->keys.push(key);
		return this->splay->root->entry->value;
	}
	queue<K> tempKeys;
	bool check = 0;
	while (!this->keys.empty())
	{
		K tempkey = this->keys.front();
		this->keys.pop();
		if (tempkey == key)
		{
			check = 1;
		}
		tempKeys.push(tempkey);
	}
	while (!tempKeys.empty())
	{
		this->keys.push(tempKeys.front());
		tempKeys.pop();
	}
	if (check == 1)	//Else if key is contained in queue
	{
		//V value = this->splay->search(key);
		V value = this->splay->findforBKU(this->splay->root, key);
		traversedList.swap(this->splay->splayList);
		this->splay->splayList.clear();
		if (int(this->keys.size()) >= this->maxNumOfKeys)
		{
			this->keys.pop();
		}
		this->keys.push(key);
		return value;
	}
	else
	{
		//this->avl->nodetoSearch->corr = this->splay->getSplayNode(this->splay->root, this->splay->root->entry->key);
		this->avl->nodetoSearch = this->splay->root->corr;
		V value;
		try
		{
			value = this->avl->find(this->avl->nodetoSearch, key);
		}
		catch (...)
		{
			traversedList.swap(this->avl->AVLList);
			this->avl->AVLList.clear();
			throw("Not found");
		}

		if (this->avl->secondSearch != NULL)
		{
			//this->splay->nodetoSearch = this->avl->getAVLNode(this->avl->root, this->avl->secondSearch->entry->key);
			this->splay->nodetoSearch = this->avl->secondSearch->corr;
			/*this->avl->splayforSearch(this->splay->nodetoSearch);*/
			this->splay->splayforSearch(this->splay->nodetoSearch);
			this->avl->secondSearch = NULL;
		}
		else
		{
			this->splay->nodetoSearch = this->avl->firstSearch->corr;
			this->splay->splayforSearch(this->splay->nodetoSearch);
		}
		this->splay->nodetoSearch = NULL;
		this->avl->firstSearch = NULL;
		this->avl->nodetoSearch = NULL;

		traversedList.swap(this->avl->AVLList);
		this->avl->AVLList.clear();
		if (int(this->keys.size()) >= this->maxNumOfKeys)
		{
			this->keys.pop();
		}
		this->keys.push(key);
		return value;
	}
}

template<class K, class V>
void BKUTree<K, V>::traverseNLROnAVL(void (*func)(K key, V value))
{
	this->avl->traverseNLR(func);
}

template<class K, class V>
void BKUTree<K, V>::traverseNLROnSplay(void (*func)(K key, V value))
{
	this->splay->traverseNLR(func);
}

template<class K, class V>
void BKUTree<K, V>::clear()
{
	this->avl->clear();
	this->splay->clear();
	this->list.clear();
}

template <class K, class V>
void printData(K key, V value) {
	cout << key << " ";
}

string transVector(vector<int> data) {
	stringstream ss;
	for (auto it = data.begin(); it != data.end(); it++) {
		ss << *it;
		ss << ',';
	}
	string re = ss.str();
	re[re.size() - 1] = '\0';
	return re;
}

int main() {
	BKUTree<int, int> myTree;
	// TEST ADD 1
	//{
	//	int arr[30] = { 79, 128, 64, 114, 137, 34, 47, 131, 88, 123, 89, 76, 114, 38, 12, 142, 123, 44, 114, 12, 110, 75, 27, 104, 13, 120, 113, 44, 64, 18 };
	//	cout << "\n=====================TEST ADD 1====-================\n";
	//	vector<int> fail;
	//	for (int i = 0; i < 30; i++) {
	//		try { myTree.add(arr[i], arr[i] * 2); }
	//		catch (const char* msg) {
	//			string data = "Duplicate key";
	//			bool yes = 1;
	//			int size = data.size();
	//			for (int i = 0; i < size; i++)  if (msg[i] != data[i]) { yes = 0;  break; }
	//			if (yes == 1)    fail.push_back(arr[i]);
	//		}
	//	}
	//	stringstream list;
	//	if (fail.size() != 0) {
	//		cout << "List fail add :";
	//		list << "List fail add :";
	//		for (int i = 0; i < fail.size(); i++) { cout << " " << fail[i];  list << " ";  list << fail[i]; }
	//		cout << endl;
	//	}
	//	cout << "List key error: 114 123 114 12 44 64\n";
	//	string finalCheck = list.str();
	//	string check = "List fail add : 114 123 114 12 44 64";
	//	for (int i = 0; i < check.size(); i++)    if (finalCheck[i] != check[i])    throw std::out_of_range("FAIL ADD DATA _ SAME KEY INT TREE");
	//	vector<int> AVL;
	//	vector<int> SPLAY;
	//	myTree.checkNode(AVL, SPLAY);
	//	if (AVL.size() != 0) { for (int i = 0; i < AVL.size(); i++)  cout << AVL[i] << " -- " << SPLAY[i] << endl;  throw std::out_of_range("FAIL ADD 1 _ CORR MISS NODE"); }
	//	else    cout << "GOOD CORR\n"; // GOOD CORR
	//}
	//// TEST DELETE 1
	//{
	//	cout << "\n===================TEST DELETE 1===================\n";
	//	vector<int> fail;
	//	int arr[15] = { 3, 44, 38, 132, 79, 114, 150, 34, 113, 89, 149, 110, 27, 137, 137 };
	//	for (int i = 0; i < 15; i++) {
	//		try { myTree.remove(arr[i]); }
	//		catch (const char* msg) {
	//			string data = "Not found";
	//			bool yes = 1;
	//			int size = data.size();
	//			for (int i = 0; i < size; i++)  if (msg[i] != data[i]) { yes = 0;  break; }
	//			if (yes == 1)    fail.push_back(arr[i]);
	//		}
	//	}
	//	stringstream list;
	//	if (fail.size() != 0) {
	//		cout << "List not found :";
	//		list << "List not found :";
	//		for (int i = 0; i < fail.size(); i++) { cout << " " << fail[i];  list << " ";  list << fail[i]; }
	//		cout << endl;
	//	}
	//	cout << "List not found : 3 132 150 149 137\n";
	//	string finalCheck = list.str();
	//	string check = "List not found : 3 132 150 149 137";
	//	for (int i = 0; i < check.size(); i++)    if (finalCheck[i] != check[i])    throw std::out_of_range("FAIL DELETE DATA _ SAME KEY INT TREE");
	//	vector<int> AVL;
	//	vector<int> SPLAY;
	//	myTree.checkNode(AVL, SPLAY);
	//	if (AVL.size() != 0) { for (int i = 0; i < AVL.size(); i++)  cout << AVL[i] << " -- " << SPLAY[i] << endl;  throw std::out_of_range("FAIL DELETE 1 _ CORR MISS NODE"); }
	//	else    cout << "GOOD CORR\n"; // GOOD CORR    
	//}
	//// TEST ADD 2
	//{
	//	int arr[10] = { 6, 42, 34, 99, 145, 19, 116, 7, 15, 24 };
	//	cout << "\n=====================TEST ADD 2=====================\n";
	//	vector<int> fail;
	//	for (int i = 0; i < 10; i++) {
	//		try { myTree.add(arr[i], arr[i] * 2); }
	//		catch (const char* msg) {
	//			string data = "Duplicate key";
	//			bool yes = 1;
	//			int size = data.size();
	//			for (int i = 0; i < size; i++)  if (msg[i] != data[i]) { yes = 0;  break; }
	//			if (yes == 1)    fail.push_back(arr[i]);
	//		}
	//	}
	//	if (fail.size() != 0) {
	//		cout << "List fail add :";
	//		for (int i = 0; i < fail.size(); i++)  cout << " " << fail[i];
	//		cout << endl;
	//	}
	//	cout << "All key is good\n";
	//	string check = "List fail add :";
	//	if (fail.size() != 0)   throw std::out_of_range("FAIL ADD DATA _ HAVE FAIL KEY");
	//	vector<int> AVL;
	//	vector<int> SPLAY;
	//	myTree.checkNode(AVL, SPLAY);
	//	if (AVL.size() != 0) { for (int i = 0; i < AVL.size(); i++)  cout << AVL[i] << " -- " << SPLAY[i] << endl;  throw std::out_of_range("FAIL ADD 1 _ CORR MISS NODE"); }
	//	else    cout << "GOOD CORR\n"; // GOOD CORR
	//}
	//myTree.clear();
	//cout << "\n=======================================================\n";
	//cout << "================  CLEAR TREE COMPLETE  ================";
	//cout << "\n=======================================================\n";
	//BKUTree<int, int> myTree;
	{
		int arr[30] = { 79, 128, 64, 114, 137, 34, 47, 131, 88, 123, 89, 76, 114, 38, 12, 142, 123, 44, 114, 12, 110, 75, 27, 104, 13, 120, 113, 44, 64, 18 };
		cout << "\n=====================TEST ADD 1=====================\n";
		vector<int> fail;
		for (int i = 0; i < 30; i++) {
			try { myTree.add(arr[i], arr[i] * 2); }
			catch (const char* msg) {
				string data = "Duplicate key";
				bool yes = 1;
				int size = data.size();
				for (int i = 0; i < size; i++)  if (msg[i] != data[i]) { yes = 0;  break; }
				if (yes == 1)    fail.push_back(arr[i]);
			}
		}
		stringstream list;
		if (fail.size() != 0) {
			cout << "List fail add :";
			list << "List fail add :";
			for (int i = 0; i < fail.size(); i++) { cout << " " << fail[i];  list << " ";  list << fail[i]; }
			cout << endl;
		}
		cout << "List key error: 114 123 114 12 44 64\n";
		string finalCheck = list.str();
		string check = "List fail add : 114 123 114 12 44 64";
		for (int i = 0; i < check.size(); i++)    if (finalCheck[i] != check[i])    throw std::out_of_range("FAIL ADD DATA _ SAME KEY INT TREE");
		vector<int> AVL;
		vector<int> SPLAY;
		myTree.checkNode(AVL, SPLAY);
		if (AVL.size() != 0) { for (int i = 0; i < AVL.size(); i++)  cout << AVL[i] << " -- " << SPLAY[i] << endl;  throw std::out_of_range("FAIL ADD 1 _ CORR MISS NODE"); }
		else    cout << "GOOD CORR\n"; // GOOD CORR
	}
	// TEST DELETE 1
	{
		cout << "\n===================TEST DELETE 1===================\n";
		vector<int> fail;
		int arr[15] = { 3, 44, 38, 132, 79, 114, 150, 34, 113, 89, 149, 110, 27, 137, 137 };
		for (int i = 0; i < 15; i++) {
			try { myTree.remove(arr[i]); }
			catch (const char* msg) {
				string data = "Not found";
				bool yes = 1;
				int size = data.size();
				for (int i = 0; i < size; i++)  if (msg[i] != data[i]) { yes = 0;  break; }
				if (yes == 1)    fail.push_back(arr[i]);
			}
		}
		stringstream list;
		if (fail.size() != 0) {
			cout << "List not found :";
			list << "List not found :";
			for (int i = 0; i < fail.size(); i++) { cout << " " << fail[i];  list << " ";  list << fail[i]; }
			cout << endl;
		}
		cout << "List not found : 3 132 150 149 137\n";
		string finalCheck = list.str();
		string check = "List not found : 3 132 150 149 137";
		for (int i = 0; i < check.size(); i++)    if (finalCheck[i] != check[i])    throw std::out_of_range("FAIL DELETE DATA _ SAME KEY INT TREE");
		vector<int> AVL;
		vector<int> SPLAY;
		myTree.checkNode(AVL, SPLAY);
		if (AVL.size() != 0) { for (int i = 0; i < AVL.size(); i++)  cout << AVL[i] << " -- " << SPLAY[i] << endl;  throw std::out_of_range("FAIL DELETE 1 _ CORR MISS NODE"); }
		else    cout << "GOOD CORR\n"; // GOOD CORR    
	}
	// TEST ADD 2
	{
		int arr[10] = { 6, 42, 34, 99, 145, 19, 116, 7, 15, 24 };
		cout << "\n=====================TEST ADD 2=====================\n";
		vector<int> fail;
		for (int i = 0; i < 10; i++) {
			try { myTree.add(arr[i], arr[i] * 2); }
			catch (const char* msg) {
				string data = "Duplicate key";
				bool yes = 1;
				int size = data.size();
				for (int i = 0; i < size; i++)  if (msg[i] != data[i]) { yes = 0;  break; }
				if (yes == 1)    fail.push_back(arr[i]);
			}
		}
		if (fail.size() != 0) {
			cout << "List fail add :";
			for (int i = 0; i < fail.size(); i++)  cout << " " << fail[i];
			cout << endl;
		}
		cout << "All key is good\n";
		string check = "List fail add :";
		if (fail.size() != 0)   throw std::out_of_range("FAIL ADD DATA _ HAVE FAIL KEY");
		vector<int> AVL;
		vector<int> SPLAY;
		myTree.checkNode(AVL, SPLAY);
		if (AVL.size() != 0) { for (int i = 0; i < AVL.size(); i++)  cout << AVL[i] << " -- " << SPLAY[i] << endl;  throw std::out_of_range("FAIL ADD 1 _ CORR MISS NODE"); }
		else    cout << "GOOD CORR\n"; // GOOD CORR
	}
	// CHECK INDEX DATA (TU CHECK TRUE FALSE)
	{
		cout << "\n==================CHECK INDEX DATA==================\n";
		cout << endl << "AVL Tree : ";
		myTree.traverseNLROnAVL(&printData);
		cout << "\nAVL Tree : 76 18 12 6 7 13 15 42 24 19 34 64 47 75 128 104 88 99 120 116 123 142 131 145" << endl;

		cout << "\nSplay Tree : ";
		myTree.traverseNLROnSplay(&printData);
		cout << "\nSplay Tree : 24 15 7 6 13 12 18 19 99 34 42 88 75 47 64 76 116 104 142 123 120 131 128 145" << endl;
	}
	// SEARCH DATA
	{
		cout << "\n====================SEARCH DATA====================\n";
		int arr[10] = { 18, 150, 49, 78, 67, 110, 104, 40, 75, 82 };
		vector<int> fail;
		vector<int> value;
		string checkValue[10];
		{
			checkValue[0] = "Find key : 18, value is : 36\nList key travel : 24 19 76";
			checkValue[1] = "Find key : 150, NOT FOUND\nList key travel : 18 42 64 75 76 128 142 145";
			checkValue[2] = "Find key : 49, NOT FOUND\nList key travel : 18 42 64 47 76 18";
			checkValue[3] = "Find key : 78, NOT FOUND\nList key travel : 18 42 64 75 76 128 104 88";
			checkValue[4] = "Find key : 67, NOT FOUND\nList key travel : 18 42 64 75 76 18";
			checkValue[5] = "Find key : 110, NOT FOUND\nList key travel : 18 42 64 75 76 128 104 120 116";
			checkValue[6] = "Find key : 104, value is : 208\nList key travel : 18 42 64 75 76 128";
			checkValue[7] = "Find key : 40, NOT FOUND\nList key travel : 18 42 24 34 76 18";
			checkValue[8] = "Find key : 75, value is : 150\nList key travel : 18 42 64";
			checkValue[9] = "Find key : 82, NOT FOUND\nList key travel : 18 42 64 75 76 128 104 88";
		}
		for (int i = 0; i < 10; i++) {
			value.push_back(-1);
			vector<int> data;
			cout << "\nFind key : " << arr[i];
			try { value[i] = myTree.search(arr[i], data); }
			catch (const char* msg) {
				string data = "Not found";
				bool yes = 1;
				int size = data.size();
				for (int i = 0; i < size; i++)  if (msg[i] != data[i]) { yes = 0;  break; }
				if (yes == 1)    fail.push_back(arr[i]);
			}
			if (value[i] != -1)    cout << ", value is : " << value[i] << endl;
			else    cout << ", NOT FOUND\n";
			cout << "List key travel :";
			for (auto it = data.begin(); it != data.end(); it++)  cout << " " << *it;
			cout << "\n\tDATA CHECK:\n";
			cout << checkValue[i];
			cout << endl;
		}
		cout << endl;
		stringstream list;
		if (fail.size() != 0) {
			cout << "List NOT FOUND :";
			list << "List NOT FOUND :";
			for (int i = 0; i < fail.size(); i++) { cout << " " << fail[i];  list << " ";  list << fail[i]; }
			cout << endl;
		}
		cout << "List NOT FOUND : 150 49 78 67 110 40 82\n";
		string finalCheck = list.str();
		string check = "List NOT FOUND : 150 49 78 67 110 40 82";
		for (int i = 0; i < check.size(); i++)    if (finalCheck[i] != check[i])    throw std::out_of_range("FAIL SEARCH DATA _ SEARCH KEY NOT IN TREE");

		vector<int> AVL;
		vector<int> SPLAY;
		myTree.checkNode(AVL, SPLAY);
		if (AVL.size() != 0) { for (int i = 0; i < AVL.size(); i++)  cout << AVL[i] << " -- " << SPLAY[i] << endl;  throw std::out_of_range("FAIL ADD 1 _ CORR MISS NODE"); }
		else    cout << "GOOD CORR\n"; // GOOD CORR
	}

	return 0;
}