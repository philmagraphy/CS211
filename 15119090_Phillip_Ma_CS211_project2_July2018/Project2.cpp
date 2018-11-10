#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <sstream>
using namespace std;

class Message1 {
public:
// 1.7 - Constructors
	Message1() {_date = 0;} // mostly so we can instantiate arrays.
	Message1(string f) {_date = 0; _from = addDomain(f);} // sender only.
	Message1(string f, string t) {_date = 0; _from = addDomain(f); _to = addDomain(t);} // sender and recipient only.
	Message1(string f, string t, string s) {_date = 0; _from = addDomain(f); _to = addDomain(t); _subject = s;} // sender, recipient, and subject.
	Message1(string f, string t, string s, string txt) {_date = 0; _from = addDomain(f); _to = addDomain(t); _subject = s; _text = txt;} // sender, recipient, subject, and text.

// 1.2 - Accessors
	string from() const {return _from;}
	string to() const {return _to;}
	string subject() const {return _subject;}
	string text() const {return _text;}
	string date() const {return string("n/a");}

// 1.4 - Mutators
	void setSubject(string s) {_subject = s;}
	void setText(string txt) {_text = txt;}
	void prependText(string t) {_text = (t + " " + _text);}
	void appendText(string t) {_text = (_text + " " + t);}
	void setRecipient(string t) {_to = addDomain(t);}

// 1.3 - Print
	void print() const {
		cout << "From: " << _from << endl;
		cout << "To: " << _to << endl;
		cout << "Subject: " << _subject << endl;
		cout << "Date: " << date() << endl;
		cout << _text << endl;
		cout << endl;
	}

private:
	string _from;
	string _to;
	string _subject;
	string _text;
	int _date;

// 1.6 - Append Domain to cleaned Recipient Name
	string addDomain(string s) {
		const string _domain = "@qc.cuny.edu";
		istringstream iss(s);
		iss >> s;
		return (s + _domain);
	}	
};

// 1.11 - operator<<
ostream& operator<< (ostream& os, const Message1 &m) {
	os << "From: " << m.from() << endl;
	os << "To: " << m.to() << endl;
	os << "Subject: " << m.subject() << endl;
	os << "Date: " << m.date() << endl;
	os << m.text() << endl;
	os << endl;
	return os;
}

class Vec_Message1 {
public:
// 2.3 - Constructors, Copy, Assign, Destroy
	Vec_Message1() : _capacity(0), _size(0), _vec(NULL) {}
	Vec_Message1(int n) {
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
	Vec_Message1(int n, const Message1 &a) {
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
	// Copy	
	Vec_Message1(const Vec_Message1 &orig) {
		_capacity = orig._size;
		_size = orig._size;
		allocate();
		for(int i = 0; i < _size; ++i) _vec[i] = orig._vec[i];
	}
	// Assign
	Vec_Message1& operator= (const Vec_Message1 &rhs) {
		if(this == &rhs) return *this;
		_capacity = rhs._size;
		_size = rhs._size;
		release();
		allocate();
		for(int i = 0; i < _size; ++i) _vec[i] = rhs._vec[i];
		return *this;
	}
	// Destroy	
	~Vec_Message1() {release();}

// capacity and size
	int capacity() const {return _capacity;} // email storage maximum.
	int size() const {return _size;} // current number of emails.
	
// 2.5 - front and back
	Message1 front() const { // oldest email.
		if(_size > 0) return _vec[0];
		else {
			Message1 default_obj;
			return default_obj;
		}
	}
	Message1 back() const { // newest email.
		if(_size > 0) return _vec[_size - 1];
		else {
			Message1 default_obj;
			return default_obj;
		}
	}

// 2.4 - clear and pop_back
	void clear() {_size = 0;} // delete all emails.
	void pop_back() {if(_size > 0) --_size;} // delete newest email.

// 2.7 - push_back
	void push_back(const Message1 &a) { // "store" email.
		if(_size < _capacity){
			_vec[_size] = a;
			++_size;
		}
		else {
			if(_capacity == 0) _capacity = 1;
			else _capacity *= 2;
			Message1 *oldvec = _vec;
			allocate();
			if(oldvec != NULL) for(int i = 0; i < _size; ++i) _vec[i] = oldvec[i];
			delete [] oldvec;
			_vec[_size] = a;
			++_size;
		}
	}

// 2.8 - at
	Message1& at(int n) { // "look at" specific email.
		if(n >= 0 && n < _size) return _vec[n];
		else {
			Message1 *pnull = NULL;
			return *pnull;
		}
	}

// 2.9 - operator[]
	Message1& operator[] (int n) {return at(n);}
	const Message1& operator[] (int n) const {
		if(n >= 0 && n < _size) return _vec[n];
		else {
			Message1 *pnull = NULL;
			return *pnull;
		}
	}

private:
	int _capacity;
	int _size;
	Message1 * _vec;
	
// 2.2 - allocate and release
	void allocate() {
		if(_capacity <= 0) _vec = NULL;
		if(_capacity > 0) _vec = new Message1[_capacity];
	}

	void release() {
		if(_vec == NULL) return;
		else delete [] _vec;
		_vec = NULL;
	}
};

// 2.10 - reverse and print
void reverse(Vec_Message1 &v) { // resort emails, newest to oldest and vice versa.
	int n = v.size();
	if(n <= 1) return;
	for(int i = 0; i < (n/2); ++i){
		Message1 tmp = v[i];
		v[i] = v[n-1-i];
		v[n-1-i] = tmp;
	}
}

void print(ostream &os, const Vec_Message1 &v) { // send emails to ostream object.
	for(int i = 0; i < v.size(); ++i) os << v[i] << endl;
}

//// 2.11 - MAIN ////
int main() {
	Message1 a; // empty email
	Message1 b ("JoeSmith"); // sender only.
	Message1 c("PhilMa", "JaneSmith"); // sender, recipient.
	Message1 d("PhilMa", "JohnSmith", "Drunk Deer Dancing Daily"); // sender, recipient, subject.
	Message1 e("PhilMa", "JohnSmith", "Empty Erney Emotionally Exits Evergreen", "Can you believe this guy Erney?"); // sender, recipient, subject, body of email.
	
	cout << e.date() << endl << e.from() << endl << e.to() << endl << e.subject() << endl << e.text() << endl; // test constructor, Message1 e.
	cout << endl;
	
	e.setSubject("E E E E E"); // update subject and body.
	e.setText("1234");
	e.print(); // test Message1 print.	

	e.prependText("ERNEY "); // modify body and recipient.
	e.appendText(" YENRE");
	e.setRecipient("   ErneyErnhardt   "); // test addDomain cleaning.

	e.print(); // test print.
	cout << d; // test operator<<
	
	Message1 f(c); // f is an instantiated copy of c.
	f.print();
	
	a = a = b; // a is assigned values of b.
	a.print();
	
	c = d = c; // d is assigned values of c.
	c.print();
	d.print();

////////
	cout << "TESTING VECTOR" << endl << endl;
	
	Vec_Message1 va; // empty email account.
	Vec_Message1 vb(5); // space for 5 emails allocated.
	Vec_Message1 vc(3, e); // space for 3 emails, all copies of e.
	
	cout << "Capacity: " << vc.capacity() << endl; // test Vec_Message1 class methods.
	cout << "Size: " << vc.size() << endl;
	cout << "Front: " << endl << vc.front() << endl;
	cout << "Back: " << endl << vc.back() << endl;
	vc.pop_back();
	cout << "Size: " << vc.size() << endl << endl;
	
	vc.clear();
	vc.push_back(d); vc.push_back(e);
	cout << "Capacity: " << vc.capacity() << endl;
	cout << "Size: " << vc.size() << endl;
	cout << "Front: " << endl << vc.front() << endl;
	cout << "Back: " << endl << vc.back() << endl;	
	
	reverse(vc); // test reverse and print.
	print(cout, vc);
	
	return 0;
}

