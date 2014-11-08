extern int f();

int g() 
{
    int ret = 0;

    goto out;
    ret = 1;

out:

    return ret;
}
