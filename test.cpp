#include <iostream>
#include "map.h"

int main()
{
    dts::map<char, int> mp;
    std::pair<char, int> pr('t', 17);
    mp.insert(pr);
    pr = {'y', 18};
    mp.insert(pr);
    pr = {'u', 19};
    mp.insert(pr);
    pr = {'i', 20};
    mp.insert(pr);
    pr = {'o', 21};
    mp.insert(pr);
    std::cout << " ";
}