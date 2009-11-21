/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <StringUtilsTest.h>

#include <maxmm/StringUtils.h>

namespace maxmm
{
namespace test
{

void StringUtilsTest::test_tokenise(void)
{
    typedef StringUtils::DefaultTokenHandler THandler;
    
    {
        std::string str("bim , bam , boom");
        
        std::vector<std::string> tokens;
        THandler handler(tokens);
        StringUtils::tokenise<THandler>(str , handler);
        
        CPPUNIT_ASSERT_EQUAL(std::size_t(3) , tokens.size());
        CPPUNIT_ASSERT(tokens[ 0 ] ==  "bim ");
        CPPUNIT_ASSERT(tokens[ 1 ] ==  " bam ");
        CPPUNIT_ASSERT(tokens[ 2 ] ==  " boom");
    }
    
    {
        std::string str("");
        std::vector< std::string > tokens;
        THandler handler(tokens);
        StringUtils::tokenise< THandler >(str , handler);
        
        CPPUNIT_ASSERT(tokens.empty());
    }
    
    {
        std::string str("boom,");
        
        std::vector< std::string > tokens ;
        THandler handler (tokens);
        StringUtils::tokenise< THandler >(str , handler);
        
        CPPUNIT_ASSERT_EQUAL(std::size_t(1) , tokens.size());
        CPPUNIT_ASSERT(tokens[0] == "boom");
    }
    
    {
        std::string str("boom,,,,,");
        
        std::vector< std::string > tokens;
        THandler handler(tokens);
        StringUtils::tokenise< THandler >(str , handler);
        
        CPPUNIT_ASSERT_EQUAL(std::size_t(5) , tokens.size());
        CPPUNIT_ASSERT(tokens[0] == "boom");
        CPPUNIT_ASSERT(tokens[1].empty());
    }

    {
        std::string str(",,,,,boom");
        
        std::vector< std::string > tokens;
        THandler handler(tokens);
        StringUtils::tokenise< THandler >(str , handler);

        
        CPPUNIT_ASSERT_EQUAL(std::size_t(6) , tokens.size());
        CPPUNIT_ASSERT(tokens[5] == "boom");
        CPPUNIT_ASSERT(tokens[0].empty());
    }
}

CppUnit::TestSuite *StringUtilsTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<StringUtilsTest>(
            "StringUtilsTest::test_tokenise",
            &StringUtilsTest::test_tokenise));

     return suite; 
}
}
}
