struct A
{
    int x, y;
};

__attribute__((warn_unused_result)) A foo();

void h(const A&);
void m(int);

void test_foo()
{
    foo(); // must be diagnosed

    h(foo()); // do not diagnose

    int z = foo().y; // do not diagnose

    foo().y; // arguable, but if diagnosed OK

    m(foo().y); // do not diagnose
}

struct B
{
    ~B();
};

__attribute__((warn_unused_result)) B bar();

void test_bar()
{
    bar(); // do not diagnose
}

A quux()
{
    return foo();
}
