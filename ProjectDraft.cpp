
#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>
#include<stdlib.h>
#include<vector>
#include<Windows.h>
#include<list>
using namespace std;
//	C:\Users\Omar\Desktop\ProjectDraft\ProjectDraft\candidate_data

class candidate {
public:
	candidate* next;
	string party_name; // N/A for independent candidate
	string candidate_name;
	string symbol; //holds unique symbol
	vector <string> constituency; //template for storing constituency names
	string candidate_ID; //unique ID for every candidate assigned automatically
	string status; //registered for independent
	int vote;
	candidate() : vote(0)
	{}
};
class p_candidate : public candidate {
	candidate* head = NULL;
	candidate* tail = NULL;

public:
	void clear_buffer()
	{
		head = NULL;
		tail = NULL;
	}
	void approve_politcal_candidate(string pname)
	{
		clear_buffer();
		file_to_buffer();
		int i = 0;
		candidate* trav;
		trav = head;
		vector <string> party_cand;
		vector <string> party_cand_n;
		while (trav != NULL)
		{
			if (trav->party_name == pname)
			{
				party_cand.push_back(trav->candidate_ID);
				party_cand_n.push_back(trav->candidate_name);
				cout << "Candidate #" << ++i << ": " << endl;
				cout << "Candidate Name: " << trav->candidate_name << endl;
				cout << "Candidate ID: " << trav->candidate_ID << endl;
				cout << "Candidate Status: " << trav->status << endl;
				cout << "Candidate Party: " << pname << endl;
				cout << "Candidate Symbol: " << trav->symbol << endl;
				cout << "List of constituencies the candidate is registered in: " << endl;
				for (int j = 0; j < trav->constituency.size(); j++)
				{
					cout << trav->constituency.at(j) << endl;
				}
				cout << endl;
			}
			trav = trav->next;
		}

		if (i == 0)
		{
			cout << "No political candidates to approve or reject." << endl;
			system("pause");
			return;
		}
		string T_ID;
		string name;
		cout << endl << "Enter the ID the candidate you wish to approve/reject: ";
		cin.ignore();
		getline(cin, T_ID);

		while (true)
		{
			bool found = false;
			for (int i = 0; i < party_cand.size(); i++)
			{
				if (T_ID == party_cand.at(i))
				{
					name = party_cand_n.at(i);
					found = true;
					break;
				}
			}
			if (found)
				break;
			else
			{
				cout << "Incorrect candidate ID entered. Please try again: ";
				getline(cin, T_ID);
			}
		}
		int p;
		cout << endl << name << " is selected.";
		cout << endl << "1. Approve";
		cout << endl << "2. Reject";
		cout << "\nEnter a valid choice: ";
		cin >> p;
		while (true)
		{
			if ((p == 1) || (p == 2))
				break;
			cout << "Invalid input entered! Enter a valid choice: ";
			cin >> p;
		}

		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("political_candidate_data\\");

		trav = head;
		system("pause");
		fstream a_file;
		if (p == 1)
		{
			while (trav != NULL)
			{
				if (trav->candidate_ID == T_ID)
				{
					a_file.open(addr + T_ID + ".txt");
					a_file << "0" << endl;
					a_file << trav->candidate_name << endl;
					a_file << trav->party_name << endl;
					a_file << trav->symbol << endl;
					a_file << "registered" << endl;
					for (int j = 0; j < trav->constituency.size(); j++)
					{
						a_file << trav->constituency.at(j) << endl;
					}
					a_file.close();
					delete trav;
					break;
				}
				trav = trav->next;
			}
			cout << "Candidate approved!";
			cout << endl;
			clear_buffer();
			file_to_buffer();
			return;
		}

		if (p == 2)
		{
			vector <string> list;
			string check;
			addr += T_ID + ".txt";
			char* addr_ = const_cast<char*>(addr.c_str());
			remove(addr_); // Step 1

			a_file.open("p_candidate_name.txt", ios::in);
			while (!a_file.eof())
			{
				getline(a_file, check);
				list.push_back(check);
			}
			a_file.close();

			a_file.open("p_candidate_name.txt", ios::out);
			for (int i = 0; i < list.size(); i++)
			{
				if (list.at(i) != T_ID)
					a_file << list.at(i) << endl;
			}
			a_file.close();// step 2


			if (head->candidate_ID == T_ID)
			{
				head = head->next;
			}
			else
			{
				candidate* current = head->next;
				candidate* prev = head;
				while (current != NULL)
				{
					if (current->candidate_ID == T_ID)
					{
						prev->next = current->next;
						tail = prev;
						break;
					}
					current = current->next;
					prev = prev->next;
				}
				delete current;
				delete prev;
			}//step3
			cout << "Candidate rejected successfully." << endl;
			system("pause");
		}

		clear_buffer();
		file_to_buffer();
	}
	bool fetch_ID(candidate* n_cand)
	{
		string ID;
		cout << "Enter candidate ID: ";
		getline(cin, ID);
		candidate* traverse;
		traverse = head;
		while (true)
		{
			if (traverse == NULL)
				break;
			if (traverse->candidate_ID == ID)
			{
				cout << "ID already taken." << endl;
				return true;
				system("pause");
			}
			traverse = traverse->next;
		}
		delete traverse;
		n_cand->candidate_ID = ID;
		return false;
	}
	bool verify_constituency(string text) //returns true if constituency does not exist
	{
		fstream temp;
		string cmpr;
		temp.open("constituency_list.txt", ios::in);
		while (!temp.eof())
		{
			getline(temp, cmpr);
			if (cmpr == text)
			{
				return false;
			}
		}
		return true;
	}
	bool check_register_constituency(candidate* n_cand, string text)
	{
		int flag = 0;
		bool ves = false; //false if constituency has been registered in prior to registration
		bool ver = true; //false if constituency exists

		ver = verify_constituency(text);
		if (ver == true)
		{
			cout << "\nConstituency does not exist." << endl;
			system("pause");
		}
		else
		{ //ver is false
			ver = false;
			int i = 0;
			for (int i = 0; i < n_cand->constituency.size(); i++)
			{
				if (n_cand->constituency.at(i) == text)
				{
					cout << "Constituency has already been registered in.";
					cout << endl;
					system("pause");
					ves = true;
				}
			}
		}
		return ver || ves;
	}
	void register_constituency(candidate* n_cand)
	{
		string text;
		bool crc = true;
		while (crc)
		{
			cout << "\nEnter constituency you'd like to register for: " << endl;
			getline(cin, text);
			crc = check_register_constituency(n_cand, text);
			if (crc == false)
				n_cand->constituency.push_back(text); //add constiteucny to linked list
		}
	}
	void addcandidate()
	{
		string text, cmpr, symbol, ID;
		bool verify = true;
		candidate* n_cand = new candidate;
		cout << "Enter candidate name: ";
		getline(cin, n_cand->candidate_name); // get name in linked list
		n_cand->status = "unregistered"; //Status is unregistered until confirmation.
		while (fetch_ID(n_cand)) {}
		fstream p_shw;
		p_shw.open("political_party.txt", ios_base::in);
		int i = 0;
		getline(p_shw, text);
		while (!p_shw.eof())
		{
			if (text == "")
			{
				break;
			}
			cout << ++i << ". " << text << endl;
			getline(p_shw, text);
		}
		p_shw.close();
		cout << "Enter party name: ";
		while (verify)
		{
			getline(cin, text);
			verify = verify_party(text);
		}
		n_cand->party_name = text; //add party name to linked list
		//assign party symbol to candidate
		p_shw.open("party_symbol.txt", ios::in);
		while (!p_shw.eof())
		{
			getline(p_shw, text);
			if (n_cand->party_name == text)
				break;
		}
		getline(p_shw, text);
		n_cand->symbol = text; //add symbol to linked list
		p_shw.close();
		//Register constituency
		cout << "\nChoose a constituency to register in: \n";
		fstream shw_const;
		shw_const.open("constituency_list.txt", ios_base::in);
		while (!shw_const.eof())
		{
			getline(shw_const, text);
			cout << text << endl;
		}
		shw_const.close();

		bool flag = true;
		while (flag)
		{
			int ch;
			register_constituency(n_cand);
			cout << "\n1.Register in another constituency.";
			cout << "\n2.Exit.";
			cin >> ch;
			cin.ignore();
			while (ch < 0 || ch > 2)
			{
				cout << "\n***Please enter a valid input***";
				cin >> ch;
				cin.ignore();
			}

			if (ch == 1)
				continue;
			else
				flag = false;
			break;
		}

		ofstream candidate_name_file;
		candidate_name_file.open("p_candidate_name.txt", ios::app);
		candidate_name_file << n_cand->candidate_ID << endl;
		candidate_name_file.close();
		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("political_candidate_data\\");
		//addr<--has address of the directory
		/*cout << addr;
		system("PAUSE");*/
		_mkdir("political_candidate_data");
		string file_name = n_cand->candidate_ID;
		ofstream ofile;
		system("pause");
		ofile.open(addr + file_name + ".txt", ios_base::app);
		ofile << "0" << endl;
		ofile << n_cand->candidate_name << endl;
		ofile << n_cand->party_name << endl;
		ofile << n_cand->symbol << endl;
		ofile << n_cand->status << endl;
		for (int i = 0; i < n_cand->constituency.size(); i++)
		{
			ofile << n_cand->constituency.at(i) << endl;
		}
		ofile.close();

		clear_buffer();
		file_to_buffer();//add new data to linked list
	}
	bool verify_party(string text) // Returns false is party is valid;
	{
		bool torf = true;
		string cmpr;
		fstream party_check("political_party.txt", ios::in);
		while (!party_check.eof())
		{
			getline(party_check, cmpr);
			if (cmpr == text)
			{
				return false;
			}
		}
		party_check.close();
		cout << "Enter a valid party: ";
		return true;
	}
	void add_to_buffer(string text)
	{
		candidate* newdata = new candidate;
		newdata->candidate_ID = text;
		newdata->next = NULL;
		if (head == NULL)
		{
			head = newdata;
			tail = newdata;
		}
		else
		{
			tail->next = newdata;
			tail = newdata;
		}
		fstream ifile;

		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("political_candidate_data\\");
		ifile.open(addr + text + ".txt", ios_base::in);
		string temp;
		getline(ifile, temp);
		newdata->vote = std::stoi(temp);
		temp.clear();
		getline(ifile, newdata->candidate_name);
		getline(ifile, newdata->party_name);
		getline(ifile, newdata->symbol);
		getline(ifile, newdata->status);
		while (!ifile.eof())
		{
			getline(ifile, temp);
			newdata->constituency.push_back(temp);
		}
		ifile.close();
	}
	void file_to_buffer()
	{
		string text;
		fstream i_candidate;
		i_candidate.open("p_candidate_name.txt", ios::in);
		if (i_candidate.is_open())
		{
			while (!i_candidate.eof())
			{
				getline(i_candidate, text);
				if (!text.empty())
					add_to_buffer(text);
			}
		}
		i_candidate.close();
	}
	void display_pcandidate()const
	{
		cout << "\nDisplaying all party candidates: " << endl;

		int i = 0;
		candidate* traverse;
		traverse = head;
		while (true)
		{
			if (traverse == NULL)
				break;
			cout << "Candidate #" << ++i << ": " << endl;
			cout << "Candidate Name: " << traverse->candidate_name << endl;
			cout << "Candidate ID: " << traverse->candidate_ID << endl;
			cout << "Candidate Party: " << traverse->party_name << endl;
			cout << "Candidate Symbol: " << traverse->symbol << endl;
			cout << "Candidate vote: " << traverse->vote << endl;
			cout << "List of constituencies the candidate is registered in: " << endl;
			for (int j = 0; j < traverse->constituency.size(); j++)
			{
				cout << traverse->constituency.at(j) << endl;
			}
			cout << endl;
			traverse = traverse->next;
		}
		delete traverse;
	}
	int length()
	{
		candidate* traverse = head;
		int count = 0;
		while (traverse != NULL)
		{
			count++;
			traverse = traverse->next;
		}
		return count;
	}
	void buffer_sort()
	{
		candidate* prev;
		prev = head;
		if ((prev == NULL) || (prev->next == NULL))
			return; // deals with the issue when there's only one data item

		candidate* current = head->next;
		//cout << current->candidate_ID;
		//system("pause");

		if ((stoi(prev->candidate_ID)) > stoi((current->candidate_ID)))
		{
			prev->next = current->next;
			current->next = prev;
			head = current; // this works
		}

		if (length() < 3)
			return;
		else
		{
			int s = length();
			candidate* nextt = current->next;
			for (int i = 0; i < s; i++)
			{
				prev = head;
				current = head->next;
				nextt = current->next;
				while (true)
				{
					if ((stoi(current->candidate_ID)) > stoi((nextt->candidate_ID)))
					{
						prev->next = nextt;
						current->next = nextt->next;
						nextt->next = current;

						current = nextt;
						nextt = nextt->next;
					}
					else
					{
						nextt = nextt->next;
						current = current->next;
						prev = prev->next;
					}
					if (nextt == NULL)
						break;
				}
			}
		}
		candidate* traverse = head;
		while (true)
		{
			traverse = traverse->next;
			if (traverse->next == NULL)
			{
				tail = traverse;
				break;
			}
		}

	}
	bool find_ID(string ID) //returns true if candidate does not exist.
	{
		candidate* traverse = head;
		while (traverse != NULL)
		{
			if (traverse->candidate_ID == ID)
				return false;
			traverse = traverse->next;
		}
		return true;
	}
	bool candidate_constituency(string cnd_ID, string voter_constituency)
	{
		candidate* trav = head;
		while (trav != NULL)
		{
			if (trav->candidate_ID == cnd_ID)
				break;
			trav = trav->next;
		}
		cout << trav->candidate_ID;
		system("pause");
		for (int i = 0; i < trav->constituency.size(); i++)
		{
			if (voter_constituency == trav->constituency.at(i))
				return false;
		}
		return true;
	}
	void cast_vote(string cnd_ID)
	{
		candidate* traverse = head;
		while (traverse != NULL)
		{
			if (traverse->candidate_ID == cnd_ID)
				break;
			traverse = traverse->next;
		}
		traverse->vote++;
		system("pause");
		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("candidate_data\\");
		//addr<--has address of the directory
		_mkdir("candidate_data");
		string file_name = traverse->candidate_ID;
		ofstream ofile;
		ofile.open(addr + file_name + ".txt");
		ofile << traverse->vote << endl;
		ofile << traverse->candidate_name << endl;
		ofile << traverse->party_name << endl;
		ofile << traverse->symbol << endl;
		ofile << traverse->status << endl;
		for (int i = 0; i < traverse->constituency.size(); i++)
		{
			ofile << traverse->constituency.at(i) << endl;
		}
		ofile.close();

	};
	string return_party(string cnd_ID)
	{
		candidate* trav = head;
		while (trav != NULL)
		{
			if (trav->candidate_ID == cnd_ID)
				break;
			trav = trav->next;
		}
		return trav->party_name;
	}
};



class i_candidate : public candidate {
	candidate* head = NULL;
	candidate* tail = NULL;
public:
	int length()
	{
		candidate* traverse = head;
		int count = 0;
		while (traverse != NULL)
		{
			count++;
			traverse = traverse->next;
		}
		return count;
	}
	void clear_buffer()
	{
		head = NULL;
		tail = NULL;
	}
	void display_icandidate()const
	{
		cout << "\nDisplaying all independent candidates: " << endl;

		int i = 0;
		candidate* traverse;
		traverse = head;
		while (true)
		{
			if (traverse == NULL)
				break;
			cout << "Candidate #" << ++i << ": " << endl;
			cout << "Candidate Name: " << traverse->candidate_name << endl;
			cout << "Candidate ID: " << traverse->candidate_ID << endl;
			cout << "Candidate Symbol: " << traverse->symbol << endl;
			cout << "Candidate Vote: " << traverse->vote << endl;
			cout << "List of constituencies the candidate is registered in: " << endl;
			for (int j = 0; j < traverse->constituency.size(); j++)
			{
				cout << traverse->constituency.at(j) << endl;
			}
			cout << endl;
			traverse = traverse->next;
		}

	}
	void add_to_buffer(string text)
	{
		candidate* newdata = new candidate;
		newdata->candidate_ID = text;
		newdata->next = NULL;
		if (head == NULL)
		{
			head = newdata;
			tail = newdata;
		}
		else
		{
			tail->next = newdata;
			tail = newdata;
		}
		fstream ifile;

		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("candidate_data\\");
		//addr<--has address of the directory
		_mkdir("candidate_data");
		ifile.open(addr + text + ".txt", ios_base::in);
		string temp;
		getline(ifile, temp);
		newdata->vote = std::stoi(temp);
		temp.clear();
		getline(ifile, newdata->candidate_name);
		getline(ifile, newdata->symbol);
		while (!ifile.eof())
		{
			getline(ifile, temp);
			newdata->constituency.push_back(temp);
		}
		ifile.close();
	}
	void file_to_buffer()
	{
		string text;
		fstream i_candidate;
		i_candidate.open("i_candidate_name.txt", ios::in);
		if (i_candidate.is_open())
		{
			while (!i_candidate.eof())
			{
				getline(i_candidate, text);
				if (!text.empty())
					add_to_buffer(text);
			}
		}
		i_candidate.close();
	}
	bool fetch_ID(candidate* n_cand)
	{
		string ID;
		cout << "Enter candidate ID: ";
		getline(cin, ID);
		candidate* traverse;
		traverse = head;
		while (true)
		{
			if (traverse == NULL)
				break;
			if (traverse->candidate_ID == ID)
			{
				cout << "ID already taken." << endl;
				return true;
				system("pause");
			}
			traverse = traverse->next;
		}
		delete traverse;
		n_cand->candidate_ID = ID;
		return false;
	}
	void addcandidate()
	{
		string text, cmpr, symbol;
		bool ver = true;
		ofstream sym;
		candidate* n_cand = new candidate;
		cout << "Enter candidate name: ";
		getline(cin, n_cand->candidate_name); // get name
		n_cand->party_name = "N/A";

		while (fetch_ID(n_cand)) {}
		while (ver)
		{
			cout << "\nEnter an appropriate symbol";
			cout << "\nSymbol can't be an existing political party.";
			cout << "\nEnter a valid symbol: ";
			getline(cin, text);
			ver = add_symbol(text);
			if (ver == true)
			{
				cout << " is already taken. Please choose another symbol." << endl;
			}
			else
			{
				n_cand->symbol = text; // add symbol to linked list
				sym.open("independent_symbol.txt", ios::app);
				sym << endl;
				sym.close();
				sym.open("independent_symbol.txt", ios::app);
				sym << n_cand->symbol;
				sym.close();
			}
		}
		//var text can be manipulated after this line
		cout << "\nChoose a constituency to register in: \n";
		fstream shw_const;
		shw_const.open("constituency_list.txt", ios_base::in);
		while (!shw_const.eof())
		{
			getline(shw_const, text);
			cout << text << endl;
		}
		shw_const.close();

		bool flag = true;
		while (flag)
		{
			int ch;
			register_constituency(n_cand);
			cout << "\n1.Register in another constituency.";
			cout << "\n2.Exit.";
			cin >> ch;
			cin.ignore();
			while (ch < 0 || ch > 2)
			{
				cout << "\n***Please enter a valid input***";
				cin >> ch;
				cin.ignore();
			}

			if (ch == 1)
				continue;
			else
				flag = false;
			break;
		}
		ofstream candidate_name_file;
		candidate_name_file.open("i_candidate_name.txt", ios::app);
		candidate_name_file << n_cand->candidate_ID << endl;
		candidate_name_file.close();
		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("candidate_data\\");
		//addr<--has address of the directory
		_mkdir("candidate_data");
		string file_name = n_cand->candidate_ID;
		ofstream ofile;
		
		ofile.open(addr + file_name + ".txt", ios_base::app);
		ofile << "0" << endl;
		ofile << n_cand->candidate_name << endl;
		ofile << n_cand->symbol << endl;
		for (int i = 0; i < n_cand->constituency.size(); i++)
		{
			ofile << n_cand->constituency.at(i) << endl;
		}
		ofile.close();
	}
	void register_constituency(candidate* n_cand)
	{
		string text;
		bool crc = true;
		while (crc)
		{
			cout << "\nEnter constituency you'd like to register for: " << endl;
			getline(cin, text);
			crc = check_register_constituency(n_cand, text);
			if (crc == false)
				n_cand->constituency.push_back(text);
		}
	}
	bool verify_constituency(string text) //returns true if constituency does not exist
	{
		fstream temp;
		string cmpr;
		temp.open("constituency_list.txt", ios::in);
		while (!temp.eof())
		{
			getline(temp, cmpr);
			if (cmpr == text)
			{
				return false;
			}
		}
		return true;
	}
	bool add_symbol(string text) //returns true if symbol is taken
	{
		string cmpr = text;
		ifstream check;
		bool flag1 = false;
		bool flag2 = false;
		string s_shw;

		check.open("party_symbol.txt");
		while (!check.eof())
		{
			getline(check, s_shw);
			if (s_shw == cmpr)
			{
				cout << s_shw;
				flag1 = true;
			}
		}
		check.close();
		check.open("independent_symbol.txt");
		while (!check.eof())
		{
			getline(check, s_shw);
			if (s_shw == cmpr)
			{
				cout << s_shw;
				flag2 = true;
			}
		}
		check.close();
		return flag1 || flag2;
	}
	bool check_register_constituency(candidate* n_cand, string text)
	{
		int flag = 0;
		bool ves = false; //false if constituency has been registered in prior to registration
		bool ver = true; //false if constituency exists

		ver = verify_constituency(text);
		if (ver == true)
		{
			cout << "\nConstituency does not exist." << endl;
			system("pause");
		}
		else
		{ //ver is false
			ver = false;
			int i = 0;
			for (int i = 0; i < n_cand->constituency.size(); i++)
			{
				if (n_cand->constituency.at(i) == text)
				{
					cout << "Constituency has already been registered in.";
					cout << endl;
					system("pause");
					ves = true;
				}
			}
		}
		return ver || ves;
	}
	void buffer_sort()
	{
		candidate* prev;
		prev = head;
		if ((prev == NULL) || (prev->next == NULL))
			return; // deals with the issue when there's only one data item

		candidate* current = head->next;
		//cout << current->candidate_ID;
		//system("pause");

		if ((stoi(prev->candidate_ID)) > stoi((current->candidate_ID)))
		{
			prev->next = current->next;
			current->next = prev;
			head = current; // this works
		}

		if (length() < 3)
			return;
		else
		{
			int s = length();
			candidate* nextt = current->next;
			for (int i = 0; i < s; i++)
			{
				prev = head;
				current = head->next;
				nextt = current->next;
				while (true)
				{
					if ((stoi(current->candidate_ID)) > stoi((nextt->candidate_ID)))
					{
						prev->next = nextt;
						current->next = nextt->next;
						nextt->next = current;

						current = nextt;
						nextt = nextt->next;
					}
					else
					{
						nextt = nextt->next;
						current = current->next;
						prev = prev->next;
					}
					if (nextt == NULL)
						break;
				}
			}
		}
		candidate* traverse = head;
		while (true)
		{
			traverse = traverse->next;
			if (traverse->next == NULL)
			{
				tail = traverse;
				break;
			}
		}

	}
	bool find_ID(string ID)
	{
		candidate* traverse = head;
		while (traverse != NULL)
		{
			if (traverse->candidate_ID == ID)
				return false;
			traverse = traverse->next;
		}
		return true;
	}
	bool candidate_constituency(string cnd_ID, string voter_constituency) //returns false if candidate exists in constituency
	{
		candidate* trav = head;
		while (trav != NULL)
		{
			if (trav->candidate_ID == cnd_ID)
				break;
			trav = trav->next;
		}
		for (int i = 0; i < trav->constituency.size(); i++)
		{
			if (voter_constituency == trav->constituency.at(i))
				return false;
		}
		
		return true;
	}
	void cast_vote(string cnd_ID)
	{
		candidate* traverse = head;
		while (traverse != NULL)
		{
			if (traverse->candidate_ID == cnd_ID)
				break;
			traverse = traverse->next;
		}
		traverse->vote++;
		system("pause");
		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("candidate_data\\");
		//addr<--has address of the directory
		_mkdir("candidate_data");
		string file_name = traverse->candidate_ID;
		ofstream ofile;
		ofile.open(addr + file_name + ".txt");
		ofile << traverse->vote<<endl;
		ofile << traverse->candidate_name << endl;
		ofile << traverse->symbol << endl;
		for (int i = 0; i < traverse->constituency.size(); i++)
		{
			ofile << traverse->constituency.at(i) << endl;
		}
		ofile.close();

	}

	
};
class element {
public:
	element* next;
	element* previous;
	string voter_status;
	string constituency_name;
	string CNIC;
	string password;
};
class d_constituency : public element {
	element* head = NULL;
	element* tail = NULL;
public:

	void add(string text)
	{
		element* newdata = new element;
		newdata->constituency_name = text;
		newdata->next = NULL;

		if (head == NULL)
		{
			head = newdata;
			tail = newdata;
			newdata->previous = NULL;
		}
		else
		{
			tail->next = newdata;
			newdata->previous = tail;
			tail = newdata;
		}
	}
	void file_to_buffer()
	{
		string text;
		fstream constituency;
		constituency.open("constituency_list.txt", ios::in);
		if (constituency.is_open())
		{
			while (!constituency.eof())
			{
				getline(constituency, text);
				if (!text.empty())
					add(text);
			}
		}
		constituency.close();
	}
	void display() const
	{
		element* traverse;
		traverse = head;
		if (traverse == NULL)
		{
			cout << "Nothing to see here..." << endl;
			system("pause");
			return;
		}
		else
		{
			int i = 0;
			cout << "Displaying all constituencies!" << endl;
			while (true)
			{
				cout << ++i << "\t" << traverse->constituency_name << endl;
				traverse = traverse->next;

				if (traverse == NULL)
				{
					break;
				}
			}
		}
	}
	void buffer_to_file()
	{
		ofstream constituency;
		constituency.open("constituency_list.txt", ios::trunc);
		element* traverse = head;
		while (traverse->next != NULL)
		{
			constituency << traverse->constituency_name << endl;
			traverse = traverse->next;
		}
		constituency << traverse->constituency_name;
		constituency.close();
	}
	void add_constituency()
	{
		display();
		cin.ignore();
		bool p = false;
		string text;
		cout << "Enter the name of the constituency: ";
		getline(cin, text);
		cout << text;
		p = search_for_constituency(text);
		if (p)
		{
			cout << "\nConstituency already exists.";
			return;
		}
		add(text);
		buffer_to_file();
		cout << "Operation successful!";
		system("pause");
	}
	bool search_for_constituency(string text)
	{
		element* traverse = head;
		while (traverse->next != NULL)
		{
			traverse = traverse->next;
			if (traverse->constituency_name == text)
			{
				return true;
			}
		}
		return false;
	}
	void delete_constituency()
	{
		cin.ignore();
		element* prev = new element;
		element* current = new element;
		string text;
		cout << endl;
		if (head == NULL)
		{
			cout << "\nNothing to delete.";
			cout << endl;
			system("pause");
			return;
		}
		else
		{
			display();
			cout << endl << "Enter the name of the constituency you wish to delete: ";
			getline(cin, text);
		}
		prev = head;
		current = head->next;
		bool isfound = false;
		if (text == head->constituency_name)
		{
			head = head->next;
			cout << endl << "Delete operation successful!";
			system("pause");
			isfound = true;
			buffer_to_file();
			return;
		}
		else
		{
			while (text != current->constituency_name && current->next != NULL)
			{
				current = current->next;
				prev = prev->next;
			}
			if (current->constituency_name == text)
			{
				isfound = true;
				prev->next = current->next;
				delete(current);
				cout << endl << "Delete operation successful!";
				buffer_to_file();
			}
			else
			{
				cout << "Constituency does not exist." << endl;
				system("pause");
				return;
			}
		}
	}
	void add_political_party()
	{
		cin.ignore();
		string text, p_name, cmpr_text;
		bool flag = true;
		fstream p_party;
		cout << "\nEnter the name of the Political Party: ";
		getline(cin, text);
		p_party.open("political_party.txt", ios::in);
		while (!p_party.eof())
		{
			getline(p_party, cmpr_text);
			if (cmpr_text == text)
			{
				cout << "\nError! Party already exists!" << endl;
				system("pause");
				return;
			}
		}
		p_party.close();
		p_party.open("political_party.txt", ios::app);
		p_party << text << endl;
		cout << "\nPolitical Party Registration successful!";
		p_party.close();

		p_name = text;
		//Assign password
		p_party.open("political_party_login.txt", ios::app);
		p_party << text << endl;
		cout << "\n\nEnter a password: ";
		getline(cin, text);
		p_party << text << endl;
		p_party.close();

		while (flag)
		{
			flag = assign_party_symbol(p_name);
			if (flag == false)
				break;
		}

	}
	bool assign_party_symbol(string p_name)
	{
		string text, cmpr_text;
		fstream p_party;
		//Assign symbol
		cout << "\nEnter a symbol: ";
		p_party.open("party_symbol.txt", ios::in);
		getline(cin, text);
		while (!p_party.eof())
		{
			getline(p_party, cmpr_text);
			if (cmpr_text == text)
			{
				cout << "\nError! Party symbol exists!" << endl;
				system("pause");
				return true;
			}
		}
		p_party.close();
		p_party.open("independent_symbol.txt", ios::in);
		while (!p_party.eof())
		{
			getline(p_party, cmpr_text);
			if (cmpr_text == text)
			{
				cout << "\nError! Party symbol exists!" << endl;
				system("pause");
				return true;
			}
		}
		p_party.close();
		p_party.open("party_symbol.txt", ios::app);
		p_party << p_name << endl;
		p_party << text << endl;
		p_party.close();
		return false;
	}
};
class voter : public element {
	element* head = NULL;
	element* tail = NULL;

public:
	void clear_buffer()
	{
		element* store, *p;
		p = head;
		while (p != NULL)
		{
			store = p->next;
			free(p);
			p = store;
		}
		head = NULL;
		tail = NULL;
	}
	void add_to_buffer(string text)
	{
		element* newdata = new element;
		newdata->CNIC = text;
		newdata->next = NULL;
		if (head == NULL)
		{
			head = newdata;
			tail = newdata;
			newdata->previous = NULL;
		}
		else
		{
			newdata->previous = tail;
			tail->next = newdata;
			tail = newdata;
		}
		fstream ifile;

		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("voters\\");
		ifile.open(addr + text + ".txt", ios_base::in);
		string temp;
		getline(ifile, newdata->constituency_name);
		getline(ifile, newdata->voter_status);
		ifile.close();
	}
	void file_to_buffer()
	{
		string text;
		ifstream voter;
		voter.open("Voter_ID.txt");
		if (voter.is_open())
		{
			while (!voter.eof())
			{
				getline(voter, text);
				if (!text.empty())
					add_to_buffer(text);
			}
		}
		voter.close();
	}
	void register_voter(d_constituency d)
	{
		fstream ID;
		system("cls");
		cin.ignore();
		string CNIC, pw, voter_const;
		cout << "Enter CNIC (Note that CNIC should exclusively be 13 digits): ";
		bool flag = true;
		while (flag)
		{
			getline(cin, CNIC);
			flag = false;
			if (CNIC.length() == 13)
			{
				for (int i = 0; i < CNIC.length(); i++)
				{
					if (CNIC.at(i) > 57 || CNIC.at(i) < 48)
					{

						cout << "CNIC contains unauthorized characters." << endl;
						cout << "Please re-enter CNIC: ";
						flag = true;
						break;
					}
				}
			}
			else
			{
				cout << "CNIC isn't 13 digits long!" << endl;
				cout << "Please re-enter CNIC: ";
				flag = true;
			}

			if (flag == false)
			{
				flag = check_ID(CNIC);
				if (flag == true)
					cout << "CNIC already in use. Please re-enter CNIC: ";
			}
		}
		//CNIC's are stored in VOTER_ID
		ID.open("Voter_ID.txt", ios::app);
		ID << CNIC << endl;
		ID.close();
		cout << "\nPlease enter a password: ";
		getline(cin, pw);
		cout << endl;
		d.display();
		cout << "\nPlease enter a constituency to register the voter in: ";
		getline(cin, voter_const);
		while (verify_constituency(voter_const)) {
			cout << "Constituency does not exist. Please choose another constituency: ";
			getline(cin, voter_const);
		}

		//save login details of the voter
		ofstream voter;
		voter.open("voter_login.txt", ios::app);
		voter << CNIC << endl;
		voter << pw << endl;
		voter.close();

		_mkdir("voters");
		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("voters\\");
		voter.open(addr + CNIC + ".txt", ios::out);
		voter << voter_const << endl;
		voter << "Vote_Pending" << endl;
		voter.close();
		clear_buffer();
		file_to_buffer();
	}
	bool verify_constituency(string text) //returns true if constituency does not exist
	{
		fstream temp;
		string cmpr;
		temp.open("constituency_list.txt", ios::in);
		while (!temp.eof())
		{
			getline(temp, cmpr);
			if (cmpr == text)
			{
				return false;
			}
		}
		return true;
	}
	bool check_ID(string ID) // returns true if CNIC is already taken.
	{
		fstream IDC;
		string cmpr;
		IDC.open("Voter_ID.txt", ios::in);
		while (!IDC.eof())
		{
			getline(IDC, cmpr);
			if (cmpr == ID)
			{
				return true;
			}
		}
		IDC.close();
		return false;
	}
	void display_results()
	{
		element* traverse = head;
		while (traverse!= NULL)
		{
			cout << "\nVoter ID: " << traverse->CNIC;
			cout << "\nVoter Status: " << traverse->voter_status;
			cout << "\nVoter constituency: " << traverse->constituency_name;
			traverse = traverse->next;
			cout << endl;
		}
		delete traverse;
	}
	bool verify_CNIC(string CNIC) //returns true if CNIC not found
	{
		element* traverse;
		traverse = head;

		while (traverse != NULL)
		{
			if (traverse->CNIC == CNIC)
			{
				return false;
			}
			traverse = traverse->next;
		}
		return true;

	}
	string find_voter_status(string ID)
	{
		element* traverse = head;
		while (traverse != NULL)
		{
			if ((traverse->CNIC) == ID)
			{
				break;
			}
			traverse = traverse->next;
		}
		
		return traverse->voter_status;
	}
	string find_voter_constituency(string CNIC)
	{
		element* traverse = head;
		while (traverse != NULL)
		{
			if (traverse->CNIC == CNIC)
			break;
			traverse = traverse->next;
		}
		return traverse->constituency_name;
	}
	void update_status(string CNIC, string new_status)
	{
		element* traverse = head;
		while (traverse != NULL)
		{
			if (traverse->CNIC == CNIC)
				break;
			traverse = traverse->next;
		}
		traverse->voter_status = new_status;
		ofstream voter;
		_mkdir("voters");
		string cppaddress(__FILE__);
		size_t pos = cppaddress.find("ProjectDraft.cpp");
		string addr = cppaddress.substr(0, pos);
		addr.append("voters\\");
		voter.open(addr + CNIC + ".txt", ios::out);
		voter << traverse->constituency_name << endl;
		voter << traverse->voter_status << endl;
		voter.close();
	}
};
void commission_portal(d_constituency d1, i_candidate i, p_candidate p, voter v);
void political_party_portal(string pname, p_candidate p);
void voter_log(d_constituency d1, i_candidate i, p_candidate p, voter v);
void voter_portal(d_constituency d1, i_candidate i, p_candidate p, voter v, string username, string password);
int main()
{
	voter v;
	v.file_to_buffer();
	d_constituency constituency;
	constituency.file_to_buffer();
	i_candidate i;
	i.file_to_buffer();
	p_candidate p;
	p.file_to_buffer();

	//string voter_constituency = v.find_voter_constituency("4220180086439"); //finds the constituency of voter
	//cout << voter_constituency;
	//system("pause");
	/*string piss = v.find_voter_status("4220180086437");
	cout << piss;*/
	unsigned int Choice;
	string password, text;
	fstream EC_password_directory;
	fstream p_party;
	string pname, cmpr;
	bool verified = false;
	while (true)
	{
		cout << "***Welcome to Election Management System!***";
		cout << "\n1.Election Commission Login." << endl;
		cout << "2.Political Party Login." << endl;
		cout << "3.Voter Login." << endl;

		cout << endl << "Type your choice here: "; cin >> Choice;


		switch (Choice)
		{
		case 1:

			system("cls");
			cout << "***Weldome to Election Commission login!***";
			cout << "\nEnter password: ";
			cin.ignore();
			getline(cin, password);

			EC_password_directory.open("EC_password.txt", ios::in);
			getline(EC_password_directory, text);
			if (text == password)
			{
				cout << "Login successful!" << endl;
				system("pause");
				cout << endl;
				commission_portal(constituency, i, p, v);
			}
			else
			{
				cout << "\nLogin attempt failed!" << endl;
				system("pause");
				break;
			}
			break;

		case 2:
			system("cls");
			cout << "***Weldome to Politcal Party login!***";
			cout << "\nEnter party name: ";
			cin.ignore();
			getline(cin, pname);
			p_party.open("political_party.txt", ios::in);
			getline(p_party, cmpr);
			while (!p_party.eof())
			{
				if (cmpr == pname)
				{
					verified = true;
					break;
				}
				getline(p_party, cmpr);
			}
			p_party.close();
			if (verified)
			{
				political_party_portal(cmpr, p);
			}
			else
			{
				cout << "\nIncorrect username entered!" << endl;
				system("pause");
			}

			break;
		case 3:
		/*	v.display_results();
			system("pause");*/
			voter_log(constituency, i, p, v);
			break;
		}
	}
}
void vote_candidate(i_candidate i, p_candidate p, voter v, string CNIC, string password);
void political_party_portal(string pname, p_candidate p)
{
	fstream pparty;
	bool correct_pass = true; //false if pass matches;
	string pw, cpw;
	pparty.open("political_party_login.txt", ios::in);
	while (!pparty.eof())
	{
		if (pname == cpw)
			break;

		getline(pparty, cpw);
	}
	getline(pparty, cpw);
	pparty.close();

	while (correct_pass)
	{
		cout << "Enter password: ";
		getline(cin, pw);
		if (pw == cpw)
			correct_pass = false;
		else
			cout << "\nIncorrect password entered!";
	}
	system("cls");
	cout << "1. Approve/reject political candidate ticket.";
	cout << "\n2. exit";

	int c;
	bool foo = true;
	cout << "\nEnter a valid input: ";
	cin >> c;
	while (foo)
	{
		if (c == 1)
		{
			foo = false;
			break;
		}
		else if (c == 2)
			return;
		else
		{
			cout << "Please enter a valid input: ";
			cin >> c;
		}
	}
	p.approve_politcal_candidate(pname);
}
void commission_portal(d_constituency constituency, i_candidate i, p_candidate p, voter v)
{
	int choice;
	system("cls");
	cout << "1.Register a national assemble constituency (e.g. NA-1, Peshawar – 1)"
		<< "\n2.List all constituencies"
		<< "\n3.Update/Delete Constituencies"
		<< "\n4.Register Political Party"
		<< "\n5.Register Candidate"
		<< "\n6.Register Voter"
		<< "\n7.Show Election Results"
		<< "\n8. Return to main menu" << endl;
	cout << "Enter a valid input: ";
	cin >> choice;

	switch (choice)
	{
	case 1:
		constituency.add_constituency();
		break;
	case 2:
		constituency.display();
		break;
	case 3:
		constituency.delete_constituency(); //Check for any registered candidates*
		break;
	case 4:
		constituency.add_political_party();
		break;
	case 5:
		int _p;
		while (true) {

			system("cls");
			cout << "1. Independent Candidate";
			cout << "\n2. Party Candidate";
			cout << "\nEnter a valid preference: ";
			cin >> _p;
			cin.ignore();
			if ((_p == 1) || (_p == 2))
				break;
		}

		if (_p == 1)
			i.addcandidate();
		if (_p == 2)
			p.addcandidate();
		break;
	case 6:
		v.register_voter(constituency);
		v.clear_buffer();
		v.file_to_buffer();
		break;
	case 7:
		v.display_results();
		break;
	case 8:
		break;
	}
}
void voter_log(d_constituency d1, i_candidate i, p_candidate p, voter v)
{
	system("cls");
	cout << "***Welcome to voter's portal!***" << endl;
	cout << "1. Login" << endl;
	cout << "2. Go back to main menu." << endl;

	int choice = 0;
	while (true)
	{
		cin >> choice;
		if (choice == 1 || choice == 2)
			break;
		cout << "Enter a valid choice: ";
	} // verify choice;

	if (choice == 2)
		return;

	string login_credential;
	cin.ignore();
	cout << "Enter your CNIC number: ";
	getline(cin, login_credential);
	while (v.verify_CNIC(login_credential))
	{
		cout << "Wrong CNIC entered!" << endl;
		cout << "Please re-enter your CNIC: ";
		getline(cin, login_credential);
	}
	system("pause");
	string verified_cred, text, ID;
	ID = login_credential;
	ifstream voter_login;
	voter_login.open("voter_login.txt");
	while (!voter_login.eof())
	{
		getline(voter_login, text);
		if (text == login_credential)
		{
			getline(voter_login, text);
			break;
		}
	}
	voter_login.close();
	cout << "Enter your password: ";
	getline(cin, login_credential);
	while (true)
	{
		if (login_credential == text)
		break;
		cout << "Invalid password entered! Please re-enter your password: ";
		getline(cin, login_credential);
	}
	
	cout << "Login successful!";
	voter_portal(d1, i, p, v, ID, login_credential);
	return;
}
void voter_portal(d_constituency d1, i_candidate i, p_candidate p, voter v, string CNIC, string password)
{
	string status;
	status = v.find_voter_status(CNIC);
	if ( status == "Vote_Pending")
	{
		cout << endl;
		cout << endl;
		p.buffer_sort();
		p.buffer_sort();
		p.display_pcandidate();
		cout << "*********************************";
		cout << endl;
		i.buffer_sort();
		i.buffer_sort();
		i.display_icandidate();
		
		vote_candidate(i, p, v, CNIC, password);
	}
	else
	{
		cout << "\nYou have already voted for: " << status << endl;
		system("pause");
	}
}
void vote_candidate(i_candidate i, p_candidate p, voter v, string CNIC, string password)
{
	int choice;
	cout << "Please make sure to vote for candidate in your constituency only!" << endl;
	cout << "1. Independent candidate." << endl;
	cout << "2. Political Party candidate." << endl;
	cin >> choice;
	while (true)
	{
		if (choice == 1 || choice == 2)
		{
			break;
		}
		cout << "Enter a valid argument: ";
		cin >> choice;
	}
	cin.ignore();
	string voter_constituency = v.find_voter_constituency(CNIC); //finds the constituency of voter
	cout << voter_constituency;
	system("pause");
	string cnd_ID;
	if (choice == 1)
	{
		cout << "Enter candidate ID you wish to vote for: ";
		getline(cin, cnd_ID);
		//verify the candidate ID here
		while (i.find_ID(cnd_ID))
		{
			cout << "Wrong candidate ID entered. Please re-enter ID: ";
			getline(cin, cnd_ID);
		}
		bool newval;
		newval = i.candidate_constituency(cnd_ID, voter_constituency); //finds whether the candidate is registerd in the same constituency as the voter
		if (!newval)
		{
			i.cast_vote(cnd_ID);
			string status = cnd_ID.append("-Independent"); // update status
			v.update_status(CNIC, status);
		}
		else
			cout << "Candidate is not from your constituency!";
		system("pause");
	}
	if (choice == 2)
	{
		cout << "Enter candidate ID you wish to vote for: ";
		getline(cin, cnd_ID);
		while (p.find_ID(cnd_ID))
		{
			cout << "Wrong candidate ID entered. Please re-enter ID: ";
			getline(cin, cnd_ID);
		}
		string party = p.return_party(cnd_ID);
		bool newval;
		p.clear_buffer();
		p.file_to_buffer();
		
		newval = p.candidate_constituency(cnd_ID, voter_constituency);
		if (!newval)
		{
			p.cast_vote(cnd_ID);
			string status = cnd_ID.append(" -" + party); // update status
			v.update_status(CNIC, status);
		}
		else
			cout << "Candidate is not from your constituency!";
		system("pause");
	}
}