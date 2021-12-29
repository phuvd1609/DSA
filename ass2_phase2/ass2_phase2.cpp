#include<iostream>
#include<vector>
#include<algorithm>
#include<iterator>
using namespace std;
class Interval
{
public:
	int start;
	int end;

	Interval(int start = 0, int end = 0)
	{
		this->start = start;
		this->end = end;
	}
};

//int minPark(vector<Interval> lInterval)
//{
//	int count = 1;
//	int maximumcount = count;
//	int len = lInterval.size();
//	if (len == 0)
//		return len;
//	int start = lInterval[0].start;
//	int end = lInterval[0].end;
//	for (int i = 1; i < len; i++)
//	{
//		for (int j = i; j < len; j++)
//		{
//			if ((lInterval[j].start >= start && lInterval[j].start < end) || lInterval[j].start < end && lInterval[j].end >= end)
//			{
//				count++;
//			}
//		}
//		if (maximumcount < count)
//		{
//			maximumcount = count;
//			count = 1;
//		}
//		start = lInterval[i].start;
//		end = lInterval[i].end;
//	}
//	return maximumcount;
//}

//int minPark(vector<Interval> lInterval)
//{
//	int count = 0;
//	int maximumcount = count;
//	int len = lInterval.size();
//	int min = 1;
//	int max = len;
//	int mintime = lInterval[0].start;
//	int maxtime = lInterval[0].end;
//	for (int i = 1; i < len; i++)
//	{
//		if (mintime > lInterval[i].start)
//			mintime = lInterval[i].start;
//		if (maxtime < lInterval[i].end)
//			maxtime = lInterval[i].end;
//	}
//	while (min <= max)
//	{
//		int mid = min + (max - min) / 2;
//		int midtime = mintime + (maxtime - mintime) / 2;
//		for (int i = 0; i < len; i++)
//		{
//			if (midtime >= lInterval[i].start && midtime < lInterval[i].end)
//				count++;
//		}
//		if (count == mid)
//			return count;
//		else if (count < mid)
//		{
//			if (maximumcount < count)
//				maximumcount = count;
//			count = 0;
//			max = mid - 1;
//			maxtime = midtime - 1;
//		}
//		else
//		{
//			if (maximumcount < count)
//				maximumcount = count;
//			count = 0;
//			min = mid + 1;
//			mintime = midtime + 1;
//		}
//	}
//	return maximumcount;
//}

bool compare(Interval s1, Interval s2)
{
	return (s1.start < s2.start);
}

int minPark(vector<Interval> lInterval)
{
	int len = int(lInterval.size());
	if (len == 0)
		return len;
	int* start = new int[len];
	int* end = new int[len];
	for (int i = 0; i < len; i++)
	{
		start[i] = lInterval[i].start;
		end[i] = lInterval[i].end;
	}
	sort(start, start + len);
	sort(end, end + len);
	int count = 1, result = count;
	int i = 1, j = 0;
	while (i < len && j < len)
	{
		if (start[i] < end[j])
		{
			count++;
			i++;
		}
		else if (start[i] >= end[j])
		{
			count--;
			j++;
		}
		if (count > result)
			result = count;
	}
	return result;
}

//class Interval {
//	int low, high;
//	Interval(int low=0, int high=0)
//	{
//		this->low = low;
//		this->high = high;
//	}
//};

class Node {
public:
	Interval* data;
	int max;
	Node* left, * right;
	int height;

public:
	Node(Interval* value)
	{
		this->data->start = value->start;
		this->data->end = value->end;
		this->max = value->end;
		this->left = NULL;
		this->right = NULL;
		this->height = 0;
	}
	Node(int s, int t)
	{
		this->data = new Interval(s, t);
		this->max = t;
		this->left = NULL;
		this->right = NULL;
		this->height = 0;
	}
	~Node() {}
};

class BusParking {
public:
	/*vector<int> start;
	vector<int> end;
	int minStart, maxStart, minEnd, maxEnd;
	int min, len;*/


	Node* root;
	int countOverlap;
	BusParking()
	{
		this->root = NULL;
		this->countOverlap = 0;
	}
	/*void add(int s, int t)
	{
		len++;
		if (int(this->start.size()) == 0)
		{
			this->start.push_back(s);
			this->end.push_back(t);
			minStart = maxStart = 0;
			minEnd = maxEnd = 0;
			min = 1;
		}
		else
		{
			int check = 0;
			int mid = 0;
			while (minStart <= maxStart)
			{
				mid = minStart + (maxStart - minStart) / 2;
				if (start[mid] == s)
				{
					check = 1;
					start.insert(start.begin() + mid, s);
				}
				else if (start[mid] < s)
					minStart = mid + 1;
				else
					maxStart = mid - 1;
			}
			if (check == 0)
				start.insert(start.begin() + mid, s);
			mid = 0;
			while (minEnd <= maxEnd)
			{
				mid = minEnd + (maxEnd - minEnd) / 2;
				if (end[mid] == s)
				{
					check = 1;
					end.insert(end.begin() + mid, s);
				}
				else if (end[mid] < s)
					minEnd = mid + 1;
				else
					maxEnd = mid - 1;
			}
			if (check == 0)
				end.insert(end.begin() + mid, s);
		}
	}*/

	Node* rightRotate(Node* y)
	{
		Node* x = y->left;


		Node* T2 = x->right;

		Node* T3 = new Node(0, 0);
		if (y != NULL && y->right != NULL)
		{
			T3 = y->right;
			y->max = (T2->max > T3->max) ? T2->max : T3->max;
		}
		else
		{
			if (T2 != NULL)
				y->max = (y->max > T2->max) ? y->max : T2->max;
		}
		Node* T1 = new Node(0, 0);
		if (x != NULL && x->left != NULL)
		{
			T1 = x->left;
			x->max = (T1->max > y->max) ? T1->max : y->max;
		}
		else
		{
			x->max = (x->max > y->max) ? x->max : y->max;
		}
		x->right = y;
		y->left = T2;

		return x;
	}
	Node* leftRotate(Node* x)
	{
		Node* y = x->right;
		Node* T2 = y->left;
		Node* T1 = new Node(0, 0);
		if (x != NULL && x->left != NULL)
		{
			T1 = x->left;
			x->max = (T1->max > T2->max) ? T1->max : T2->max;
		}
		else
		{
			if (T2 != NULL)
				x->max = (x->max > T2->max) ? x->max : T2->max;
		}

		Node* T3 = new Node(0, 0);
		if (y != NULL && y->right != NULL)
		{
			T3 = y->right;
			y->max = (x->max > T3->max) ? x->max : T3->max;
		}
		else
		{
			y->max = (x->max > y->max) ? x->max : y->max;
		}
		y->left = x;
		x->right = T2;

		return y;
	}

	int height(Node* N)
	{
		if (N == NULL)
			return 0;
		return N->height;
	}

	int max(int a, int b)
	{
		return (a > b) ? a : b;
	}

	int getBalance(Node* N)
	{
		if (N == NULL)
			return 0;
		return height(N->left) - height(N->right);
	}

	Node* addNode(Node* node, int s, int t)
	{
		if (node == NULL)
		{
			Node* p = new Node(s, t);
			return p;
		}
		int l = node->data->start;
		int r = node->data->end;
		// If root's low value is smaller, then new interval goes to 
		// left subtree 
		if (s < l || (s == l && t < r))
			node->left = addNode(node->left, s, t);

		// Else, new node goes to right subtree. 
		else if (s > l || (s == l && t > r))
			node->right = addNode(node->right, s, t);

		// Update the max value of this ancestor if needed 
		if (node->max < t)
			node->max = t;

		//node->height = 1 + max(height(node->left), height(node->right));
		//int balance = getBalance(node);

		//if (balance > 1 && (s < l || (s == l && t < r)))
		//	return rightRotate(node);

		//// Right Right Case  
		//if (balance < -1 && (s > l || (s == l && t > r)))
		//	return leftRotate(node);

		//// Left Right Case  
		//if (balance > 1 && (s > l || (s == l && t > r)))
		//{
		//	node->left = leftRotate(node->left);
		//	return rightRotate(node);
		//}

		//// Right Left Case  
		//if (balance < -1 && (s < l || (s == l && t < r)))
		//{
		//	node->right = rightRotate(node->right);
		//	return leftRotate(node);
		//}

		return node;
	}

	void add(int s, int t)
	{
		Interval i = Interval(s, t);
		this->root = addNode(this->root, s, t);
		this->countOverlap = 0;
		overlapSearch(this->root, i);
	}

	bool doOVerlap(Interval i1, Interval i2)
	{
		if (i1.start <= i2.end && i2.start <= i1.end)
			return true;
		return false;
	}

	void overlapSearch(Node* root, Interval i)
	{
		// Base Case, tree is empty 
		if (root == NULL) return;

		// If given interval overlaps with root 
		if (doOVerlap(*(root->data), i))
		{
			this->countOverlap++;
			//return root->data;
		}

		// If left child of root is present and max of left child is 
		// greater than or equal to given interval, then i may 
		// overlap with an interval is left subtree 
		if (root->left != NULL && root->left->max >= i.end)
			overlapSearch(root->left, i);

		// Else interval can only overlap with right subtree 
		else
			overlapSearch(root->right, i);
	}

	Node* maxValueNode(Node* node)
	{
		Node* current = node;

		/* loop down to find the leftmost leaf */
		while (current->right != NULL)
			current = current->right;

		return current;
	}

	Node* deleteNode(Node* node, int s, int t)
	{
		if (node == NULL)
			return node;
		int l = node->data->start;
		int r = node->data->end;

		if (s < l || (s == l && t < r))
			node->left = deleteNode(node->left, s, t);

		else if (s > l || (s == l && t > r))
			node->right = deleteNode(node->right, s, t);

		else
		{

			if ((node->left == NULL) || (node->right == NULL))
			{
				Node* temp = node->left ? node->left : node->right;


				if (temp == NULL)
				{
					temp = node;
					node = NULL;
				}
				else
					*node = *temp;
				free(temp);
			}
			else
			{
				Node* temp = maxValueNode(node->left);
				node->data = temp->data;
				node->max = temp->max;
				node->left = deleteNode(node->left, temp->data->start, temp->data->end);
			}
		}

		return root;
	}

	void remove(int s, int t)
	{
		this->root = deleteNode(this->root, s, t);
	}

	int minPark()
	{
		return this->countOverlap;
	}
};

int main()
{
	/*vector<Interval> intervals;
	intervals.push_back(Interval(0, 3));
	intervals.push_back(Interval(3, 5));
	intervals.push_back(Interval(1, 5));
	intervals.push_back(Interval(2, 5));*/
	//cout << intervals.size();
	//cout << minPark(intervals);
	BusParking* b = new BusParking();
	b->add(1, 5);
	b->add(2, 5);
	b->add(3, 5);
	b->add(5, 6);
	cout << b->minPark();
	////output: 3
	return 0;
}