#include "main.h"

class table {
	int id;
	int res;
	int soLanGoiMon;
	bool khuVuc;
	string name;

public:
	table() : id(-1), res(-1), soLanGoiMon(0), khuVuc(0) { this->name = ""; }

	table(int id, int res, int soLanGoiMon, bool khuVuc, string name = "") {
		this->id = id;
		this->res = res;
		this->soLanGoiMon = soLanGoiMon;
		this->name = name;
		this->khuVuc = khuVuc;
	}

	void setKhuVuc(bool khuVuc) {
		this->khuVuc = khuVuc;
	}

	void setID(int id) {
		this->id = id;
	}

	void setRes(int res) {
		this->res = res;
	}

	void setGoiMon(int goi) {
		this->soLanGoiMon = goi;
	}

	void setName(string name) {
		this->name = name;
	}

	bool getKhuVuc() {
		return this->khuVuc;
	}

	int getID() {
		return this->id;
	}

	int getRes() {
		return this->res;
	}

	int getGoiMon() {
		return this->soLanGoiMon;
	}

	void tangMon() {
		++this->soLanGoiMon;
	}

	string getName() {
		return this->name;
	}

	bool coKhach() {
		return this->name.size() != 0;
	}

	bool operator== (table b) {
		return this->getID() == b.getID() && this->getName() == b.getName() && this->getRes() == b.getRes();
	}

	~table() {}

};

struct HeapNode {
	table ta;
	int thuTu;
	int orderTime;

	HeapNode() : thuTu(-1), orderTime(-1) { ta = table(); }

	HeapNode(int thuT) : thuTu(thuT), orderTime(thuT) { ta = table(); }

	HeapNode(table t, int thuT) : ta(t), thuTu(thuT), orderTime(thuT) {}

	HeapNode(table t, int thuT, int order) : ta(t), thuTu(thuT), orderTime(order) {}

	bool operator == (HeapNode a) {
		return this->ta == a.ta;
	}

	bool operator < (HeapNode a) {
		if (this->ta.getGoiMon() == a.ta.getGoiMon())
			return this->thuTu < a.thuTu;

		return this->ta.getGoiMon() < a.ta.getGoiMon();
	}

	bool operator >= (HeapNode a) {
		return this->ta.getGoiMon() >= a.ta.getGoiMon();
	}

	~HeapNode() {  }
};

struct compa {
	bool operator ()(HeapNode a, HeapNode b) {
		if (a.ta.getGoiMon() == b.ta.getGoiMon())
			return a.thuTu < b.thuTu;

		return a.ta.getGoiMon() < b.ta.getGoiMon();
	}
};

class Heap {
protected:
	HeapNode* elements;
	int capacity;
	int count;

public:
	Heap()
	{
		this->capacity = MAXSIZE;
		this->count = 0;
		this->elements = new HeapNode[this->capacity];
	}
	~Heap()
	{
		delete[] this->elements;
	}
	void push(HeapNode item) {
		++this->count;
		this->ensureCapacity(this->count);

		*(this->elements + this->count - 1) = item;

		this->reheapUp(this->count - 1);

	};

	void linear() {
		for (int i = 0; i < this->count; ++i)
			cout << (this->elements + i)->ta.getRes() << "-" << (this->elements + i)->ta.getID() << "-" << (this->elements + i)->ta.getGoiMon() << endl;
	}

	void print() {
		if (this->count == 0) return;

		stack<HeapNode*> st;
		st.push(this->elements);

		while (!st.empty()) {
			int left = 2 * (st.top() - this->elements) + 1;
			int right = 2 * (st.top() - this->elements) + 2;

			cout << st.top()->ta.getID() << "-" << st.top()->ta.getGoiMon() << endl;
			st.pop();

			if (right < this->count) st.push(this->elements + right);
			if (left < this->count) st.push(this->elements + left);
		}

	}


	bool isEmpty() {
		return this->count == 0;
	};

	bool contains(HeapNode item) {
		for (int i = 0; i < this->count; ++i) {
			if (*(this->elements + i) == item) return 1;
		}

		return 0;
	};

	HeapNode peek() {
		if (this->count == 0) return HeapNode(-1);
		return this->elements[0];
	};

	bool pop() {
		if (this->count < 1) return 0;

		--this->count;

		*this->elements = *(this->elements + this->count);

		this->reheapDown(0);

		return 1;

	};

	int size() {
		return this->count;
	};

	int getItem(HeapNode item) {

		for (int i = 0; i < this->count; ++i)
			if (item == *(this->elements + i)) return i;

		return -1;
	}

	bool remove(HeapNode item) {

		if (this->count < 1) return 0;

		int rmId = this->getItem(item);

		if (rmId == -1) return 0;

		--this->count;

		*(this->elements + rmId) = *(this->elements + this->count);

		reheapUp(rmId);
		reheapDown(rmId);

		return 1;
	}

	void clear() {
		while (this->count != 0) {
			this->remove(*this->elements);
		}

		this->capacity = 10;
		this->count = 0;

		delete[] this->elements;

		this->elements = new HeapNode[capacity];
	}

	void update(HeapNode temp) {
		int oldThu = this->getItem(temp);

		if (oldThu == -1) return;

		oldThu = (this->elements + oldThu)->thuTu;

		remove(temp);

		temp.thuTu = oldThu;

		push(temp);

	}

private:
	void ensureCapacity(int minCapacity) {
		if (minCapacity > this->capacity) {
			int newCapacity = (this->capacity * 3) / 2 + 1;
			if (newCapacity < minCapacity)
				newCapacity = minCapacity;

			this->capacity = newCapacity;

			HeapNode* newArray = new HeapNode[this->capacity];

			for (int i = 0; i < count; ++i) {
				*(newArray + i) = *(elements + i);
			}

			delete[] this->elements;

			this->elements = newArray;
		}
	};
	void reheapUp(int position) {
		if (this->count < 2) return;

		if (position > 0) {
			int parent = (position - 1) / 2;
			if (*(this->elements + position) < *(this->elements + parent)) {
				swap(*(this->elements + position), *(this->elements + parent));
				reheapUp(parent);
			}
		}
	}
	void reheapDown(int index) {
		if (this->count < 2) return;

		int leftChild = index * 2 + 1;
		int rightChild = index * 2 + 2;

		int lessChild = -1;

		if (leftChild < this->count) {
			if ((rightChild <= this->count)) {
				if (elements[rightChild] < elements[leftChild])
					lessChild = rightChild;
				else lessChild = leftChild;
			}

			if (elements[lessChild] < elements[index]) {
				swap(elements[lessChild], elements[index]);
				reheapDown(lessChild);
			}
		}
	};
};

struct MinHeapNode {
	char data;
	int freq;
	unsigned int thuTu;

	// left and right child
	MinHeapNode* left, * right;

	MinHeapNode(char data, unsigned freq, unsigned int thuTu) {
		this->left = nullptr;
		this->right = nullptr;
		this->data = data;
		this->freq = freq;
		this->thuTu = thuTu;
	}

	MinHeapNode(char data, unsigned freq, MinHeapNode* l, MinHeapNode* r, unsigned int thuTu) {
		this->left = l;
		this->right = r;
		this->data = data;
		this->freq = freq;
		this->thuTu = thuTu;
	}

	~MinHeapNode() {}
};

struct compare {

	bool operator()(MinHeapNode* l, MinHeapNode* r)

	{
		if (l->freq == r->freq) {

			if (l->data == r->data) return l->thuTu > r->thuTu;
			return l->data > r->data;
		}
		return (l->freq > r->freq);
	}
};

void printCodes(MinHeapNode* root, string str, map<char, string>& m)
{

	if (!root)
		return;

	if (root->data != '~') {
		m[root->data] = str;
	}


	printCodes(root->left, str + "0", m);
	printCodes(root->right, str + "1", m);
}

string HuffmanCodes(vector<pair<char, int>> v, int size, string s) {
	if (v.size() == 1) {
		string s;

		for (int i = 0; i < v[0].second; ++i) {
			s += "1";
		}

		return s;
	}

	unsigned int c = 0;
	MinHeapNode* left, * right, * top;

	priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

	for (int i = 0; i < size; ++i) {
		minHeap.push(new MinHeapNode(v[i].first, v[i].second, 0));
	}

	while (minHeap.size() != 1) {
		
		left = minHeap.top();
		minHeap.pop();

		right = minHeap.top();
		minHeap.pop();

		top = new MinHeapNode('~', left->freq + right->freq, left, right, ++c);

		minHeap.push(top);
	}

	map<char, string> m;
	printCodes(minHeap.top(), "", m);

	string re = "";

	int i = 0;

	auto it = s.end();

	--it;

	while (++i < 16) {
		re = m[*it] + re;
		if ((re.size() >= 15) || (it == s.begin())) break;
		--it;
	}

	return re;
}

bool cmp(pair<char, int> a, pair<char, int> b) {
	if (a.second == b.second) return a.first < b.first;
	return a.second < b.second;
}

class khuVuc1 {
	table* arr;
	int size;

public:
	khuVuc1() : size(0) { this->arr = new table[1]; };

	khuVuc1(int size) {
		this->size = 0;
		arr = new table[size];

		for (int i = 0; i < size; ++i) {
			(arr + i)->setID(-1);
			(arr + i)->setName("");
			(arr + i)->setRes(-1);
			(arr + i)->setKhuVuc(1);
		}

	}

	void update(table ta) {
		int hash = find(ta);

		if (hash == -1) return;
		
		(arr + hash)->tangMon();

		/*for (int i = 0; i < MAXSIZE / 2; ++i) {
			if (*(arr + i) == ta) {
				(arr + i)->tangMon();
				return;
			}
		}*/
	}

	int find(table t) {
		int res = t.getRes();
		int i = 0;
		int hash = res % (MAXSIZE / 2);

		while ( i < MAXSIZE / 2) {
			if (arr[(hash + i) % (MAXSIZE / 2)] == t) return (hash + i) % (MAXSIZE / 2);
			++i;
		}

		/*for (int i = 0; i < MAXSIZE / 2; ++i) {
			if (*(arr + i) == t) return i;
		}*/

		return -1;
	}

	bool remove(table t, deque<table>& q, deque<HeapNode>& st, Heap& h, int hash) {
		if (this->size < 1) return 0;

		(arr + hash)->setName("");
		(arr + hash)->setID(-1);
		(arr + hash)->setRes(-1);
		(arr + hash)->setGoiMon(0);
		(arr + hash)->setKhuVuc(1);

		for (auto i = q.begin(); i != q.end(); ++i)
			if (*i == t) {
				q.erase(i);
				break;
			}

		for (auto i = st.begin(); i != st.end(); ++i)
			if (i->ta.getName() == t.getName()) {
				st.erase(i);
				break;
			}

		h.remove(HeapNode(t, 1));

		--this->size;

		return 1;
	}

	bool add(table& t) {
		if (this->size == MAXSIZE / 2) return 0;

		int res = t.getRes();
		int i = 0;
		int hash = res % (MAXSIZE / 2);
		t.setKhuVuc(1);

		while (arr[(hash + i) % (MAXSIZE / 2)].getID() != -1 && i < MAXSIZE / 2) {
			++i;
		}

		(arr + ((hash + i) % (MAXSIZE / 2)))->setID(t.getID());
		(arr + ((hash + i) % (MAXSIZE / 2)))->setName(t.getName());
		(arr + ((hash + i) % (MAXSIZE / 2)))->setRes(res);
		(arr + ((hash + i) % (MAXSIZE / 2)))->setGoiMon(1);
		(arr + ((hash + i) % (MAXSIZE / 2)))->setKhuVuc(1);
		++this->size;

		return 1;
	}

	void clear(table findRes[], deque<table>& q, deque<HeapNode>& st, Heap& h) {
		for (int i = 0; i < MAXSIZE / 2; ++i) {
			if ((arr + i)->getID() != -1) {
				(arr + i)->setName("");
				(arr + i)->setID(-1);
				(arr + i)->setRes(-1);
				(arr + i)->setGoiMon(0);
				(arr + i)->setKhuVuc(1);
			}
		}

		deque<table> q1;

		for (auto i = q.begin(); i != q.end(); ++i) {
			if (i->getKhuVuc()) {
				findRes[i->getID()] = table();
				HeapNode temp(*i, 1);
				auto f = std::find(st.begin(), st.end(), temp);
				st.erase(f);
				h.remove(temp);
			}
			else q1.push_back(*i);
		}

		q = q1;
		this->size = 0;
	}

	void print() {
		for (int i = 0; i < MAXSIZE / 2; ++i) {
			if (arr[i].getID() != -1)
				cout << arr[i].getID() << "-" << arr[i].getRes() << "-" << arr[i].getGoiMon() << endl;
		}
	}

	~khuVuc1() {
		delete[] this->arr;
	}
};

enum BalanceValue
{
	LH = -1,
	EH = 0,
	RH = 1
};

class AVLTree
{
public:
	class Node;
private:
	Node* root;
protected:
	int getHeightRec(Node* node)
	{
		if (!node)
			return 0;

		int lh = this->getHeightRec(node->pLeft);
		int rh = this->getHeightRec(node->pRight);
		return (lh > rh ? lh : rh) + 1;
	}
public:
	AVLTree() : root(nullptr) {}
	~AVLTree() {}
	int getHeight()
	{
		return this->getHeightRec(this->root);
	}

	Node* rotL(Node* root) {
		if (!root)  return nullptr;

		Node* r = root->pRight;
		Node* lOr = r->pLeft;

		r->pLeft = root;
		root->pRight = lOr;

		root->balance = (BalanceValue)(getHeightRec(root->pRight) - getHeightRec(root->pLeft));
		if (root->pLeft) root->pLeft->balance = (BalanceValue)(getHeightRec(root->pLeft->pRight) - getHeightRec(root->pLeft->pLeft));
		if (root->pRight) root->pRight->balance = (BalanceValue)(getHeightRec(root->pRight->pRight) - getHeightRec(root->pRight->pLeft));

		return r;
	}

	Node* rotR(Node* root) {
		if (!root)  return nullptr;

		Node* l = root->pLeft;
		Node* rOl = l->pRight;

		l->pRight = root;
		root->pLeft = rOl;

		root->balance = (BalanceValue)(getHeightRec(root->pRight) - getHeightRec(root->pLeft));
		if (root->pLeft) root->pLeft->balance = (BalanceValue)(getHeightRec(root->pLeft->pRight) - getHeightRec(root->pLeft->pLeft));
		if (root->pRight) root->pRight->balance = (BalanceValue)(getHeightRec(root->pRight->pRight) - getHeightRec(root->pRight->pLeft));

		return l;
	}

	int getBalance(Node* root) {
		if (!root) return 0;

		return getHeightRec(root->pLeft) - getHeightRec(root->pRight);
	}

	Node* insert(Node* root, table& data) {
		if (!root) {
			return new Node(data);
		}
		else if (data.getRes() < root->data.getRes()) {

			root->pLeft = insert(root->pLeft, data);

		}
		else {

			root->pRight = insert(root->pRight, data);

		}

		int balance = getBalance(root);

		// Left Left
		if (balance > 1 && data.getRes() < root->pLeft->data.getRes())
			root = rotR(root);

		// Right Right Case
		else if (balance < -1 && data.getRes() >= root->pRight->data.getRes())
			root = rotL(root);

		// Left Right Case
		else if (balance > 1 && data.getRes() >= root->pLeft->data.getRes())
		{
			root->pLeft = rotL(root->pLeft);
			root = rotR(root);
		}

		// Right Left Case
		else if (balance < -1 && data.getRes() < root->pRight->data.getRes()) {
			root->pRight = rotR(root->pRight);
			root = rotL(root);
		}

		root->balance = (BalanceValue)(getHeightRec(root->pRight) - getHeightRec(root->pLeft));

		/* return the (unchanged) node pointer */
		return root;
	}

	void insert(table& value) {
		this->root = insert(this->root, value);
	}

	Node* deleteRightBalance(Node*& root, bool& shorter)
	{
		if (root->balance == LH)
		{
			root->balance = EH;
		}
		else if (root->balance == EH)
		{
			root->balance = RH;
			shorter = false;
		}
		else
		{
			Node* rightTree = root->pRight;
			if (rightTree->balance == LH)
			{
				Node* leftTree = rightTree->pLeft;
				if (leftTree->balance == LH)
				{
					rightTree->balance = RH;
					root->balance = EH;
				}
				else
				{
					root->balance = LH;
					rightTree->balance = EH;
				}
				leftTree->balance = EH;
				root->pRight = rotR(rightTree);
				root = rotL(root);
			}
			else
			{
				if (rightTree->balance != EH)
				{
					root->balance = EH;
					rightTree->balance = EH;
				}
				else
				{
					root->balance = RH;
					rightTree->balance = LH;
					shorter = false;
				}
				root = rotL(root);
			}
		}
		return root;
	}
	Node* deleteLeftBalance(Node*& root, bool& shorter)
	{
		if (root->balance == RH)
		{
			root->balance = EH;
		}
		else if (root->balance == EH)
		{
			root->balance = LH;
			shorter = false;
		}
		else
		{
			Node* leftTree = root->pLeft;
			if (leftTree->balance == RH)
			{
				Node* rightTree = leftTree->pRight;
				if (rightTree->balance == RH)
				{
					leftTree->balance = LH;
					root->balance = EH;
				}
				else
				{
					root->balance = RH;
					leftTree->balance = EH;
				}
				rightTree->balance = EH;
				root->pLeft = rotL(leftTree);
				root = rotR(root);
			}
			else
			{
				if (leftTree->balance != EH)
				{
					root->balance = EH;
					leftTree->balance = EH;
				}
				else
				{
					root->balance = LH;
					leftTree->balance = RH;
					shorter = false;
				}
				root = rotR(root);
			}
		}
		return root;
	}

	Node* deleteNodeRec(Node*& root, table ta, bool& shorter, bool& success)
	{
		int res = ta.getRes();

		if (!root)
		{
			shorter = false;
			success = false;
			return nullptr;
		}
		if (res < root->data.getRes())
		{
			root->pLeft = deleteNodeRec(root->pLeft, ta, shorter, success);
			if (shorter)
				root = deleteRightBalance(root, shorter);
		}
		else if (res > root->data.getRes())
		{
			root->pRight = deleteNodeRec(root->pRight, ta, shorter, success);
			if (shorter)
				root = deleteLeftBalance(root, shorter);
		}
		else
		{
			if (root->data == ta) {
				Node* deleteNode = root;
				if (!root->pRight)
				{
					Node* newRoot = root->pLeft;
					success = true;
					shorter = true;
					delete(deleteNode);
					return newRoot;
				}
				else if (!root->pLeft)
				{
					Node* newRoot = root->pRight;
					success = true;
					shorter = true;
					delete(deleteNode);
					return newRoot;
				}
				else
				{
					Node* exchPtr = root->pRight;
					while (exchPtr->pLeft)
					{
						exchPtr = exchPtr->pLeft;
					}
					root->data = exchPtr->data;
					root->pRight = deleteNodeRec(root->pRight, exchPtr->data, shorter, success);
					if (shorter)
						root = deleteLeftBalance(root, shorter);
				}
			}
			else {
				root->pLeft = deleteNodeRec(root->pLeft, ta, shorter, success);
				if (shorter) {
					root = deleteRightBalance(root, shorter);
				}

				if (!success) {
					root->pRight = deleteNodeRec(root->pRight, ta, shorter, success);
					if (shorter)
						root = deleteLeftBalance(root, shorter);
				}

			}

		}
		return root;
	}
	void remove(Node*& root, table& ta) {
		bool shorter = false;
		bool success = false;
		root = deleteNodeRec(root, ta, shorter, success);
	}

	bool find(Node* root, int& res, table ta) {
		if (!root) return 0;

		if (res < root->data.getRes())
			return find(root->pLeft, res, ta);
		else if (res > root->data.getRes())
			return find(root->pRight, res, ta);

		if (root->data == ta) return 1;
		else {
			bool check = find(root->pLeft, res, ta);

			if (check) return 1;

			check = find(root->pRight, res, ta);

			return check;
		}

		return 0;
	}

	bool reGoi(Node*& root, int res, table ta) {
		if (!root) return 0;

		if (res < root->data.getRes())
			return reGoi(root->pLeft, res, ta);
		else if (res > root->data.getRes())
			return reGoi(root->pRight, res, ta);

		if (root->data == ta) {
			root->data.tangMon();
			return 1;
		}
		else {
			bool check = reGoi(root->pRight, res, ta);

			if (check) return 1;

			check = reGoi(root->pLeft, res, ta);

			return check;
		}

		return 0;
	}

	void print(Node* root) {
		if (!root) return;

		queue<Node*> q;
		q.push(root);

		while (!q.empty()) {
			Node* top = q.front();
			q.pop();

			if (top->pLeft) q.push(top->pLeft);
			if (top->pRight) q.push(top->pRight);

			cout << top->data.getID() << "-" << top->data.getRes() << "-" << top->data.getGoiMon() << endl;
		}
	}

	class Node
	{
	private:
		table data;
		Node* pLeft, * pRight;
		BalanceValue balance;
		friend class AVLTree;
		friend class khuVuc2;

	public:
		Node(table value) : data(value), pLeft(NULL), pRight(NULL), balance(EH) {}

		~Node() {}
	};
};

class khuVuc2 : public AVLTree {
	int size;
	Node* root;
public:
	khuVuc2() : size(0) { this->root = nullptr; }
	~khuVuc2() {
		if (!this->root) return;
		queue<Node*> q;
		q.push(this->root);

		while (!q.empty()) {
			Node* temp = q.front();
			q.pop();

			if (temp->pLeft) q.push(temp->pLeft);
			if (temp->pRight) q.push(temp->pRight);

			temp = nullptr;
			free(temp);
		}
	}

	void update(table ta) {
		int res = ta.getRes();

		reGoi(this->root, res, ta);
	}

	bool add(table& data) {
		if (this->size == MAXSIZE / 2) return 0;
		data.setKhuVuc(0);
		this->root = insert(this->root, data);
		++this->size;
		return 1;
	}

	bool del(table ta, deque<table>& q, deque<HeapNode>& st, Heap& h) {
		if (!this->root)  return 0;

		remove(this->root, ta);

		for (auto i = q.begin(); i != q.end(); ++i)
			if (*i == ta) {
				q.erase(i);
				break;
			}

		for (auto i = st.begin(); i != st.end(); ++i)
			if (i->ta == ta) {
				st.erase(i);
				break;
			}

		h.remove(HeapNode(ta, 1));

		--this->size;

		return 1;
	}

	void clear(table findRes[], deque<table>& q, deque<HeapNode>& st, Heap& h) {
		while (this->root) {
			
			table temp = this->root->data;

			remove(this->root, temp);
		}

		deque<table> q1;

		for (auto i = q.begin(); i != q.end(); ++i) {
			if (!(i->getKhuVuc())) {
				findRes[i->getID()] = table();
				HeapNode temp(*i, 1);
				auto f = std::find(st.begin(), st.end(), temp);
				st.erase(f);
				h.remove(temp);
			}
			else q1.push_back(*i);
		}

		q = q1;
		this->size = 0;
	}

	bool search(table ta) {
		if (!this->root)  return 0;

		int res = ta.getRes();

		return find(this->root, res, ta);
	}

	void print2() {
		if (!this->root)  return;

		print(this->root);
	}
};

bool cle(int id, table findRes[], khuVuc1& a, khuVuc2& b, deque<table>& q, deque<HeapNode>& st, Heap& h) {
	if (id < 1) {
		a.clear(findRes, q, st, h);
		return 1;
	}
	else if (id > MAXSIZE) {
		b.clear(findRes, q, st, h);
		return 1;
	}

	if (!findRes[id].coKhach()) return 0;

	table ta = *(findRes + id);

	if (ta.getName().size() == 0) return 0;

	bool re = 0;

	int khu1 = a.find(ta);
	if (khu1 != -1) re = a.remove(ta, q, st, h, khu1);
	else if (b.search(ta)) re = b.del(ta, q, st, h);

	if (re) {
		findRes[id] = table();
	}

	return re;
}

void reg(string name, table findRes[], khuVuc1& a, khuVuc2& b, deque<table>& q, deque<HeapNode>& st, Heap& h, int& thuTu) {
	map<char, int> m;

	vector<pair<char, int>> v;
	for (int i = 0; name[i] != '\0'; ++i) {
		++m[name[i]];
	}
	for (auto x : m) {
		v.push_back(make_pair(x.first, x.second));
	}

	sort(v.begin(), v.end(), cmp);

	string t = HuffmanCodes(v, v.size(), name);

	if (t.size() > 15)
		t = t.substr(t.size() - 15);

	int coe2 = 1;
	int res = 0;

	for (int i = t.size() - 1; i >= 0; --i, coe2 *= 2) {
		if (t[i] == '1') res += coe2;
	}

	int id = res % MAXSIZE + 1;
	int c = MAXSIZE;

	table ta(id, res, 1, res % 2, name);
	bool goi = 0;

	while (c >= 0) {
		if (findRes[id].coKhach()) {
			if (findRes[id].getName() == name) {
				ta.setGoiMon(findRes[id].getGoiMon() + 1);
				goi = 1;
				break;
			}
		}

		if (c == 0) break;

		if (id == MAXSIZE)
			id = 0;

		--c;
		++id;
	}

	if (!goi) {
		c = 0;

		while (c < MAXSIZE) {
			if (findRes[id].coKhach()) {
				if (id == MAXSIZE)
					id = 1;
				else ++id;
			}
			else break;

			++c;
		}
	}

	ta.setID(id);

	bool conCho = 0;

	if (!goi) {
		if (res % 2) {
			conCho = a.add(ta);
			if (!conCho) conCho = b.add(ta);
		}
		else {
			conCho = b.add(ta);
			if (!conCho) conCho = a.add(ta);
		}
	}
	else {
		conCho = 1;
		a.update(ta);
		b.update(ta);
	}

	if (conCho) {

		if (q.empty() || find(q.begin(), q.end(), ta) == q.end()) q.push_back(ta);

		HeapNode temp(ta, thuTu);

		if (st.empty()) st.push_back(temp);
		else {

			auto f = find(st.begin(), st.end(), temp);

			if (f != st.end()) {
				temp.thuTu = f->thuTu;
				st.erase(f);
				st.push_back(temp);
			}
			else st.push_back(temp);

		}

		if (goi) {
			h.update(temp);
		}
		else h.push(temp);
	}
	else {
		bool checkTrong = 0;

		if (res % 3 == 0 && !q.empty()) {

			ta.setID(q.front().getID());

			checkTrong = cle(q.front().getID(), findRes, a, b, q, st, h);
		}
		else if (res % 3 == 1 && !st.empty()) {

			int maxId = 0;
			int treNhat = 0;

			auto i = st.begin();

			for (; i != st.end(); ++i) {
				if (treNhat < thuTu - i->orderTime) {
					maxId = i->ta.getID();
					treNhat = thuTu - i->orderTime;
				}
			}

			ta.setID(maxId);

			checkTrong = cle(maxId, findRes, a, b, q, st, h);
		}
		else {
			if (h.size() != 0) {
				ta.setID(h.peek().ta.getID());

				checkTrong = cle(h.peek().ta.getID(), findRes, a, b, q, st, h);
			}
		}

		if (res % 2) {
			conCho = a.add(ta);
			if (!conCho) conCho = b.add(ta);
		}
		else {
			conCho = b.add(ta);
			if (!conCho) conCho = a.add(ta);

		}

		HeapNode temp(ta, thuTu);

		if (checkTrong) {
			q.push_back(ta);
			st.push_back(temp);
			h.push(temp);
		}
	}

	findRes[ta.getID()] = ta;
	++thuTu;
}

void simulate(string filename)
{
	int thuTu = 0;
	khuVuc1 a(MAXSIZE / 2);
	khuVuc2 b;

	deque<table> q;
	deque<HeapNode> st;
	Heap h;

	table findRes[MAXSIZE + 1];

	ifstream file(filename, ios::in);
	string str = "";

	while (getline(file, str)) {

		if (str == "") break;
		else if (str.find(' ') != string::npos) {
			size_t f = str.find(' ');
			size_t l = str.rfind(' ');
			if (f != l) continue;

			string checkReg = str.substr(0, f);

			if (checkReg == "REG") {
				string name = str.substr(f + 1);

				if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) continue;

				reg(name, findRes, a, b, q, st, h, thuTu);
			}
			else if (checkReg == "CLE") {
				string sID = str.substr(f + 1);

				int id = 0;
				try {
					id = stoi(sID);
				}
				catch (...) {
					continue;
				}

				cle(id, findRes, a, b, q, st, h);
			}
		}
		else {
			if (str == "PrintHT") {
				a.print();
			}
			else if (str == "PrintAVL") {
				b.print2();
			}
			else if (str == "PrintMH") {
				h.print();
			}
			
		}
	}
	return;
}