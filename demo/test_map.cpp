#include <iostream>
#include <map>

using namespace std;

int main()
{
    map<string,string> maps;
    maps.insert(map<string,string>::value_type("172.0.0.1","this is test1"));

    map<string,string>::iterator it;
    it=maps.find("172.0.0.1");

    if(it==maps.end())
    {
        std::cout<<"this is end"<<std::endl;
    }
    else{
        std::cout<<it->first<<std::endl;
        std::cout<<it->second<<std::endl;
        it->second+=" this is add";
        std::cout<<it->first<<std::endl;
        std::cout<<it->second<<std::endl;
    }
    return 0;
}
