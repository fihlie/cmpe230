// Example program
#include <iostream>

using namespace std;

main ()
{
    unsigned int x = 50816;
    uint8_t xlow = x & 0xff;
    uint8_t xhigh = (x >> 8);
    cout<<(int)xlow<<endl;
    cout<<(int)xhigh<<endl;
    unsigned int y = ( ( xhigh & 0xFF ) << 8 ) | ( xlow & 0xFF );
    cout<<y;
}