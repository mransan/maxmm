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

class NeuronTest : public CppUnit::TestFixture
{
public:

    inline void setUp(void) {} 

    inline void tearDown(void) {}

    void test_constructor_and_accessor(void);

    static CppUnit::TestSuite* getSuite(void);

};

} // namespace test
} // namespace maxmm

#endif
