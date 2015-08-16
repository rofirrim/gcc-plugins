void fizz();
void buzz();
void fizzbuzz();
void print_number(int n);

void test(int n)
{
    for (int i = 0; i < n; i++)
    {
        bool div_3 = i % 3 == 0;
        bool div_5 = i % 5 == 0;

#define BITMAP2(x, y) ((x) | ((y) << 1))

        switch (BITMAP2(div_3, div_5))
        {
            case BITMAP2(1, 0): // 3
                fizz();
                break;
            case BITMAP2(0, 1): // 5
                buzz();
                break;
            case BITMAP2(1, 1): // 15
                fizzbuzz();
                break;
            default:
                print_number(i);
        }

#undef BITMAP2
    }
}
