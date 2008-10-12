#include <CastTest.h>

namespace maxmm
{
    namespace test
    {
    
        
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
        
        void CastTest::test_static_cast( void )
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
        
            {
                struct S1
                {
                    uint32_t _i1; 
                    uint32_t _i2;
                    S1( uint32_t i1 , uint32_t i2 )
                    : _i1( i1 ) , _i2( i2 )
                    { }
                    virtual ~S1( void )
                    { }
                };

                struct SDerived : public S1
                {
                    uint32_t _i3;
                    SDerived( uint32_t i1 , uint32_t i2 , uint32_t i3 )
                    :   S1( i1 , i2 ) , _i3( i3 )
                    { }
                };
                
                struct SDerivedString : public S1
                {
                    std::string _st;
                    SDerivedString( 
                        uint32_t i1 , 
                        uint32_t i2 , 
                        const std::string &st)
                    :   S1( i1 , i2 ) , _st( st )
                    { }
                };
                
                //
                // You can use static cast from base to derived 
                //
                {
                    SDerived sd( 1 , 2 , 3);
                    S1 *from = &sd;

                    SDerived *to= static_cast< SDerived* >( from );
                    
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , to->_i1 );
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , to->_i2 );
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 3 ) , to->_i3 );
                }

                //
                // But can NOT use static_cast from 2 different pointer. You
                // MUST use the reinterpret_cast.
                //
                {
                    struct S2
                    {
                        uint32_t _i1; 
                        uint32_t _i2;
                        uint32_t _i3;
                        S2( uint32_t i1 , uint32_t i2 , uint32_t i3 )
                        : _i1( i1 ) , _i2( i2 ) , _i3( i3 )
                        { }
                        //
                        // Note that since we are comparing S1 and S2 and that
                        // virtual function modify the internal data structure
                        // we need to have a virtual destructor in order to
                        // reinterpret correctly the 2 structures and retreive
                        // expected value.
                        //
                        virtual ~S2( void )
                        { }
                    };

                    S2 from( 1 , 2 , 3 );
                    S1 *to = reinterpret_cast< S1* >( &from );
                    
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , to->_i1 );
                    CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , to->_i2 );
                }
                
                // 
                // Static cast can be used even if the pointer is type is wrong.
                //
                {
                    SDerivedString sdst( 1 , 2 , "boom" );
                    S1 *from = &sdst;
                    {
                        SDerived *to = static_cast< SDerived* >( from );
                        
                        CPPUNIT_ASSERT( 0 != to );
                        CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , to->_i1 );
                        CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , to->_i2 );
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
                        S1 &ref = *from;
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
        }

        CppUnit::TestSuite *CastTest::getSuite( void )
        {
             CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
             suite->addTest( 
                new CppUnit::TestCaller<CastTest>( 
                    "CastTest::test_static_cast", 
                    &CastTest::test_static_cast ) );
      
            return suite; 
        }
    }
}
