#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <sstream>
#include <ctime>
#include <chrono>
#include <map>
using namespace std;

template<typename T>
class Vec;

class Name;
class EmailAccount;
class Message;
class BaseFolder;
class Inbox;
class Outbox;
class Drafts;
class ISP;

//////////////////////////////////////////////////// TEMPLATED VEC CLASS //////////////////////////////////////////////////////////////

template<typename T>
class Vec {
public:
	Vec();
	Vec(int n);
	Vec(int n, const T &a);
	
	Vec(const Vec &orig);
	Vec& operator= (const Vec &rhs);
	~Vec();
	
	int capacity() const;
	int size() const;
	
	T front() const;
	T back() const;
	
	void clear();
	void pop_back();
	void push_back(const T &a);
	void erase(int n);
	
	T& at(int n);	
	T& operator[] (int n);
	const T& operator[] (int n) const;
	
private:
	void allocate();
	void release();
	
	int _capacity;
	int _size;
	T * _vec;
};

//////////////////////////////////////////////// VEC METHODS

	//// Constructors
template<typename T>
Vec<T>::Vec() : _capacity(0), _size(0), _vec(NULL) {}

template<typename T>
Vec<T>::Vec(int n) {
	if(n <= 0) {
		_capacity = 0;
		_size = 0;
		_vec = NULL;
	}
	else {
		_capacity = n;
		_size = n;
		allocate();
	}
}

template<typename T>
Vec<T>::Vec(int n, const T &a) {
	if(n <= 0) {
		_capacity = 0;
		_size = 0;
		_vec = NULL;
	}
	else {
		_capacity = n;
		_size = n;
		allocate();
		for(int i = 0; i < _capacity; ++i) _vec[i] = a;
	}
}

	//// Copy, Assign, Destroy
template<typename T>
Vec<T>::Vec(const Vec<T> &orig) {
	_capacity = orig._size;
	_size = orig._size;
	allocate();
	for(int i = 0; i < _size; ++i) _vec[i] = orig._vec[i];
}
	
template<typename T>
Vec<T>& Vec<T>::operator= (const Vec<T> &rhs) {
	if (this == &rhs) return *this;
	_capacity = rhs._size;
	_size = rhs._size;
	release();
	allocate();
	for(int i = 0; i < _size; ++i) _vec[i] = rhs._vec[i];
	return *this;
}

template<typename T>
Vec<T>::~Vec() {release();}

	//// vector methods, operator[]
template<typename T>
int Vec<T>::capacity() const {return _capacity;}

template<typename T>
int Vec<T>::size() const {return _size;}

template<typename T>	
T Vec<T>::front() const {
	if(_size <= 0) {
		T default_obj;
		return default_obj;
	}
	return _vec[0];
}

template<typename T>
T Vec<T>::back() const {
	if(_size <= 0) {
		T default_obj;
		return default_obj;
	}
	return _vec[_size - 1];
}

template<typename T>
void Vec<T>::clear() {
	_size = 0;
}
	
template<typename T>	
void Vec<T>::pop_back() {
	if(_size > 0) --_size;
}

template<typename T>
void Vec<T>::push_back(const T &a) {
	if(_size < _capacity) {
		_vec[_size] = a;
		++_size;
	}
	else {
		if(_capacity == 0) _capacity = 1;
		else _capacity *= 2;
		T *oldvec = _vec;
		allocate();
		if(oldvec != NULL) for(int i = 0; i < _size; ++i) _vec[i] = oldvec[i];
		delete [] oldvec;
		_vec[_size] = a;
		++_size;
	}
}

	//////////// P3.1 - erase
template<typename T>
void Vec<T>::erase(int n) {
	if(n >= 0 && n < _size) {
		for(int i = n; i < (_size - 1); ++i) _vec[i] = _vec[i+1];
		_size -= 1;
	}
}

template<typename T>
T& Vec<T>::at(int n) {
	if(n >= 0 && n < _size) return _vec[n];
	else {
		T *pnull = NULL;
		return *pnull;
	}
}

template<typename T>
T& Vec<T>::operator[] (int n) {
	return at(n);
}

template<typename T>
const T& Vec<T>::operator[] (int n) const {
	if(n >= 0 && n < _size) return _vec[n];
	else {
		T *pnull = NULL;
		return *pnull;
	}
}

	//// allocate and release
template<typename T>
void Vec<T>::release() {
	if (_vec != NULL) delete [] _vec;
	_vec = NULL;
}

template<typename T>
void Vec<T>::allocate() {
	if (_capacity > 0) _vec = new T[_capacity];
	else _vec = NULL;
}

//////////////////////////////////////////////////// NAME CLASS ///////////////////////////////////////////////////////////////

class Name {
public:
	Name() {}
	Name(string s);
	
	void set(string s);
	
	string name() const;
	string address() const;
	
private:
	string _name; // "short name" that is displayed in a message prompt, the Bob in Bob@qc.cuny.edu
	string _address; // full email address.
};

////////////////////////////////////////////////////////////// EMAIL ACCOUNT CLASS /////////////////////////////////////////////////

class EmailAccount {
public:
	EmailAccount(string s);
	~EmailAccount();
	
	const Name& owner() const; // accessor
	Drafts& drafts(); // accessor/mutator
	BaseFolder& in(); // accessor/mutator
	BaseFolder& out(); // accessor/mutator
	void send(Message *m);
	void receive(Message *m);
	void insert(Message *m);
	
private:
	Name _owner;
	Drafts * _drafts;
	BaseFolder * _in;
	BaseFolder * _out;
	
	// nothing good ever came from copying.
	EmailAccount(const EmailAccount &orig);
	EmailAccount& operator=(const EmailAccount &rhs);
};

///////////////////////////////////////////////////////// MESSAGE CLASS //////////////////////////////////////////

class Message {
public:
	Message(string f);
	Message(string f, string t);
	Message(string f, string t, string s);
	Message(string f, string t, string s, string txt);
	
	// copy constructor
	Message(const Message &orig);

	const Name& from() const;
	const Name& to() const;
	string subject() const;
	string text() const;
	
	string date() const;
	
	void send();
	
	void setRecipient(string t);
	void setSubject(string s);
	void setText(string txt);
	
	void prependText(string t);
	void appendText(string t);
	
	void print() const;

private:
	void setDate();
	
	Name _from; // use "Name" class
	Name _to; // use "Name" class
	string _subject;
	string _text;
	time_t _date; // see below
};

/////////////////////////////////////////////////////////////////////// BASE FOLDER CLASS ///////////////////////////////////////

class BaseFolder {
public:	
	virtual ~BaseFolder();
	
	int size() const;
	void display() const;
	void erase(int n);
	void forward(int n) const;
	void print(int n) const;
	void receive(const Message *m);
	void reply(int n) const;
	
protected:
	// Constructor
	BaseFolder(EmailAccount *ac);
	
	virtual string type() const;
	virtual const Name& tofrom(const Message *m) const;
	
	// data
	Vec<const Message*> _msg;
	EmailAccount *_ac;

	// disable copies.
private:
	BaseFolder(const BaseFolder &orig);
	BaseFolder& operator=(const BaseFolder &rhs);
};

////////////////////////////////////////////////////// INBOX CLASS //////////////////////////////////////////////////////////

class Inbox : public BaseFolder {
public:
	Inbox(EmailAccount *ac);
	
protected:
	virtual string type() const;
	virtual const Name& tofrom(const Message *m) const;
};

////////////////////////////////////////////////////// OUTBOX CLASS ///////////////////////////////////////////////////////

class Outbox : public BaseFolder {
public:
	Outbox(EmailAccount *ac);
	
protected:
	virtual string type() const;
	virtual const Name& tofrom(const Message *m) const;
};

//////////////////////////////////////////////////// DRAFTS CLASS ///////////////////////////////////////////////////////

class Drafts {
public:
	Drafts(EmailAccount *ac);
	~Drafts();
	
	void display() const;
	void send(int n);
	void erase(int n);
	
	Message* addDraft();
	Message* addDraft(Message *m);
	Message* getDraft(int n);
	Message* operator[](int n);
	
private:
	int newKey();
	
	// data
	EmailAccount *_ac;
	map<int, Message*> _drafts;
	int _newKey; // generates keys to index draft messages.
	
	// who needs copies anyway.
	Drafts(const Drafts &orig);
	Drafts& operator=(const Drafts &rhs);
};

/////////////////////////////////////////////////////////// ISP CLASS ////////////////////////////////////////////////////////////////

class ISP {
public:
	static void addAccount(EmailAccount *e);
	static void send(Message *m);
	
private:
	ISP() {}
	static map<Name, EmailAccount*> _accounts;
};

/////////////////////////////////////////////////// ALL METHODS EXCEPT VEC ////////////////////////////////////////////////////////////////
////////////////////////////////////////////// NAME METHODS

Name::Name(string s) {set(s);}

void Name::set(string s) { // mutator
	const string _domain = "@qc.cuny.edu";
	istringstream iss(s);
	iss >> _name;
	if(_name != "") {
		_address = _name + _domain;
		for(int i = 0; i < _address.length(); ++i) _address[i] = tolower(_address[i]);
	}
	else _address = "";
}

string Name::name() const {return _name;} // accessor
string Name::address() const {return _address;} // accessor

	//// Q3
bool operator==(const Name &n1, const Name &n2) {
	return (n1.address() == n2.address());
}

bool operator<(const Name &n1, const Name &n2) {
	return (n1.address() < n2.address());
}

////////////////////////////////////////////////// EMAIL ACCOUNT METHODS

EmailAccount::EmailAccount(string s) {
	_owner.set(s);
	_drafts = new Drafts(this);
	_in = new Inbox(this);
	_out = new Outbox(this);
	ISP::addAccount(this);
}

EmailAccount::~EmailAccount() {
	delete _drafts;
	delete _in;
	delete _out;
}

const Name& EmailAccount::owner() const {return _owner;}
Drafts& EmailAccount::drafts() {return *_drafts;}
BaseFolder& EmailAccount::in() {return *_in;}
BaseFolder& EmailAccount::out() {return *_out;}

void EmailAccount::send(Message *m) {
	_out->receive(m);
	ISP::send(m);
}

void EmailAccount::receive(Message *m) {_in->receive(m);}

void EmailAccount::insert(Message *m) {_drafts->addDraft(m);}

EmailAccount::EmailAccount(const EmailAccount &orig) {}
EmailAccount& EmailAccount::operator=(const EmailAccount &rhs) {return *this;}

/////////////////////////////////////////////// MESSAGE METHODS

Message::Message(string f) : _date(0) {_from.set(f);}
Message::Message(string f, string t) : _date(0) {_from.set(f); _to.set(t);}
Message::Message(string f, string t, string s) : _date(0) {_from.set(f); _to.set(t); _subject = s;}
Message::Message(string f, string t, string s, string txt) : _date(0) {_from.set(f); _to.set(t); _subject = s; _text = txt;}

Message::Message(const Message &orig) {
	_from = orig._from;
	_to = orig._to;
	_subject = orig._subject;
	_text = orig._text;
	_date = orig._date;
}

const Name& Message::from() const {return _from;}
const Name& Message::to() const {return _to;}
string Message::subject() const {return _subject;}
string Message::text() const {return _text;}
	
string Message::date() const { // see below
	if(_date > 0) return ctime(&_date);
	else return "";
}
	
void Message::send() {setDate();} // see below
	
void Message::setRecipient(string t) {_to.set(t);}
void Message::setSubject(string s) {_subject = s;}
void Message::setText(string txt) {_text = txt;}
	
void Message::prependText(string t) {_text = (t + " " + _text);} // see Message1 class
void Message::appendText(string t) {_text = (_text + " " + t);} // see Message1 class
	
void Message::print() const { // see Message1 class
	cout << "From: " << _from.name() << " <" << _from.address() << ">" << endl;
	cout << "To: " << _to.name() << " <" << _to.address() << ">" << endl;
	cout << "Subject: " << _subject << endl;
	cout << "Date: " << date() << endl;
	cout << _text << endl;
	cout << endl;	
}

void Message::setDate() { // see below
	auto t_now = chrono::system_clock::now();
	_date = chrono::system_clock::to_time_t(t_now);
}

/////////////////////////////////////////////// BASE FOLDER METHODS

BaseFolder::~BaseFolder() {
	for(int i = 1; i < _msg.size(); ++i) delete _msg[i];
}

int BaseFolder::size() const {return _msg.size();}

void BaseFolder::display() const {
	cout << _ac->owner().name() << " " << type() << endl;
	if(_msg.size() <= 1) cout << "No messages to display." << endl;
	else for(int i = 1; i < _msg.size(); ++i) {
		const Name &tmp = tofrom(_msg[i]);
		cout << "Message #" << i << endl;
		cout << tmp.name() << endl;
		cout << "Subject: " << _msg[i]->subject() << endl;
	}
}

void BaseFolder::erase(int n) {
	if(n >= 1 && n < _msg.size()) {
		delete _msg[n];
		_msg.erase(n);
	}
}

void BaseFolder::forward(int n) const {
	if(n >= 1 && n < _msg.size()) {
		const Message *m = _msg[n];
		string fwd_subject = "Fwd: " + m->subject();
		Message *ptr = new Message(_ac->owner().name(), "", fwd_subject, m->text());
		_ac->insert(ptr);
	}	
}

void BaseFolder::print(int n) const {if(n >= 1 && n < _msg.size()) _msg[n]->print();}

void BaseFolder::receive(const Message *m) {_msg.push_back(m);}

void BaseFolder::reply(int n) const {
	if(n >= 1 && n < _msg.size()) {
		const Message *m = _msg[n];
		string subject = "Re: " + m->subject();
		const Name &tmp = tofrom(m);
		Message *ptr = new Message(_ac->owner().name(), tmp.name(), subject, m->text());
		_ac->insert(ptr);
	}
}

string BaseFolder::type() const {} // pure virtual if desired
const Name& BaseFolder::tofrom(const Message *m) const {} // pure virtual if desired

	// Constructor
BaseFolder::BaseFolder(EmailAccount *ac) : _ac(ac) {_msg.push_back(NULL);}

BaseFolder::BaseFolder(const BaseFolder &orig) {}

BaseFolder& BaseFolder::operator=(const BaseFolder &rhs) {return *this;}

///////////////////////////////////////////// INBOX METHODS

Inbox::Inbox(EmailAccount *ac) : BaseFolder(ac) {} // public constructor

string Inbox::type() const {return "Inbox: ";}

const Name& Inbox::tofrom(const Message *m) const {return m->from();}

//////////////////////////////////////////// OUTBOX METHODS

Outbox::Outbox(EmailAccount *ac) : BaseFolder(ac) {} // public constructor

string Outbox::type() const {return "Outbox: ";}

const Name& Outbox::tofrom(const Message *m) const {return m->to();}

////////////////////////////////////////// DRAFTS METHODS

Drafts::Drafts(EmailAccount *ac) : _ac(ac), _newKey(0) {_drafts[0] = NULL;}

Drafts::~Drafts() {
	map<int, Message*>::iterator mit;
	for(mit = _drafts.begin(); mit != _drafts.end(); ++mit) {
		Message *ptr = mit->second;
		delete ptr;
	}
}

void Drafts::display() const {
	cout << _ac->owner().name() << " Drafts: " << endl;
	if(_drafts.size() <= 1) cout << "No messages to display." << endl;
	else {
		map<int, Message*>::const_iterator mit;
		for(mit = _drafts.begin(); mit != _drafts.end(); ++mit) {
			if(mit->first == 0) continue;
			const Message *ptr = mit->second;
			cout << "Draft # " << mit->first << endl; // print key value.
			cout << ptr->to().name() << endl;
			cout << "Subject: " << ptr->subject() << endl;
		}
	}
}

void Drafts::send(int n) {
	Message *ptr = NULL;
	if(_drafts.count(n) > 0) ptr = _drafts[n];
	if(ptr == NULL) cout << "Message not found!" << endl;
	else {
		if(ptr->to().address() == "") cout << "You must specify a recipient." << endl;
		else {
			ptr->send();
			_drafts.erase(n);
			_ac->send(ptr);
		}
	}
}

void Drafts::erase(int n) {
	delete _drafts[n];
	_drafts.erase(n); // calls map erase method.
}

Message* Drafts::addDraft() {
	int key = newKey();
	Message *ptr = new Message(_ac->owner().name());
	_drafts[key] = ptr;
	return ptr;
}

Message* Drafts::addDraft(Message *m) {
	int key = newKey();
	_drafts[key] = m;
	return m;
}

Message* Drafts::getDraft(int n) {
	if(_drafts.count(n) > 0) return _drafts[n];
	else return NULL;
}

Message* Drafts::operator[](int n) {
	if(_drafts.count(n) > 0) return _drafts[n];
	else return NULL;
}

int Drafts::newKey() {return ++_newKey;}

Drafts::Drafts(const Drafts &orig) {}
Drafts& Drafts::operator=(const Drafts &rhs) {return *this;}

///////////////////////////////////////////// ISP METHODS
map<Name, EmailAccount*> ISP::_accounts; // initializing static data member _accounts.

void ISP::addAccount(EmailAccount *e) {

  // DEBUG
  cerr << "ISP add a/c = " << e->owner().name() << "  " << e->owner().address() << endl;
  cerr << "ISP map size before = " << _accounts.size() << endl;

  _accounts[e->owner()] = e;
  
  // DEBUG
  cerr << "ISP map size after = " << _accounts.size() << endl;
  for (auto i : ISP::_accounts) {
    cerr << "ISP loop = " << i.first.name() << "  " << i.second->owner().name() << endl;
  }
  
}

void ISP::send(Message *m) {
	EmailAccount *ac = NULL;
	if(_accounts.count(m->to()) > 0) ac = _accounts[m->to()];
	if(ac == NULL) cout << "Delivery failed, to recipient: " << m->to().name() << endl;
	else {
		Message *clone = new Message(*m);
		ac->receive(clone);
	}
}

///////////////////////////////////////////////// MAIN TEST /////////////////////////////////////////////////////////

int main() {
	// create 2 email accounts.
	EmailAccount Bobbo("Bobbo");
	EmailAccount Alice("Alice");
	
	// create 2 draft messages in Bobbo's Drafts folder.
	Message *m1 = Bobbo.drafts().addDraft();
	Message *m2 = Bobbo.drafts().addDraft();
	
	// print content of Message m1.
	m1->print();
	
	// modify Message m2.
	m2->setRecipient("Alice");
	m2->setSubject("Project Test");
	m2->setText("Hope this works.");
	
	// print content of Message m2.
	m2->print();
	
	// try to send non-existent Message m3.
	Bobbo.drafts().send(3);
	cout << endl;
	
	// list all drafts in Bobbo's Drafts folder.
	cout << endl << "---Bobbo Drafts Check---" << endl;
	Bobbo.drafts().display();

	// try to send Message m1 to Sateesh.
	m1->setRecipient("Sateesh");
	Bobbo.drafts().send(1);
	
	// send Message m2 to Alice.
	Bobbo.drafts().send(2);

	// check Bobbo's Drafts, Inbox, and Outbox.
	cout << endl << "---Bobbo Mailbox Check #1---" << endl;
	Bobbo.drafts().display();
	Bobbo.in().display();
	Bobbo.out().display();
	
	//// Alice's Turn.
	// check Alice's Drafts, Inbox, and Outbox.
	cout << endl << "---Alice Mailbox Check #1---" << endl;
	Alice.drafts().display();
	Alice.in().display();
	Alice.out().display();	
	
	// check number of emails and print out Message m2 in Alice's Inbox.
	cout << endl << "---Alice Mailbox Check #2---" << endl;
	cout << Alice.in().size() << endl;
	Alice.in().print(1);
	
	// reply and forward Message m2 twice for each (2 FW and 2 RE).
	Alice.in().forward(1);
	Alice.in().forward(1);
	Alice.in().reply(1);
	Alice.in().reply(1);
	
	// create new draft in Alice's Drafts folder.
	Alice.drafts().addDraft();
	
	// check Alice's Drafts, Inbox, and Outbox.
	cout << endl << "---Alice Mailbox Check #3---" << endl;
	Alice.drafts().display();
	Alice.in().display();
	Alice.out().display();		
	
	// modify FW email #2 and try to send them to Sateesh.
	Alice.drafts().getDraft(2)->setRecipient("Sateesh");
	Alice.drafts().send(2);
	
	// try to send RE email #2 to self (Alice).
	Alice.drafts()[4]->setRecipient("Alice");
	Alice.drafts().send(4);
	Alice.in().display();
	
	// modify everything in 1st draft (FW email #1) and send back to Bobbo.
	Alice.drafts()[1]->setRecipient("Bobbo");
	Alice.drafts().getDraft(1)->setSubject("Project Test... Worked.");
	Alice.drafts().getDraft(1)->appendText("Oh it does :)!");
	Alice.drafts().getDraft(1)->prependText("It did work!");
	Alice.drafts().send(1);	
	
	// delete all emails in Alice's email account.
	cout << endl << "---Alice's Emails Pre-Deletion---" << endl;	
	Alice.drafts().display();
	Alice.in().display();
	Alice.out().display();	
	
	Alice.drafts().erase(3);
	Alice.drafts().erase(5);
	Alice.in().erase(1);
	Alice.in().erase(2);
	Alice.in().erase(3);
	Alice.out().erase(1);
	Alice.out().erase(2);
	Alice.out().erase(3);

	cout << endl << "---Alice's Emails Post-Deletion---" << endl;	
	Alice.drafts().display();
	Alice.in().display();
	Alice.out().display();	
	
	//// back to Bobbo.
	cout << endl << "---Bobbo Mailbox Check #2---" << endl;
	Bobbo.drafts().display();
	Bobbo.in().display();
	Bobbo.out().display();
	
	cout << endl;
	Bobbo.in().print(1);
	
	return 0;
}
