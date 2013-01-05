#include <iostream>
#include"ThreadWrapper.h"
using namespace std;

void run_thread()
{
    cout<<"run for thread!"<<endl;
}
int main()
{
    cout << "Hello world!" << endl;
    ThreadWrapper threadWrapper;
    threadWrapper.start((void*)"ttt");
    return 0;
}
