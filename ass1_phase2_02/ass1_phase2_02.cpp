#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <climits>
#include <time.h>
using namespace std;


class BusSystem
{
public:
	class Node;
	Node* head = NULL;
	Node* tail = NULL;

protected:
	int max;
	int count;

public:
	class Node
	{
	public:
		int time_A, time_B, direction;
		string code, lp;
		Node* next;
		Node* prev;
		friend class FragmentLinkedList;

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
		Node(string code, string lp, int direction, int time_A, int time_B)
		{
			this->code = code;
			this->lp = lp;
			this->direction = direction;
			this->time_A = time_A;
			this->time_B = time_B;
		}
	};

public:
	string converttostring(int n)
	{
		stringstream ss;
		ss << n;
		string str = ss.str();
		return str;
	}

	int size()
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

	string SQ(int n)
	{
		this->max = n;
		if (n < 0)
			return "-1";
		return "1";
	}

	string INS(string code, string lp, int time_A, int time_B, int direction = 0)
	{
		Node* node = new Node(code, lp, direction, time_A, time_B);
		if (this->head != NULL)
		{
			int n = 0;
			Node* ptemp = this->head;
			Node* p = this->head;

			while (ptemp != NULL && ptemp->code != code)
			{
				ptemp = ptemp->next;
				p = p->next;
			}

			if (p != NULL && p->code == code)
			{
				while (p != NULL && p->code == code)
				{
					if (p->time_A == time_A)
					{
						if (p->direction == direction)
						{
							return "-1";
						}
						else
						{
							if (p->lp == lp)
							{
								return "-1";
							}
						}
					}
					if (time_A <= p->time_B && p->lp == lp)
					{
						return "-1";
					}
					n++;
					p = p->next;
				}
			}

			if (ptemp != NULL && ptemp->code == code)
			{
				/*if (ptemp != NULL && ptemp->time_A >= time_A && ptemp->direction == direction)
				{
					return "-1";
				}*/

				while (ptemp != NULL && ptemp->code == code && ptemp->time_A < time_A)
				{
					if (ptemp->lp == lp)
					{
						if (ptemp->time_B < time_A)
						{

						}
						else
						{
							return "-1";
						}
					}

					if (ptemp->time_A == time_A)
					{
						return "-1";
					}
					ptemp = ptemp->next;
				}

				if (ptemp != NULL && (ptemp->time_A >= time_A || ptemp->code != code))
				{
					ptemp = ptemp->prev;

					n++;
					if (n > this->max)
					{
						return "-1";
					}
					if (ptemp == NULL)
					{
						node->next = this->head;
						this->head->prev = node;
						this->head = node;
						this->head->prev = NULL;
					}
					else if (ptemp->next != NULL)
					{
						node->next = ptemp->next;
						ptemp->next->prev = node;
						ptemp->next = node;
						node->prev = ptemp;
					}
					else
					{
						node->next = this->tail->next;
						node->prev = this->tail;
						this->tail->next = node;
						this->tail = node;
					}
				}
				else if (ptemp == NULL)
				{
					n++;
					if (n > this->max)
					{
						return "-1";
					}
					node->next = this->tail->next;
					node->prev = this->tail;
					this->tail->next = node;
					this->tail = node;
				}
			}
			else
			{
				n++;
				if (n > this->max)
				{
					return "-1";
				}
				node->next = this->tail->next;
				this->tail->next = node;
				node->prev = this->tail;
				this->tail = node;
			}

			return converttostring(n);

		}
		else
		{
			this->head = node;
			this->tail = node;
			int n = 1;
			if (n > this->max)
			{
				return "-1";
			}
			return "1";
		}
	}

	void removeNode(Node** ptemp)
	{
		Node* p = *ptemp;
		*ptemp = (*ptemp)->next;

		if (p == NULL)
			return;
		if (p == this->head)
		{
			if (this->head != this->tail)
			{
				this->head = this->head->next;
				this->head->prev = NULL;
			}
			else
			{
				this->head = NULL;
				this->tail = NULL;
				p = NULL;
			}
		}
		else
		{
			p->prev->next = p->next;
			if (p->next != NULL)
			{
				p->next->prev = p->prev;
			}
			else
			{
				this->tail = p->prev;
				this->tail->next = NULL;
			}
		}
		free(p);
		return;
	}


	string DEL(string code, int time_A = -5, int time_B = -5)
	{
		Node* ptemp = this->head;
		while (ptemp != NULL && ptemp->code != code)
		{
			ptemp = ptemp->next;
		}
		if (ptemp != NULL && ptemp->code == code)
		{
			int n = 0;
			if (time_A != -5 && time_B != -5)
			{
				while (ptemp != NULL && ptemp->time_A < time_A && ptemp->code == code)
				{
					ptemp = ptemp->next;
				}
				while (ptemp != NULL && ptemp->time_A >= time_A && ptemp->time_A <= time_B && ptemp->code == code)
				{
					n++;
					removeNode(&ptemp);
				}
				return converttostring(n);
			}
			else if (time_A != -5 && time_B == -5)
			{
				if (ptemp->time_A != time_A)
				{
					while (ptemp != NULL && ptemp->time_A != time_A && ptemp->code == code)
					{
						ptemp = ptemp->next;
						if (ptemp != NULL && ptemp->time_A == time_A)
						{
							n++;
							removeNode(&ptemp);
						}
					}
				}
				while (ptemp != NULL && ptemp->time_A == time_A && ptemp->code == code)
				{
					if (ptemp != NULL && ptemp->time_A == time_A)
					{
						n++;
						removeNode(&ptemp);
					}
				}
				return converttostring(n);
			}
			else if (time_A == -5 && time_B == -5)
			{
				while (ptemp != NULL && ptemp->code == code)
				{
					n++;
					removeNode(&ptemp);
				}
				return converttostring(n);
			}
			else
			{
				return "0";
			}
		}
		else
		{
			return "0";
		}
	}

	string CS(string code, int time, int direction = 2)
	{
		Node* ptemp = this->head;
		while (ptemp != NULL && ptemp->code != code)
		{
			ptemp = ptemp->next;
		}

		if (ptemp != NULL && ptemp->code == code)
		{
			int n = 0;
			while (ptemp != NULL && ptemp->code == code)
			{
				if (direction == 2)
				{
					if (ptemp->time_A <= time && ptemp->time_B >= time)
					{
						n++;
					}
				}
				else if (direction == 1)
				{
					if (ptemp->time_A <= time && ptemp->time_B >= time && ptemp->direction == 1)
					{
						n++;
					}
				}
				else if (direction == 0)
				{
					if (ptemp->time_A <= time && ptemp->time_B >= time && ptemp->direction == 0)
					{
						n++;
					}
				}
				ptemp = ptemp->next;
			}
			return converttostring(n);
		}
		else
		{
			return "0";
		}
	}

	string CE(string code, int time, int direction = 2)
	{
		Node* ptemp = this->head;
		while (ptemp != NULL && ptemp->code != code)
		{
			ptemp = ptemp->next;
		}
		if (ptemp->code == code)
		{
			int n = 0;
			while (ptemp != NULL && ptemp->code == code)
			{
				if (direction == 2)
				{
					if (ptemp->time_B < time)
					{
						n++;
					}
				}
				else if (direction == 1)
				{
					if (ptemp->time_B < time && ptemp->direction == 1)
					{
						n++;
					}
				}
				else if (direction == 0)
				{
					if (ptemp->time_B < time && ptemp->direction == 0)
					{
						n++;
					}
				}
				ptemp = ptemp->next;
			}
			return converttostring(n);
		}
		else
		{
			return "0";
		}
	}

	string GS(string code, int time, int direction = 2)
	{
		Node* ptemp = this->head;
		while (ptemp != NULL && ptemp->code != code)
		{
			ptemp = ptemp->next;
		}
		if (ptemp->code == code)
		{
			Node* p = ptemp;
			if (direction == 2)
			{
				int diff = INT_MAX;

				while (ptemp != NULL && ptemp->time_A > time && ptemp->code == code)
				{
					ptemp = ptemp->next;
				}

				if (ptemp == NULL || ptemp->code != code)
				{
					return "-1";
				}

				if (ptemp != NULL && ptemp->code == code && ptemp->time_A <= time)
				{
					diff = time - ptemp->time_A;
					ptemp = ptemp->next;
				}

				while (ptemp != NULL && ptemp->code == code && ptemp->time_A <= time)
				{
					if (time - ptemp->time_A < diff)
					{
						diff = time - ptemp->time_A;
					}
					ptemp = ptemp->next;
				}

				if (diff == INT_MAX)
				{
					return "-1";
				}

				while (p != ptemp && time - p->time_A != diff)
				{
					p = p->next;
				}

				if (p->direction == 1)
				{
					if (p->next != NULL && p->next->direction == 0 && time - p->next->time_A == time - p->time_A)
					{
						p = p->next;
					}
					else if (p->prev != NULL && p->prev->direction == 0 && time - p->prev->time_A == time - p->time_A)
					{
						p = p->prev;
					}
				}
			}
			else if (direction == 1)
			{
				while (ptemp != NULL && ptemp->direction != 1 && ptemp->code == code)
				{
					ptemp = ptemp->next;
				}
				if (ptemp == NULL || ptemp->code != code)
				{
					return "-1";
				}
				int diff = INT_MAX;
				while (ptemp != NULL && ptemp->time_A <= time && ptemp->code == code)
				{
					if (ptemp->direction == 1 && time - ptemp->time_A < diff)
					{
						diff = time - ptemp->time_A;
					}
					ptemp = ptemp->next;
				}

				if (diff == INT_MAX)
				{
					return "-1";
				}

				while (p != ptemp)
				{
					if (time - p->time_A == diff && p->direction == direction)
					{
						break;
					}
					else
					{
						p = p->next;
					}
				}
			}
			else if (direction == 0)
			{
				while (ptemp != NULL && ptemp->direction != 0 && ptemp->code == code)
				{
					ptemp = ptemp->next;
				}
				if (ptemp == NULL || ptemp->code != code)
				{
					return "-1";
				}
				int diff = INT_MAX;
				while (ptemp != NULL && ptemp->time_A <= time && ptemp->code == code)
				{
					if (ptemp->direction == 0 && time - ptemp->time_A < diff)
					{
						diff = time - ptemp->time_A;
					}
					ptemp = ptemp->next;
				}

				if (diff == INT_MAX)
				{
					return "-1";
				}

				while (p != ptemp)
				{
					if (time - p->time_A == diff && p->direction == direction)
					{
						break;
					}
					else
					{
						p = p->next;
					}
				}
			}
			return p->lp;
		}
		else
		{
			return "-1";
		}
	}

	string GE(string code, int time, int direction = 2)
	{
		Node* ptemp = this->head;
		while (ptemp != NULL && ptemp->code != code)
		{
			ptemp = ptemp->next;
		}
		if (ptemp->code == code)
		{
			Node* p = ptemp;
			if (direction == 2)
			{
				int diff = INT_MAX;

				/*while (ptemp != NULL && ptemp->time_B > time && ptemp->code == code)
				{
					ptemp = ptemp->next;
				}*/

				if (ptemp == NULL || ptemp->code != code)
				{
					return "-1";
				}

				if (ptemp != NULL && ptemp->code == code && ptemp->time_B < time)
				{
					diff = time - ptemp->time_B;
					ptemp = ptemp->next;
				}

				while (ptemp != NULL && ptemp->code == code)
				{
					if (time - ptemp->time_B < diff && ptemp->time_B < time)
					{
						diff = time - ptemp->time_B;
					}
					ptemp = ptemp->next;
				}

				if (diff == INT_MAX)
				{
					return "-1";
				}

				Node* p1 = this->tail;
				if (ptemp != NULL)
				{
					p1 = ptemp->prev;
				}

				while (time - p1->time_B != diff && p1 != p)
				{
					p1 = p1->prev;
				}

				if (p1->direction == 1)
				{
					if (p1->next != NULL && p1->next->direction == 0 && time - p1->next->time_B == time - p1->time_B && p1->next->code == code && p1->next->time_A >= p1->time_A)
					{
						p1 = p1->next;
					}
					else if (p1->prev != NULL && p1->prev->direction == 0 && time - p1->prev->time_B == time - p1->time_B && p1->prev->code == code && p1->prev->time_A >= p1->time_A)
					{
						p1 = p1->prev;
					}
				}
				return p1->lp;
			}
			else if (direction == 1)
			{
				while (ptemp != NULL && ptemp->direction != 1 && ptemp->code == code)
				{
					ptemp = ptemp->next;
				}
				if (ptemp == NULL || ptemp->code != code)
				{
					return "-1";
				}
				int diff = INT_MAX;
				while (ptemp != NULL && ptemp->code == code)
				{
					if (ptemp->direction == 1 && time - ptemp->time_B < diff && ptemp->time_B < time)
					{
						diff = time - ptemp->time_B;
					}
					ptemp = ptemp->next;
				}

				if (diff == INT_MAX)
				{
					return "-1";
				}

				Node* p1 = this->tail;
				if (ptemp != NULL)
				{
					p1 = ptemp->prev;
				}

				while (p1 != p)
				{
					if (time - p1->time_B == diff && p1->direction == direction)
					{
						break;
					}
					else
					{
						p1 = p1->prev;
					}
				}
				return p1->lp;

			}
			else if (direction == 0)
			{
				while (ptemp != NULL && ptemp->direction != 0 && ptemp->code == code)
				{
					ptemp = ptemp->next;
				}
				if (ptemp == NULL || ptemp->code != code)
				{
					return "-1";
				}
				int diff = INT_MAX;
				while (ptemp != NULL && ptemp->code == code)
				{
					if (ptemp->direction == 0 && time - ptemp->time_B < diff && ptemp->time_B < time)
					{
						diff = time - ptemp->time_B;
					}
					ptemp = ptemp->next;
				}

				if (diff == INT_MAX)
				{
					return "-1";
				}

				Node* p1 = this->tail;
				if (ptemp != NULL)
				{
					p1 = ptemp->prev;
				}

				while (p1 != p)
				{
					if (time - p1->time_B == diff && p1->direction == direction)
					{
						break;
					}
					else
					{
						p1 = p1->prev;
					}
				}
				return p1->lp;
			}
			return "-1";
		}
		else
		{
			return "-1";
		}
	}

	int convertnumber(string time)
	{
		int size = time.length();
		if (size == 0)
		{
			return -5;
		}
		for (int i = 0; i < size; i++)
		{
			if (time[i] < 48 || time[i]>57)
				return -1;
		}
		stringstream ss(time);
		int x = 0;
		ss >> x;
		return x;
	}

	string query(string instruction)
	{
		if (instruction.length() == 0 || instruction[0] == ' ' || instruction[instruction.length() - 1] == ' ')
		{
			return "-1";
		}
		else
		{
			string::iterator it = instruction.begin();
			string ins;
			while (*it != ' ' && it != instruction.end())
			{
				ins.append(1, *it);
				it++;
				if (it == instruction.end())
					return "-1";
			}
			if (ins.length() == 2)
			{
				if (ins == "SQ")
				{
					it++;
					if (it == instruction.end() || *it == ' ')
						return "-1";

					string n;
					while (*it != ' ' && *it != '\0')
					{
						n.append(1, *it);
						it++;
						if (it == instruction.end())
							break;
					}
					if (it != instruction.end() && *it == ' ')
						return "-1";
					int N = convertnumber(n);
					return SQ(N);
				}
				else if (ins == "CS" || ins == "CE" || ins == "GS" || ins == "GE")
				{
					if (it == instruction.end())
						return "-1";
					string code;
					it++;
					if (it == instruction.end() || *it == ' ')
						return "-1";
					while (*it != ' ' && *it != '\0')
					{
						code.append(1, *it);
						it++;
						if (it == instruction.end())
							return "-1";
					}

					if (code.length() > 5)
						return "-1";

					string time;
					it++;
					if (it == instruction.end() || *it == ' ')
						return "-1";
					while (*it != ' ' && *it != '\0')
					{
						time.append(1, *it);
						it++;
						if (it == instruction.end())
							break;
					}
					if (time.length() > 10)
						return "-1";


					int direction = 2;
					if (it != instruction.end())
					{
						it++;
						if (*it == ' ')
							return "-1";
						else if (*it == '1')
							direction = 1;
						else if (*it == '0')
							direction = 0;
						else
							return "-1";
					}

					int TIME = convertnumber(time);
					if (ins == "CS")
					{
						return CS(code, TIME, direction);
					}
					else if (ins == "CE")
					{
						return CE(code, TIME, direction);
					}
					else if (ins == "GS")
					{
						return GS(code, TIME, direction);
					}
					else if (ins == "GE")
					{
						return GE(code, TIME, direction);
					}
					return "-1";
				}
				else
				{
					return "-1";
				}
			}
			else if (ins.length() == 3)
			{
				if (it == instruction.end())
					return "-1";
				string code;
				it++;
				if (it == instruction.end() || *it == ' ')
					return "-1";
				while (*it != ' ' && *it != '\0')
				{
					code.append(1, *it);
					it++;
					if (it == instruction.end() && ins == "INS")
						return "-1";
					else if (it == instruction.end() && ins == "DEL")
						break;
				}

				if (code.length() > 5 || code.length() == 0)
					return "-1";

				if (ins == "INS")
				{
					string lp;
					it++;
					if (it == instruction.end() || *it == ' ')
						return "-1";
					while (*it != ' ' && *it != '\0')
					{
						lp.append(1, *it);
						it++;
						if (it == instruction.end())
							return "-1";
					}
					if (lp.length() > 10 || lp.length() == 0)
						return "-1";

					string direc = "";
					int direction = 0;
					if (*it == ' ')
						it++;

					if (*it == ' ')
						return "-1";

					while (*it != ' ' && *it != '\0')
					{
						if (*it == ' ')
						{
							return "-1";
						}
						else
						{
							direc.append(1, *it);
							it++;
						}
					}

					string time_A, time_B;

					it++;
					if (it == instruction.end() || *it == ' ')
						return "-1";

					while (*it != ' ' && *it != '\0')
					{
						time_A.append(1, *it);
						if (*it != '\0')
						{
							it++;
						}
						if (it == instruction.end())
							break;
					}
					if (time_A.length() > 10)
						return "-1";

					if (it != instruction.end())
					{
						it++;
						while (*it != ' ' && *it != '\0')
						{
							time_B.append(1, *it);
							it++;
							if (it == instruction.end())
								break;
						}
					}
					if (time_B.length() == 0)
					{
						time_B = time_A;
						time_A = direc;
						direction = 0;
					}
					else if (direc.length() == 1)
					{
						if (direc == "1")
						{
							direction = 1;
						}
						else if (direc == "0")
						{
							direction = 0;
						}
						else
						{
							return "-1";
						}
					}
					else if (direc.length() != 1)
					{
						return "-1";
					}

					if (time_B.length() > 10 || it != instruction.end() || (direction != 1 && direction != 0))
						return "-1";
					int timeA = convertnumber(time_A), timeB = convertnumber(time_B);
					if ((timeA == -1) || (timeB == -1) || !(timeB > timeA))
						return "-1";


					return INS(code, lp, timeA, timeB, direction);
				}
				else if (ins == "DEL")
				{

					string time_A, time_B;
					if (it == instruction.end())
					{

					}
					else
					{
						it++;
						if (*it == ' ')
							return "-1";

						while (*it != ' ' && *it != '\0')
						{
							time_A.append(1, *it);
							it++;
							if (it == instruction.end())
								break;
						}
						if (time_A.length() > 10)
							return "-1";

						if (it == instruction.end())
						{

						}
						else
						{
							it++;
							if (it == instruction.end() || *it == ' ')
								return "-1";
							while (*it != ' ' && *it != '\0')
							{
								time_B.append(1, *it);
								it++;
								if (it == instruction.end())
									break;
							}
							if (time_B.length() > 10 || it != instruction.end())
								return "-1";
						}
					}
					int timeA = convertnumber(time_A), timeB = convertnumber(time_B);
					if ((timeA == -1) || (timeB == -1) || (!(timeB > timeA) && timeA != -5 && timeB != -5))
						return "-1";
					return DEL(code, timeA, timeB);
				}
				else
				{
					return "-1";
				}
			}
			else
			{
				return "-1";
			}
		}
	}
};

// END: STUDENT ANSWER

int main()
{
	BusSystem* bs = new BusSystem();
	cout << bs->query("SQ 500") << endl;

	cout << endl;

	cout << bs->query("INS 50 50D1-23342 0 1234 5678") << endl;
	cout << bs->query("INS 50 50D1-23343 1 1234 5678") << endl;

	cout << bs->query("INS 50 50D1-23341 0 2345 5678") << endl;
	cout << bs->query("INS 50 50D1-23340 1 2346 5678") << endl;

	cout << bs->query("INS 50 50D1-23345 1 3456 6789") << endl;
	cout << bs->query("INS 50 50D1-23344 0 3456 6789") << endl;
	
	cout << endl;

	cout << bs->query("INS 8 51D1-23342 0 1234 5678") << endl;
	cout << bs->query("INS 8 51D1-23343 1 1234 5678") << endl;

	cout << bs->query("INS 8 51D1-23341 0 2345 5678") << endl;
	cout << bs->query("INS 8 51D1-23340 1 2346 5678") << endl;

	cout << endl;

	cout << bs->query("INS 10 52D1-23342 0 1234 5678") << endl;
	cout << bs->query("INS 10 52D1-23343 1 1236 5678") << endl;

	cout << endl;
	cout << bs->query("GS 50 6789") << endl;
	cout << bs->query("GS 10 6789 0") << endl;
	cout << bs->query("GS 8 6789 0") << endl;


	cout << endl;

	/*BusSystem* bs = new BusSystem();
	cout << bs->query("SQ 500") << endl;
	cout << bs->query("INS 50 ABG 0 6 7") << endl;
	cout << bs->query("INS 50 ABK 1 6 7") << endl;*/

	/*BusSystem* bs = new BusSystem();
	cout << bs->query("SQ 100000") << '\n';

	cout << "------------------------------\n";
	clock_t begin = clock();
	for (int i = 1; i <= 1000; i++)
	{
		string ins = "INS 50 ";
		ins += to_string(i) + " " + to_string(i * 2) + " " + to_string(100000 + i * 2);
		bs->query(ins);
	}
	clock_t end = clock();
	cout << "INS execution time: " << (double)(end - begin) / CLOCKS_PER_SEC << '\n';
	cout << "------------------------------\n";

	begin = clock();
	cout << bs->query("CS 50 100") << '\n';
	end = clock();
	cout << "CS execution time: " << (double)(end - begin) / CLOCKS_PER_SEC << '\n';
	cout << "------------------------------\n";

	begin = clock();
	cout << bs->query("CE 50 100160") << '\n';
	end = clock();
	cout << "CE execution time: " << (double)(end - begin) / CLOCKS_PER_SEC << '\n';
	cout << "------------------------------\n";

	begin = clock();
	cout << bs->query("GS 50 100") << '\n';
	end = clock();
	cout << "GS execution time: " << (double)(end - begin) / CLOCKS_PER_SEC << '\n';
	cout << "------------------------------\n";

	begin = clock();
	cout << bs->query("GE 50 100161") << '\n';
	end = clock();
	cout << "GE execution time: " << (double)(end - begin) / CLOCKS_PER_SEC << '\n';
	cout << "------------------------------\n";

	begin = clock();
	cout << bs->query("DEL 50 100 160") << '\n';
	end = clock();
	cout << "DEL execution time: " << (double)(end - begin) / CLOCKS_PER_SEC << '\n';
	cout << "------------------------------\n";*/

	/*BusSystem* bs = new BusSystem();
	cout << bs->query("SQ 500") << endl;
	cout << bs->query("INS 50 50D1-23342 1234 5678") << endl;
	cout << bs->query("INS 50 50D1-23343 0 4000 5000") << endl;
	cout << bs->query("INS 50 50D1-23342 1 1200 1222") << endl;
	cout << bs->query("CS 50 1210") << endl;*/

	return 0;
}