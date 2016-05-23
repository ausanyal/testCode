int fib(int n)  0 1 1 2 3 5
{
    int a = 1, b = 0, c=0;
    
    while(n--)   
    {
        c = a + b;
        //cout<<c<<endl;
        a = b;
        b = c;
    }
   
        cout<<c;
   
}

