#include<iostream>
#include<typeinfo>


using namespace std;


class base{
public:
	int a;
};

class derived:public base{
public:
	int b;

};

void foo( int &a){
//	int b = const_cast<int>(a);
	int b = a;
	b++;
//	a++;
	cout<<a;
}

void foo2(const derived &d){
	base  b1;
	b1 = d;
}

int main(){
	derived *d1;
	base 	*b1;

	cout<<"hello world";
	int b=2;
	b1=d1;
}
