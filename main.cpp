#include "Vector.hpp"
#include <iostream>
#include "timer.hpp"
#include <vector>
 
int main()
{
    double partTime;
    Timer timer;
    unsigned int sz = 100000000; //10000, 100000, 1000000, 10000000, 100000000
    timer.reset();
    std::vector<int> v1;
    for (int i = 1; i <= sz; ++i)
    v1.push_back(i);
    
    partTime = timer.elapsed();
    std::cout << sz << " std::vector laikas: " << partTime << std::endl;
    timer.reset();

    Vector<int> v2;
    for (int i = 1; i <= sz; ++i)
    v2.push_back(i);
    
    partTime = timer.elapsed();
    std::cout << sz << " Custom vector laikas: " << partTime << std::endl;
}