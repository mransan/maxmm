/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_XmlEncoderTest_h
#define maxmm_test_XmlEncoderTest_h


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class XmlEncoderTest : CppUnit::TestFixture 
        {
            public:
                XmlEncoderTest(void);
                ~XmlEncoderTest(void);
           
                void setUp(void);
                void tearDown(void);

                void test_primitive(void);
                void test_classlike(void);
                void test_pair(void);
                void test_container(void);
                void test_map(void);
                void test_nullable(void);

                static CppUnit::TestSuite* getSuite(void);

            private:
        };
    }
}

#endif
