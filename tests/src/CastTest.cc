#include <CastTest.h>

namespace maxmm
{
    namespace test
    {
    
        namespace 
        {
            struct S1
            {
                uint16_t _i1; 
                uint16_t _i2;
                S1( uint16_t i1 , uint16_t i2 )
                : _i1( i1 ) , _i2( i2 )
                { }
            };
            
            struct S2
            {
                uint32_t _i1;
                uint16_t _i3;
                S2( 
                    uint32_t i1 , 
                    uint16_t i3 )
                :   _i1( i1 ) , _i3( i3 )
                { }
            };
            
            struct SBase
            {
                uint32_t _i1; 
                uint32_t _i2;
                SBase( uint32_t i1 , uint32_t i2 )
                : _i1( i1 ) , _i2( i2 )
                { }
                virtual ~SBase( void )
                { }
            };

            struct SDerived : public SBase
            {
                uint32_t _i3;
                SDerived( uint32_t i1 , uint32_t i2 , uint32_t i3 )
                :   SBase( i1 , i2 ) , _i3( i3 )
                { }
            };
            
            struct SDerivedString : public SBase
            {
                std::string _st;
                SDerivedString( 
                    uint32_t i1 , 
                    uint32_t i2 , 
                    const std::string &st)
                :   SBase( i1 , i2 ) , _st( st )
                { }
            };
        }

        CastTest::CastTest( void )
        {
        
        }
    
        CastTest::~CastTest( void )
        {

        }
        
        void CastTest::setUp( void )
        {
        
        }

        void CastTest::tearDown( void )
        {
            
        }
        
        void CastTest::test_primitive_type_cast( void )
        {
            {
                uint16_t from( 10 );
                uint8_t to = static_cast< uint8_t >( from );
                
                CPPUNIT_ASSERT_EQUAL( static_cast< uint8_t >( 10 ) , to );
            }
            
            {
                uint16_t from( 0xFFFF );
                uint8_t  to = static_cast< uint16_t >( from );

                uint8_t  expected( 0xFF );

                CPPUNIT_ASSERT_EQUAL( expected , to );
            }
            
            {
                uint16_t from( 0x1234 );
                uint8_t  to = static_cast< uint16_t >( from );

                uint8_t  expected( 0x34 );

                CPPUNIT_ASSERT_EQUAL( expected , to );
            }
        }
        
        void CastTest::test_reinterpret_cast( void )
        {
            
            {
                S1 from( 1 , 2 );
                S2 *to = reinterpret_cast< S2* >( &from );
                
                CPPUNIT_ASSERT_EQUAL( uint32_t( 0x00020001 ) , to->_i1 );
            } 
            
            {
                S2 from( 1 , 2 );
                S1 *to = reinterpret_cast< S1* >( &from );
                
                CPPUNIT_ASSERT_EQUAL( uint16_t( 1 ) , to->_i1 );
                CPPUNIT_ASSERT_EQUAL( uint16_t( 0 ) , to->_i2 );
            }
            

        }
        void CastTest::test_static_cast( void )
        {
            
            //
            // You can use static cast from base to derived 
            // with pointer...
            //
            {
                SDerived sd( 1 , 2 , 3);
                SBase *from = &sd;

                SDerived *to= static_cast< SDerived* >( from );
                
                CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , to->_i1 );
                CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , to->_i2 );
                CPPUNIT_ASSERT_EQUAL( uint32_t( 3 ) , to->_i3 );
            }
            //
            // or with reference ...
            //
            {
                SDerived sd( 1 , 2 , 3 );
                SBase& from = sd;
                SDerived &to = static_cast< SDerived& >( from );
            
                CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , to._i1 );
                CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , to._i2 );
                CPPUNIT_ASSERT_EQUAL( uint32_t( 3 ) , to._i3 );
            }

            // 
            // Static cast can be used even if the pointer is type is wrong.
            //
            {
                SDerivedString sdst( 1 , 2 , "boom" );
                SBase *from = &sdst;
                {
                    SDerived *to = static_cast< SDerived* >( from );
                    
                    CPPUNIT_ASSERT( 0 != to );
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , to->_i1 );
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , to->_i2 );
                }
                
                {
                    SDerived &to = static_cast< SDerived& >( *from );
                    
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , to._i1 );
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , to._i2 );
                }
                //
                // Dyanmic cast with pointer returns 0 in case of an invalid
                // cast.
                //
                {
                    SDerived *to = dynamic_cast< SDerived* >( from );
                
                    CPPUNIT_ASSERT( 0 == to );
                }
                
                //
                // Dynamic cast with reference throws an std::bad_cast
                // exception in case of an invalid cast.
                //
                {
                    bool success = false;
                    SBase &ref = *from;
                    try
                    {
                        SDerived &to = dynamic_cast< SDerived& >( ref );
                    }
                    catch( std::bad_cast& e )
                    {
                        success = true;
                    }
                    CPPUNIT_ASSERT( success );
                }
            }
        }

        void CastTest::test_const_cast( void )
        {
            {
                S1 s1( 1 , 2 );
                const S1 &s1_cref = s1;
                
                S1 &s1_ref = const_cast< S1& >( s1_cref );
            }

            // You cannot remove const using dynamic cast. To go from a const
            // base reference to child non-const reference you must cast twice.
            // (One dynamic cast and one const_cast)
            {
                SDerived s_derived( 1 , 2 , 3 );
                const SBase &s_base_cref = s_derived;
                
                SDerived &s_cast = 
                    const_cast< SDerived& >( 
                        dynamic_cast< const SDerived& >( s_base_cref ) );
            }
            
            {
                SDerived s_derived( 1 , 2 , 3 );
                const SBase &s_base_cref = s_derived;
                
                SDerived &s_cast = 
                    const_cast< SDerived& >(
                        static_cast< const SDerived& >( s_base_cref ) );
            }

        }

        CppUnit::TestSuite *CastTest::getSuite( void )
        {
             CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
             suite->addTest(
                new CppUnit::TestCaller<CastTest>(
                    "CastTest::test_primitive_type_cast",
                    &CastTest::test_primitive_type_cast) );
             suite->addTest(
                new CppUnit::TestCaller<CastTest>(
                    "CastTest::test_reinterpret_cast",
                    &CastTest::test_reinterpret_cast ) );
             suite->addTest( 
                new CppUnit::TestCaller<CastTest>( 
                    "CastTest::test_static_cast", 
                    &CastTest::test_static_cast ) );
             suite->addTest(
                new CppUnit::TestCaller<CastTest>(
                    "CastTest::test_const_cast" ,
                    &CastTest::test_const_cast ) );
            return suite; 
        }
    }
}
