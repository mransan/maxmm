#include <maxutils/all.h>
#include <TestCommon.h>

using namespace maxutils;


int main()
{
    initLogger();

    std::string *initvalue = new std::string("initial value");
    MTWrapper<std::string> mtstring(20, initvalue);
    MTWrapper<std::string>::RetiredList rlist;
    
    HPtrRecord *p;
    std::string *mystring;
    mtstring.getPtr(&mystring, &p);
    LOG_INFO << "mystring after get: " << *mystring << std::endl;
    
    for(int i=0 ; i<1000 ; i++)
    {
        LOG_INFO << "creating new string #: " << i << std::endl;
        std::string *newString = new std::string("value: ");
        *newString += 65 + i%26 ;
    
        mtstring.update(newString, rlist);
        
        mtstring.getPtr(&mystring, &p);
        LOG_INFO << "mystring after get: " << *mystring << std::endl;
        HPtrRecord::release(p);

     }

    for (MTWrapper<std::string>::RetiredListItr itr = rlist.begin();
         itr!=rlist.end();
         itr++
        ) 
    {
        delete *itr;
    }
    
    Logger::close();
}

