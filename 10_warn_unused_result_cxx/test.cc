typedef
struct A
{
    int x, y;
} A;

__attribute__((warn_unused_result)) A foo();

void h(const A&);
void m(int);

void test_foo()
{
    foo(); // OK to diagnose

    h(foo()); // ERROR: do not diagnose

    int z = foo().y; // ERROR: do not diagnose

    foo().y; // arguable but OK

    m(foo().y); // ERROR: do not diagnose
}

struct B
{
    ~B();
};

__attribute__((warn_unused_result)) B bar();

void test_bar()
{
    bar(); // ERROR: do not diagnose
}
