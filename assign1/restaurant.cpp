#include "main.h"

void reg(restaurant* r = 0, restaurant* wait = 0, restaurant* orderIn = 0, restaurant* orderWait = 0, int id = 0, string name = "", int age = 0, bool hasID = 1);
void regm(restaurant* r = 0, restaurant* orderIn = 0, string name = "", int age = 0, int num = 0);
void cle(restaurant* r = 0, restaurant* wait = 0, restaurant* orderIn = 0, restaurant* orderWait = 0, int id = 0);
void ps(restaurant* orderIn = 0, restaurant* orderWait = 0, int num = 0, bool hasID = 1);
void pq(restaurant* orderWait = 0, int num = 0, bool hasID = 1);
void pt(restaurant* r = 0);
void sq(restaurant* wait = 0, int num = 0);

void simulate(string filename, restaurant* r)
{
	ifstream file(filename, ios::in);

	restaurant* wait = new restaurant();

	restaurant* orderIn = new restaurant();

	restaurant* orderWait = new restaurant();

	string str = "";

	while (getline(file, str)) {

		string command = "";
		
		size_t fsp = 0, lsp = 0;
		
		if (str == "") break;
		if (str.find(' ') == string::npos) {

			if (str == "PS") ps(orderIn, orderWait, 0, 0);
			else if (str == "PQ") pq(orderWait, 0, 0);
			else if (str == "PT") pt(r);
			continue;
		}
		else {
			fsp = str.find(' '), lsp = str.find_last_of(' ');
			command = str.substr(0, fsp);

		}
		if (command == "REG") {
			string sAge(str.begin() + lsp + 1, str.end());

			size_t secondsp = str.find(' ', fsp + 1);
			int age = stoi(sAge), id = 0;

			bool hasID = 1;
			if (lsp == secondsp) {
				secondsp = fsp;
				hasID = 0;
			}

			string name(str.begin() + secondsp + 1, str.begin() + lsp);

			if (hasID) {
				string sId(str.begin() + fsp + 1, str.begin() + secondsp);
				id = stoi(sId);
			}

			reg(r, wait, orderIn, orderWait, id, name, age, hasID);
		}
		else if (command == "REGM") {
			string sNum(str.begin() + lsp + 1, str.end());
			size_t secondsp = str.find(' ', fsp + 1);
			string sAge(str.begin() + secondsp + 1, str.begin() + lsp);
			string name(str.begin() + fsp + 1, str.begin() + secondsp);
			int num = stoi(sNum), age = stoi(sAge);
			
			regm(r, orderIn, name, age, num);

		}
		else if (command == "CLE") {
			string sId(str.begin() + 3, str.end());
			int id = stoi(sId);

			cle(r, wait, orderIn, orderWait, id);
		}
		else if (command == "PS") {
			int num = 0;
			if (fsp != 0) {
				string sNum(str.begin() + fsp + 1, str.end());
				num = stoi(sNum);
			}

			ps(orderIn, orderWait, num, 1);
		}
		else if (command == "PQ") {
			int num = 0;
			if (fsp != 0) {
				string sNum(str.begin() + fsp + 1, str.end());
				num = stoi(sNum);
			}

			pq(orderWait, num, 1);
		}
		else if (command == "SQ") {
			string sNUM(str.begin() + fsp + 1, str.end());
			int num = stoi(sNUM);
			sq(wait, num);
		}
	}
	
	table* lastWait = wait->recentTable;

	while (wait->recentTable && wait->recentTable->ID != 1) wait->recentTable = wait->recentTable->next;

	table* delWait = wait->recentTable;

	while (delWait) {
		if (delWait == lastWait) {
			free(lastWait);
			break;
		}

		lastWait->next = delWait->next;
		table* delnext = delWait->next;

		delWait->next = nullptr;
		delWait = nullptr;

		free(delWait);

		delWait = delnext;
		delnext = delnext->next;
	}
	delete wait;

	table* lastOrderWait = orderWait->recentTable;

	while (orderWait->recentTable && orderWait->recentTable->ID != 1) orderWait->recentTable = orderWait->recentTable->next;

	table* delOrderWait = orderWait->recentTable;

	while (delOrderWait) {
		if (delOrderWait == lastOrderWait) {
			free(lastOrderWait);
			break;
		}

		lastOrderWait->next = delOrderWait->next;
		table* delnext = delOrderWait->next;
		delOrderWait->next = nullptr;
		delOrderWait = nullptr;

		free(delOrderWait);

		delOrderWait = delnext;
		delnext = delnext->next;
	}
	delete orderWait;

	table* lastOrderIn = orderIn->recentTable;

	while (orderIn->recentTable && orderIn->recentTable->ID != 1) orderIn->recentTable = orderIn->recentTable->next;

	table* delOrderIn = orderIn->recentTable;

	while (delOrderIn) {
		if (delOrderIn == lastOrderIn) {
			free(lastOrderIn);
			break;
		}

		lastOrderIn->next = delOrderIn->next;
		table* delnext = delOrderIn->next;
		delOrderIn->next = nullptr;
		delOrderIn = nullptr;

		free(delOrderIn);

		delOrderIn = delnext;
		delnext = delnext->next;
	}
	delete orderIn;

	file.close();
}

void reg(restaurant* r, restaurant* wait, restaurant* orderIn, restaurant* orderWait, int id, string name, int age, bool hasID) {
	if (age < 16 || (hasID && (id < 1 || id > MAXSIZE)))  return;

	int c = 1;
	table* head = r->recentTable;

	while (head->next->ID > head->ID) head = head->next;

	head = head->next;

	bool checkID = 0;

	while (c++ <= MAXSIZE && id == 0) {
		if (head->name == "") {
			head->name = name;
			head->age = age;

			if (!orderIn->recentTable) {
				orderIn->recentTable = orderIn->insert(orderIn->recentTable, 1, name, age);
				orderIn->recentTable->name = name;
			}
			else {
				if (orderIn->recentTable->ID == MAXSIZE) return;
				orderIn->recentTable = orderIn->insert(orderIn->recentTable, orderIn->recentTable->ID + 1, name, age);
			}
			r->recentTable = head;
			return;
		}
		head = head->next;
	}

	while (c++ <= MAXSIZE) {
		if (head->ID == id) {
			checkID = 1;
			break;
		}
		head = head->next;
	}


	c = 1;

	while (c++ <= MAXSIZE && id != 0) {

		if (checkID) {
			if (head->name == "") {
				head->name = name;
				head->age = age;

				if (!orderIn->recentTable) {
					orderIn->recentTable = orderIn->insert(orderIn->recentTable, 1, name, age);
					orderIn->recentTable->name = name;
				}
				else {
					if (orderIn->recentTable->ID == MAXSIZE) return;
					orderIn->recentTable = orderIn->insert(orderIn->recentTable, orderIn->recentTable->ID + 1, name, age);
				}

				r->recentTable = head;
				return;
			}
			else break;
		}
		else {
			if (head->name == "") {
				head->name = name;
				head->age = age;

				if (!orderIn->recentTable) {
					orderIn->recentTable = orderIn->insert(orderIn->recentTable, 1, name, age);
					orderIn->recentTable->name = name;
				}
				else {
					if (orderIn->recentTable->ID == MAXSIZE) return;
					orderIn->recentTable = orderIn->insert(orderIn->recentTable, orderIn->recentTable->ID + 1, name, age);
				}

				r->recentTable = head;
				return;
			}
		}
		head = head->next;
	}

	head = head->next;

	while (c++ <= MAXSIZE && id != 0) {
		if (head->name == "") {
			head->name = name;
			head->age = age;

			if (!orderIn->recentTable) {
				orderIn->recentTable = orderIn->insert(orderIn->recentTable, 1, name, age);
				orderIn->recentTable->name = name;
			}
			else {
				if (orderIn->recentTable->ID == MAXSIZE) return;
				orderIn->recentTable = orderIn->insert(orderIn->recentTable, orderIn->recentTable->ID + 1, name, age);
			}

			r->recentTable = head;
			return;
		}
		head = head->next;
	}
	
	if (!wait->recentTable) {
		wait->recentTable = wait->insert(wait->recentTable, 1, name, age);
		wait->recentTable->name = name;
	}
	else {
		if (wait->recentTable && wait->recentTable->ID == MAXSIZE) return;
		wait->recentTable = wait->insert(wait->recentTable, wait->recentTable->ID + 1, name, age);
	}

	if (!orderWait->recentTable) {
		orderWait->recentTable = orderWait->insert(orderWait->recentTable, 1, name, age);
		orderWait->recentTable->name = name;
	}
	else {
		if (orderWait->recentTable->ID == MAXSIZE) return;
		orderWait->recentTable = orderWait->insert(orderWait->recentTable, orderWait->recentTable->ID + 1, name, age);
	}

}

void regm(restaurant* r, restaurant* orderIn, string name, int age, int num) {
	if (age < 16 || num > MAXSIZE || num < 1) return;

	table* tra = r->recentTable, * pre = nullptr, * tail = nullptr;

	while (tra->next->ID > tra->ID) tra = tra->next;

	tra = tra->next;

	if (num == 1) {
		if (tra->name == "") {
			tra->name = name;
			tra->age = age;

			r->recentTable = tra;
			if (!orderIn->recentTable) {
				orderIn->recentTable = orderIn->insert(orderIn->recentTable, 1, name, age);
				orderIn->recentTable->name = name;
			}
			else {
				if (orderIn->recentTable->ID == MAXSIZE) return;
				orderIn->recentTable = orderIn->insert(orderIn->recentTable, orderIn->recentTable->ID + 1, name, age);
			}
			return;
		}
		else return;
	}

	int c = num, ct = 1;

	while (ct++ <= MAXSIZE) { // test lai
		if (tra->name == "") {
			pre = tra;
			break;
		}
		tra = tra->next;
	}

	if (!pre) return;

	ct = 1;

	table* no = nullptr;

	while (!tail && ct++ < MAXSIZE) {
		if (tra->name == "") {
			c--;
			if (c == 0) {
				tail = tra;
				if (no) pre = no->next;
				break;
			}
		}
		else {
			c = num;
			no = tra;
		}
		tra = tra->next;
	}

	if (!tail) return;

	tra = tra->next;

	no = nullptr;
	
	c = num;
	ct = 1;

	while (ct++ < MAXSIZE) {
		if (pre->next->ID < pre->ID) break;
		if (tra->name == "") {
			if (no) {
				c--;
				if (c == 0) {
					tail = tra;
					pre = no->next;
					no = nullptr;
				}
			}
			else {
				pre = pre->next;
				tail = tail->next;
			}
		}
		else {
			c = num;
			no = tra;
		}
		tra = tra->next;
	}
	table* del = pre->next, * temp = pre->next;

	pre->age = age;
	pre->name = name;

	while (temp != tail) {
		temp = temp->next;
		delete del;
		del = temp;
	}

	pre->next = tail->next;

	delete tail;

	r->recentTable = pre;

	if (!orderIn->recentTable) {
		orderIn->recentTable = orderIn->insert(orderIn->recentTable, 1, name, age);
		orderIn->recentTable->name = name;
	}
	else {
		if (orderIn->recentTable->ID == MAXSIZE) return;
		orderIn->recentTable = orderIn->insert(orderIn->recentTable, orderIn->recentTable->ID + 1, name, age);
	}
}

void cle(restaurant* r, restaurant* wait, restaurant* orderIn, restaurant* orderWait, int id) {
	if (id > MAXSIZE || id < 1) return;

	int c = 1;

	table* tra = r->recentTable;
	table* thuTu = orderIn->recentTable;
	
	while (tra->ID != id && c++ <= MAXSIZE) {
		tra = tra->next;
		
		if (c == MAXSIZE + 1 && tra->ID != id) return;
	}

	int checkID = -1;

	if (tra->name != "") {

		if (thuTu == thuTu->next) {
			orderIn->recentTable->next = nullptr;
			orderIn->recentTable = nullptr;

			thuTu->next = nullptr;
			thuTu = nullptr;

			free(wait->recentTable);
		}
		else {
			
			while (thuTu->next->name != tra->name || thuTu->next->age != tra->age) {
				thuTu = thuTu->next;
			}

			orderIn->recentTable = thuTu;

			checkID = thuTu->ID;

			table* xoa = thuTu->next;
			thuTu->next = xoa->next;
			delete xoa;
		}

		r->recentTable = tra;

		if (thuTu) {
			if (thuTu->next->ID < checkID) {
				if (thuTu->next->ID != 1) {
					
					int i = 1;
					while (i <= checkID - 1) {
						thuTu = thuTu->next;
						thuTu->ID = i++;
					}
				}
			}
			else {
				thuTu = thuTu->next;

				while (thuTu->ID != 1) {
					thuTu->ID = ++checkID;
					thuTu = thuTu->next;
				}
			}
		}
	}
	else return;

	while (orderIn->recentTable && orderIn->recentTable->next->ID != 1) orderIn->recentTable = orderIn->recentTable->next;

	int ganNhatID = -1;

	table* voGanNhat = wait->recentTable, * voSomNhat = nullptr;
	if (voGanNhat) {
		voSomNhat = voGanNhat->next;
		ganNhatID = voGanNhat->ID;
	}

	table* oldNext = tra->next;

	while (voSomNhat && voSomNhat->ID != 1) voSomNhat = voSomNhat->next;

	if (voSomNhat) {
		tra->name = voSomNhat->name;
		tra->age = voSomNhat->age;

		if (!orderIn->recentTable) {
			orderIn->recentTable = orderIn->insert(orderIn->recentTable, 1, voSomNhat->name, voSomNhat->age);
			orderIn->recentTable->name = voSomNhat->name;
		}
		else {
			if (orderWait->recentTable->ID == MAXSIZE) return;
			orderIn->recentTable = orderIn->insert(orderIn->recentTable, orderIn->recentTable->ID + 1, voSomNhat->name, voSomNhat->age);
		}

		if (voGanNhat == voSomNhat) {
			wait->recentTable->next = nullptr;
			wait->recentTable = nullptr;

			voGanNhat->next = nullptr;
			voGanNhat = nullptr;

			voSomNhat->next = nullptr;
			voSomNhat = nullptr;

			orderWait->recentTable->next = 0;
			orderWait->recentTable = 0;

			free(orderWait->recentTable);
			
			free(wait->recentTable);
		}
		else {
			table* xoa = voGanNhat->next;
			voGanNhat->next = xoa->next;
			delete xoa;
		}

		if (voGanNhat) voSomNhat = voGanNhat->next;
		else voSomNhat = nullptr;

		c = 1;

		while (tra->ID + 1 != oldNext->ID && c++ <= MAXSIZE) {
			if (!voSomNhat) {
				
				if (tra->ID == MAXSIZE) {
					if (tra->next->ID != 1)	tra = r->insert(tra, 1, "", 0);
					else break;
				}
				else tra = r->insert(tra, tra->ID + 1, "", 0);
			}
			else {

				if (tra->ID == MAXSIZE) {
					if (tra->next->ID != 1)	tra = r->insert(tra, 1, voSomNhat->name, voSomNhat->age);
					else break;
				}
				else tra = r->insert(tra, tra->ID + 1, voSomNhat->name, voSomNhat->age);

				--ganNhatID;

				if (!orderIn->recentTable) {
					orderIn->recentTable = orderIn->insert(orderIn->recentTable, 1, voSomNhat->name, voSomNhat->age);
					orderIn->recentTable->name = voSomNhat->name;
				}
				else {
					if (orderWait->recentTable->ID == MAXSIZE) return;
					orderIn->recentTable = orderIn->insert(orderIn->recentTable, orderIn->recentTable->ID + 1, voSomNhat->name, voSomNhat->age);
				}

				if (voGanNhat == voSomNhat) {
					wait->recentTable->next = nullptr;
					wait->recentTable = nullptr;

					voGanNhat->next = nullptr;
					voGanNhat = nullptr;

					voSomNhat->next = nullptr;
					voSomNhat = nullptr;

					free(wait->recentTable);
				}
				else {
					table* xoa = voGanNhat->next;
					voGanNhat->next = xoa->next;
					delete xoa;
				}

			}

			if (voGanNhat) voSomNhat = voGanNhat->next;
			else voSomNhat = nullptr;
		}

		tra->next = oldNext;

	}
	else {
		tra->name = "";
		tra->age = 0;

		r->recentTable = tra;
		
		while (tra->ID + 1 != oldNext->ID && c++ <= MAXSIZE) {
			
			if (tra->ID == MAXSIZE) {
				if (tra->next->ID != 1)	tra = r->insert(tra, 1, "", 0);
				else break;
			}
			else tra = r->insert(tra, tra->ID + 1, "", 0);

		}

		return;
	}

	if (wait->recentTable) {
		c = 1;
		table* changeID = wait->recentTable;

		while (c < ganNhatID) {
			changeID = changeID->next;
			changeID->ID = c++;
		}

		int khachChoMoi = c - 1;

		while (changeID->ID != 1) changeID = changeID->next;

		table* check = orderWait->recentTable;

		table* preCheck = check;

		int khachChoCu = check->ID;

		c = 1;

		while (preCheck->next->name != check->name)
			preCheck = preCheck->next;

		while (c++ <= khachChoCu) {
			int i = 1;

			while (i++ <= khachChoMoi) {
				if (check->name == changeID->name && check->age == changeID->age) {
					check->ID *= -1;
				}
				changeID = changeID->next;
			}

			check = check->next;
			preCheck = preCheck->next;
		}

		c = 1;

		while (c++ <= khachChoCu) {

			if (check->ID > 0) {
				if (check == check->next) {
					check->next = 0;
					check = 0;

					orderWait->recentTable->next = 0;
					orderWait->recentTable = 0;

					free(orderWait->recentTable);
					break;
				}
				else {
					table* xoa = check;

					if (xoa == orderWait->recentTable) orderWait->recentTable = preCheck;

					preCheck->next = xoa->next;
					free(xoa);
				}
			}
			else {
				preCheck = preCheck->next;
			}

			check = preCheck->next;
		}

		while (orderWait->recentTable->ID < 0) {
			orderWait->recentTable->ID *= -1;
			orderWait->recentTable = orderWait->recentTable->next;
		}

		while (orderWait->recentTable->next->ID > orderWait->recentTable->ID)
			orderWait->recentTable = orderWait->recentTable->next;

		table* change = orderWait->recentTable;
		if (change) {
			int stop = wait->recentTable->ID;

			c = 1;

			while (c <= stop) {
				change = change->next;
				change->ID = c++;
			}
		}

	}
	else {

		table* check = orderWait->recentTable;

		table* preCheck = check;

		int khachChoCu = check->ID;

		c = 1;

		while (preCheck->next->name != check->name)
			preCheck = preCheck->next;

		while (c++ <= khachChoCu) {
			if (check == check->next) {
				check->next = 0;
				check = 0;

				orderWait->recentTable->next = 0;
				orderWait->recentTable = 0;

				free(orderWait->recentTable);
				break;
			}
			else {
				table* xoa = check;
				preCheck->next = xoa->next;
				free(xoa);
			}

			check = preCheck->next;
		}
	}

	r->recentTable = tra;

}

void ps(restaurant* orderIn, restaurant* orderWait, int num, bool hasID) {

	table* thuTuIn = orderIn->recentTable;

	table* thuTuWait = orderWait->recentTable;

	int c = 1, sokhachhienco = 0, soKhachCoBan = 0, soKhachCho = 0;

	if (thuTuIn) soKhachCoBan = thuTuIn->ID;

	if (thuTuWait) soKhachCho = thuTuWait->ID;

	sokhachhienco = soKhachCoBan + soKhachCho;

	if (sokhachhienco == 0) {
		cout << "Empty\n";
		return;
	}

	if (hasID && (num < 1 || num > 2 * MAXSIZE)) return;
	else {
		string out = "";

		if (thuTuIn) thuTuIn = thuTuIn->next;

		if (thuTuWait) thuTuWait = thuTuWait->next;

		if (num == 0 || num > sokhachhienco) {
			while (c++ <= soKhachCoBan) {
				out = thuTuIn->name + "\n" + out;
				thuTuIn = thuTuIn->next;
			}

			c = 1;

			while (c++ <= soKhachCho) {
				out = thuTuWait->name + "\n" + out;
				thuTuWait = thuTuWait->next;
			}

		}
		else if (num <= sokhachhienco) {
			int stop = 0, end = 0;

			if (soKhachCho >= num) {
				stop = soKhachCho - num + 1;
				end = soKhachCho;

				c = 1;

				while (c++ < stop) {
					thuTuWait = thuTuWait->next;
				}

				c = stop;

				while (c++ <= end) {
					out = thuTuWait->name + "\n" + out;
					thuTuWait = thuTuWait->next;
				}
			}
			else if (soKhachCho < num) {
				stop = soKhachCoBan + soKhachCho - num + 1;
				end = soKhachCho;
				if (end == 0) {
					end = soKhachCoBan;

					c = 1;

					while (c++ < stop) {
						thuTuIn = thuTuIn->next;
					}

					c = stop;

					while (c++ <= end) {
						out = thuTuIn->name + "\n" + out;
						thuTuIn = thuTuIn->next;
					}
				}
				else {
					c = 1;

					while (c++ < stop) {
						thuTuIn = thuTuIn->next;
					}

					c = stop;

					while (c++ <= soKhachCoBan) {
						out = thuTuIn->name + "\n" + out;
						thuTuIn = thuTuIn->next;
					}

					c = 1;

					while (c++ <= end) {
						out = thuTuWait->name + "\n" + out;
						thuTuWait = thuTuWait->next;
					}
				}
			}
		}

		cout << out;
	}

}

void pq(restaurant* orderWait, int num, bool hasID) {
	table* tra = orderWait->recentTable;

	int c = 1, sokhachhangcho = 0;

	if (orderWait->recentTable) sokhachhangcho = orderWait->recentTable->ID;

	if (sokhachhangcho == 0) {
		cout << "Empty\n";
		return;
	}

	if (hasID && (num < 1 || num > MAXSIZE)) return;

	string out = "";

	tra = tra->next;

	c = 1;
	if (num == 0 || num > sokhachhangcho) {
		while (c++ <= sokhachhangcho) {
			out = out + tra->name + "\n";
			tra = tra->next;
		}
	}
	else if (num <= sokhachhangcho) {
		while (c++ <= num) {
			if (tra->name != "") out = out + tra->name + "\n";
			tra = tra->next;
		}

	}

	cout << out;
}

void pt(restaurant* r) {
	table* recent = r->recentTable;

	int checkPoint = recent->ID;

	while (recent->next->ID != checkPoint) {
		if (recent->name != "") {
			cout << recent->ID << "-" << recent->name << "\n";
		}
		recent = recent->next;
	}

	if (recent->name != "") {
		cout << recent->ID << "-" << recent->name << "\n";
	}

}

void sq(restaurant* wait, int num) {
	
	table* tra = wait->recentTable;

	int c = 1, sokhachhangcho = 0;

	if (tra) sokhachhangcho = tra->ID;

	if (sokhachhangcho == 0) {
		cout << "Empty\n";
		return;
	}
	else if (sokhachhangcho == 1) {
		cout << tra->name << "\n";
		return;
	}

	if (num > MAXSIZE || num < 1) return;

	table* oldPre = tra;

	tra = tra->next;

	table* preTra = tra;

	table* oldPreTra = preTra;

	if (num <= sokhachhangcho) {
		c = 1;

		while (c++ <= num) {
			int maxAge = preTra->age;
			
			table* change = preTra, * preChange = preTra;
			tra = preTra->next;

			int i = 1;

			while (i++ <= sokhachhangcho - c + 2) {
			
				if (tra->age > maxAge) {
					change = tra;
					maxAge = tra->age;
				}
				if (tra->age == maxAge && tra->ID < preTra->ID) {
					change = tra;
				}

				if (i == sokhachhangcho - c + 2) break;

				tra = tra->next;
			}

			while (preChange->next->ID != change->ID) preChange = preChange->next;

			if (preTra == change) {
				oldPre = oldPre->next;
				preTra = preTra->next;
				continue;
			}

			if (change->age > oldPreTra->age) oldPreTra = change;

			tra = change->next;

			if (preTra->next == change) {
				change->next = preTra;
				preTra->next = tra;
				oldPre->next = change;
			}
			else if (change->next == preTra) {
				change->next = preTra->next;
				preTra->next = change;
				preChange->next = preTra;
				oldPre = preTra;
			}
			else {
				change->next = preTra->next;
				preTra->next = tra;
				oldPre->next = change;
				preChange->next = preTra;
			}

			oldPre = oldPre->next;
			preTra = change->next;
		}

	}
	else {
		c = 1;

		while (c++ < sokhachhangcho) {
			int maxAge = preTra->age;

			table* change = preTra, * preChange = preTra;
			tra = preTra->next;

			int i = 1;

			while (i++ <= sokhachhangcho - c + 2) {

				if (tra->age > maxAge) {
					change = tra;
					maxAge = tra->age;
				}
				if (tra->age == maxAge && tra->ID < preTra->ID) {
					change = tra;
				}
				
				if (i == sokhachhangcho - c + 2) break;

				tra = tra->next;
			}

			while (preChange->next->ID != change->ID) preChange = preChange->next;

			if (preTra == change) {
				oldPre = oldPre->next;
				preTra = preTra->next;
				continue;
			}

			if (change->age > oldPreTra->age) oldPreTra = change;

			tra = change->next;

			if (preTra->next == change) {
				change->next = preTra;
				preTra->next = tra;
				oldPre->next = change;
			}
			else if (change->next == preTra) {
				change->next = preTra->next;
				preTra->next = change;
				preChange->next = preTra;
				oldPre = preTra;
			}
			else {
				change->next = preTra->next;
				preTra->next = tra;
				oldPre->next = change;
				preChange->next = preTra;
			}

			oldPre = oldPre->next;
			preTra = change->next;
		}

	}

	c = 0;

	while (c < sokhachhangcho) {
		oldPreTra->ID = ++c;
		oldPreTra = oldPreTra->next;
	}

	while (wait->recentTable->ID != 1) wait->recentTable = wait->recentTable->next;

	while (wait->recentTable->ID != sokhachhangcho) {
		cout << wait->recentTable->name << endl;
		wait->recentTable = wait->recentTable->next;
	}

	cout << wait->recentTable->name << endl;
}