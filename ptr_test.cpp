#include "SharedPtr.hpp"
#include <new>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <errno.h>
#include <assert.h>



using namespace std;
using namespace cs540;



class Random {
    public:
        Random(unsigned int s = 0);
        // Generate random int in range [l,h].
        int operator()(int l, int h) {
            return std::uniform_int_distribution<int>(l, h)(gen);
        }
        Random(const Random &) = delete;
        Random &operator=(const Random &) = delete;
    private:
        std::default_random_engine gen;
};

Random::Random(unsigned int seed) : gen(seed) {}



void basic_tests_1();
void basic_tests_2();
// RunSecs needs to be here so that it can be set via command-line arg.
int RunSecs = 15;
void threaded_test();
size_t AllocatedSpace;



void
usage() {
    fprintf(stderr, "Bad args, usage: ./a.out [ -t secs ]\n");
    exit(1);
}



int
main(int argc, char *argv[]) {

    int c;

    setlinebuf(stdout);

    {
        // Force initial iostreams allocation so that memory-leak detecting
        // will work right.  Tabs and pointer are to get locale stuff
        // allocated.
        int *p = (int *) 0x1234;
        cout << "\tForce initial allocation on cout: " << p << endl;
        cerr << "\tForce initial allocation on cerr: " << p << endl;
        clog << "\tForce initial allocation on clog: " << p << endl;
    }

    while ((c = getopt(argc, argv, "t:")) != -1) {
        switch (c) {
            case 't':
                RunSecs = atoi(optarg);
		assert(1 <= RunSecs && RunSecs <= 10000);
                break;
            case '?':
                usage();
                break;
            default:
                assert(false);
                abort();
        }
    }

    // Catch any command-line args without -.
    if (optind < argc) {
        usage();
    }

    basic_tests_1();
//    basic_tests_2();
//    threaded_test();
}

void *operator new(size_t sz) {
    char *p = (char *) malloc(sz + 8);
    *((size_t *) p) = sz;
    __sync_add_and_fetch(&AllocatedSpace, sz);
    return p + 8;
}


void operator delete(void *vp) noexcept {

    if (vp == 0) {
        return;
    }

    char *p = (char *) vp;
    size_t sz = *((size_t *) (p - 8));
    __sync_sub_and_fetch(&AllocatedSpace, sz);
    // Zero out memory to help catch bugs.
    memset(p - 8, 0xff, sz + 8);
    free(p - 8);
}


/* Basic Tests 1 ================================================================================ */

class Base1 {
public:
//    protected:
        Base1() : derived_destructor_called(false) {
            printf("Base1::Base1()\n");
        }
//    private:
        Base1(const Base1 &); // Disallow.
        Base1 &operator=(const Base1 &); // Disallow.
//    protected:
        virtual ~Base1() {
            printf("Base1::~Base1()\n");
            assert(derived_destructor_called);
        }
//    protected:
        bool derived_destructor_called;
};

class Derived : public Base1 {
        friend void basic_tests_1();
    private:
        Derived() {}
        Derived(const Derived &); // Disallow.
        Derived &operator=(const Derived &); // Disallow.
    public:
        ~Derived() {
            printf("Derived::~Derived()\n");
            derived_destructor_called = true;
        }
        int value;
};

class Base_polymorphic {
    protected:
        Base_polymorphic() {
            printf("Base_polymorphic::Base_polymorphic()\n");
        }
    private:
        Base_polymorphic(const Base_polymorphic &); // Disallow.
        Base_polymorphic &operator=(const Base_polymorphic &); // Disallow.
    protected:
        virtual ~Base_polymorphic() {
            printf("Base_polymorphic::~Base_polymorphic()\n");
        }
};

class Derived_polymorphic : public Base_polymorphic {
        friend void basic_tests_1();
    private:
        Derived_polymorphic() {}
        Derived_polymorphic(const Derived_polymorphic &); // Disallow.
        Derived_polymorphic &operator=(const Derived_polymorphic &); // Disallow.
};

class Derived2_polymorphic : public Base_polymorphic {
    private:
        Derived2_polymorphic() {}
        Derived2_polymorphic(const Derived2_polymorphic &); // Disallow.
        Derived2_polymorphic &operator=(const Derived2_polymorphic &); // Disallow.
};

class Base2 {
    protected:
        Base2() : derived_destructor_called(false) {
            printf("Base2::Base2()\n");
        }
    private:
        Base2(const Base2 &); // Disallow.
        Base2 &operator=(const Base2 &); // Disallow.
    protected:
        ~Base2() {
            printf("Base2::~Base2()\n");
            assert(derived_destructor_called);
        }
    protected:
        bool derived_destructor_called;
};

class Derived_mi : public Base1, public Base2 {
        friend void basic_tests_1();
    private:
        Derived_mi() {}
        Derived_mi(const Derived_mi &); // Disallow.
        Derived_mi &operator=(const Derived_mi &); // Disallow.
    public:
        ~Derived_mi() {
            printf("Derived_mi::~Derived_mi()\n");
            Base1::derived_destructor_called = true;
            Base2::derived_destructor_called = true;
        }
        int value;
};

class Base1_vi {
    protected:
        Base1_vi() : derived_destructor_called(false) {
            printf("Base1_vi::Base1_vi()\n");
        }
    private:
        Base1_vi(const Base1_vi &); // Disallow.
        Base1_vi &operator=(const Base1_vi &); // Disallow.
    protected:
        ~Base1_vi() {
            printf("Base1_vi::~Base1_vi()\n");
            assert(derived_destructor_called);
        }
    protected:
        bool derived_destructor_called;
};

class Base2_vi : public virtual Base1_vi {
    protected:
        Base2_vi() {
            printf("Base2_vi::Base2_vi()\n");
        }
    private:
        Base2_vi(const Base2_vi &); // Disallow.
        Base2_vi &operator=(const Base2_vi &); // Disallow.
    protected:
        ~Base2_vi() {
            printf("Base2_vi::~Base2_vi()\n");
            assert(derived_destructor_called);
        }
};

class Derived_vi : public virtual Base1_vi, public Base2_vi {
        friend void basic_tests_1();
    private:
        Derived_vi() {}
        Derived_vi(const Derived_vi &); // Disallow.
        Derived_vi &operator=(const Derived_vi &); // Disallow.
    public:
        ~Derived_vi() {
            printf("Derived_vi::~Derived_vi()\n");
            derived_destructor_called = true;
        }
        int value;
};

void basic_tests_1() {

    size_t base = AllocatedSpace;
/*
    // Test deleting through original class.----doesn't work failed to convert derived to base
    {
        // Base1 created directly with Derived *.
        {
            SharedPtr<Base1> sp(new Derived);
            {
                // Test copy constructor.
                SharedPtr<Base1> sp2(sp);
            }
        }
        // Base1 assigned from SharedPtr<Derived>.
        {
            SharedPtr<Base1> sp2;
            {
                SharedPtr<Derived> sp(new Derived);
                // Test template copy constructor.
                SharedPtr<Base1> sp3(sp);
                sp2 = sp;
                sp2 = sp2;
            }
        }
    }*/
	 // Test copy constructor and template copy constructor.
    {
        {
            SharedPtr<Derived> sp(new Derived);
            SharedPtr<Derived> sp2(sp);
        }
        {
            SharedPtr<Derived> sp(new Derived);
            SharedPtr<Base1> sp2(sp);
        }
    }

    // Test assignment operator and template assignment operator.
    {
        {
            SharedPtr<Derived> sp(new Derived);
            SharedPtr<Derived> sp2;
            sp2 = sp;
            sp = sp; // Self assignment.
        }
        {
            SharedPtr<Derived> sp(new Derived);
//            SharedPtr<Base1> sp2;
 //           sp2 = sp;
 //           sp2 = sp2; // Self assignment.
 //           sp2 = sp;
  //          sp = sp;
        }
    }
    printf("Basic tests 1 passed.\n");
}
