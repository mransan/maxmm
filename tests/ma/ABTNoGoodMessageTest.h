/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_ABTNoGoodMessage_h
#define maxmm_test_ABTNoGoodMessage_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace maxmm
{
namespace test
{

class ABTNoGoodMessageTest : public CppUnit::TestFixture
{
public:

    void setUp(void);

    void tearDown(void);

    void test_accessors(void);                
    void test_xml(void);

    static CppUnit::TestSuite* getSuite(void);

};

} // namespace test
} // namespace maxmm

#endif
