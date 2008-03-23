#include <maxutils/all.h>
#include <TestCommon.h>

using namespace maxutils;


int main()
{
    initLogger();

    std::string *initvalue = new std::string("initial value");
    
    MTWrapper<std::string>              mt_testString(20, initvalue);
    MTWrapper<std::string>::RetiredList rlist;
    
    std::string *testString;
    
    for(int i=0 ; i<1000 ; i++)
    {
        LOG_INFO << "creating new string #: " << i << std::endl;
        std::string *newString = new std::string("value: ");
        *newString += 65 + i%26 ;
    
        mt_testString.update(newString, rlist);
        
        {
            HPRecReaderScopeLock<std::string>(mt_testString, &testString);
            LOG_INFO << "mystring after get: " << *testString << std::endl;
        }
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

