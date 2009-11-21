/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_AgentTest_h
#define maxmm_test_AgentTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace maxmm
{
namespace test
{

class AgentTest : public CppUnit::TestFixture
{
public:

    void setUp(void);

    void tearDown(void);

    void test_constructor(void);
    void test_accessors(void);                

    static CppUnit::TestSuite* getSuite(void);

};

} // namespace test
} // namespace maxmm

#endif
