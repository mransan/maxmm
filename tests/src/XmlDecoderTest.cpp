/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <XmlDecoderTest.h>

#include <maxmm/XmlDecoder.h>
#include <libxml/tree.h>

#include <set>

namespace 
{
    struct XmlPrimitiveMandatory
    {
        XmlPrimitiveMandatory( void )
        :   _value( 0 )
        { }
        
        bool operator<( const XmlPrimitiveMandatory &v ) const 
        {
            return _value < v._value ;
        }
        void decode( const maxmm::XmlDecoder & decoder )
        {
             decoder.read_element( "node1" , _value );
        }

        uint32_t _value;
    };
    
    struct XmlPrimitiveOptional
    {
        static const Glib::ustring root_node;
        XmlPrimitiveOptional( void )
        :   _value( 0 )
        { }

        void decode( const maxmm::XmlDecoder & decoder )
        {
             decoder.read_element( "node1" , _value , true);
        }

        uint32_t _value;
    };
    const Glib::ustring XmlPrimitiveOptional::root_node("root"); 
    
    struct XmlClassLike
    {
        XmlClassLike( void )
        :   _primitive1( ) , _primitive2( )
        { }

        void decode( const maxmm::XmlDecoder & decoder )
        {
            decoder.read_element( "primitive1" , _primitive1 , true );
            decoder.read_element( "primitive2" , _primitive2 , true );
        }

        XmlPrimitiveMandatory _primitive1;
        XmlPrimitiveMandatory _primitive2;
    };
    
    struct SysClass
    {
        SysClass( void )
        { }
        
        void decode( maxmm::XmlDecoder &decoder )
        {
            decoder.read_element( "primitive" , _primitive );
            decoder.read_element( "classlike" , _classlike );
            decoder.read_container( "set" , "item" , _set );
        }

        XmlPrimitiveMandatory _primitive;
        XmlClassLike _classlike;
        std::set< XmlPrimitiveMandatory > _set;
    };
    
    struct Compare
    {
        bool operator( )( uint32_t lhs , uint32_t rhs )
        {
            std::cout << "less" << std::endl;
            return lhs < rhs ;
        }
    };
}


namespace maxmm
{
    namespace test
    {
        XmlDecoderTest::XmlDecoderTest( void )
        { }

        XmlDecoderTest::~XmlDecoderTest( void )
        { }

        void XmlDecoderTest::setUp( void )
        { }

        void XmlDecoderTest::tearDown( void )
        { }
        
        void XmlDecoderTest::test_primitive( void )
        {
            //
            // Test that a valid xml is correclty decoded.
            //
            {
                Glib::ustring xml("<root><node1>12</node1></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveMandatory test;
                try
                {
                    test.decode( decoder );
                }
                catch( ... )
                {
                    CPPUNIT_ASSERT( false );
                }
                CPPUNIT_ASSERT( test._value == 12 );
            }
            
            //
            // Test that an xml withouth the mandatory node raise the correct
            // exception.
            //
            {
                Glib::ustring xml("<root><node2>12</node2></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveMandatory test;
                int exception = 0;
                try
                {
                    test.decode( decoder );
                }
                catch( maxmm::XmlDecoderException& e )
                {
                    std::cout << e.what( ) << std::endl; 
                    exception = 1;
                }
                catch( ... )
                {
                    exception = 2;
                }
                CPPUNIT_ASSERT_EQUAL( int( 1 ) , exception );
            }
            
            //
            // Test that the decoder raises an exception when multiple nodes are
            // found.
            //
            {
                Glib::ustring xml("<root><node1>12</node1><node1>13</node1></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveMandatory test;
                int exception = 0;
                try
                {
                    test.decode( decoder );
                }
                catch( maxmm::XmlDecoderException& e )
                {
                    std::cout << e.what( ) << std::endl; 
                    exception = 1;
                }
                catch( ... )
                {
                    exception = 2;
                }

                CPPUNIT_ASSERT_EQUAL( int( 1 ) , exception );
            }
            
            //
            // Test that the decoder raises an exception when no content is
            // found in the node.
            //
            {
                Glib::ustring xml("<root><node1><boom></boom></node1></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveMandatory test;
                int exception = 0;
                try
                {
                    test.decode( decoder );
                }
                catch( maxmm::XmlDecoderException& e )
                {
                    std::cout << e.what( ) << std::endl; 
                    exception = 1;
                }
                catch( ... )
                {
                    exception = 2;
                }
                CPPUNIT_ASSERT_EQUAL( int( 1 ) , exception );
            }

        }
        
        void XmlDecoderTest::test_classlike( void )
        {
            //
            // Test that a valid xml is correctly decoded.
            //
            {
                Glib::ustring xml(
                    "<root>"
                        "<primitive1>"
                            "<node1>12</node1>"
                        "</primitive1>"
                        "<primitive2>"
                            "<node1>13</node1>"
                        "</primitive2>"
                    "</root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlClassLike test;
                try
                {
                    test.decode( decoder );
                }
                catch( ... )
                {
                    CPPUNIT_ASSERT( false );
                }
                CPPUNIT_ASSERT( test._primitive1._value == 12 );
                CPPUNIT_ASSERT( test._primitive2._value == 13 );
            }
            
        }
        
        void XmlDecoderTest::test_rootnode( void )
        {

           //
           // Test that a top level class is correctly decoded.
           //
           {
                Glib::ustring xml("<root><node1>12</node1></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveOptional test;
                try
                {
                    decoder.read_element( test );
                }
                catch( ... )
                {
                    CPPUNIT_ASSERT( false );
                }
                
                CPPUNIT_ASSERT( 12  == test._value );
            }
            
            //
            // Test that an invalid root node is correctly detected.
            //
            {
                Glib::ustring xml("<invalid><node1>12</node1></invalid>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveOptional test;
                int except = 0;
                try
                {
                    decoder.read_element( test );
                }
                catch( maxmm::XmlDecoderException & e )
                {
                    std::cout << e.what( ) << std::endl;
                    except = 1;
                }
                catch( std::exception & )
                {
                    except = 2;
                }
                
                CPPUNIT_ASSERT( 1  == except );
            }
        }
        

        void XmlDecoderTest::test_container( void )
        {
            {
                Glib::ustring xml(
                    "<root>\
                        <list>\
                            <item>1</item>\
                            <item>2</item>\
                            <item>3</item>\
                        </list>\
                     </root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );

                std::vector< uint32_t > list;

                try
                {
                    decoder.read_container( "list" , "item" , list );
                }
                catch( std::exception& e )
                {
                    std::cout << e.what( ) << std::endl;
                    CPPUNIT_ASSERT( false );
                }
                
                CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , list[0] );
                CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , list[1] );
                CPPUNIT_ASSERT_EQUAL( uint32_t( 3 ) , list[2] );
            }
            {
                Glib::ustring xml(
                    "<root>\
                        <set>\
                            <item>1</item>\
                            <item>2</item>\
                            <item>3</item>\
                        </set>\
                     </root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );

                std::set< uint32_t > set;

                try
                {
                    decoder.read_container( "set" , "item" , set );
                }
                catch( std::exception& e )
                {
                    std::cout << e.what( ) << std::endl;
                    CPPUNIT_ASSERT( false );
                }
                
                std::set< uint32_t > set_ref;
                set_ref.insert( 1 );
                set_ref.insert( 2 );
                set_ref.insert( 3 );
                CPPUNIT_ASSERT( set_ref == set );
            } 
            {
                Glib::ustring xml(
                    "<root>\
                        <set>\
                            <item>1</item>\
                            <item>2</item>\
                            <item>3</item>\
                        </set>\
                     </root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );

                std::set< uint32_t, std::greater< uint32_t > > set;

                try
                {
                    decoder.read_container( "set" , "item" , set );
                }
                catch( std::exception& e )
                {
                    std::cout << e.what( ) << std::endl;
                    CPPUNIT_ASSERT( false );
                }
                
                std::set< uint32_t, std::greater< uint32_t > > set_ref;
                set_ref.insert( 1 );
                set_ref.insert( 2 );
                set_ref.insert( 3 );
                CPPUNIT_ASSERT( set_ref == set );
            } 
        }
        
        void XmlDecoderTest::test_pair( void )
        {
            {
                Glib::ustring xml(
                    "<root>\
                      <pair>\
                        <first>1</first>\
                        <second>2</second>\
                      </pair>\
                    </root>");

                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );

                std::pair< uint32_t , uint32_t > pair;
                try
                {
                    decoder.read_element("pair", "first", "second", pair );
                }
                catch(std::exception &e )
                {
                    std::cout << "exception " <<  e.what() << std::endl;
                    CPPUNIT_ASSERT( false );
                }

                CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , pair.first );
                CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , pair.second );
            }
        }
        
        void XmlDecoderTest::test_map( void )
        {
            {
                Glib::ustring xml(
                    "<root>\
                        <map>\
                            <item>\
                                <first>1</first>\
                                <second>2</second>\
                            </item>\
                            <item>\
                                <first>3</first>\
                                <second>4</second>\
                            </item>\
                        </map>\
                    </root>");

                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );

                std::map< uint32_t , uint32_t > map;
                
                try
                {
                    decoder.read_container( 
                        "map" , 
                        "item" , 
                        "first" , 
                        "second" ,
                        map);
                }
                catch( std::exception& e )
                {
                    std::cout << e.what( ) << std::endl;
                    CPPUNIT_ASSERT( false );
                }
                
                std::map< uint32_t , uint32_t > map_ref;
                map_ref.insert( std::make_pair( 1 , 2 ) );
                map_ref.insert( std::make_pair( 3 , 4 ) );
                
                CPPUNIT_ASSERT( map_ref == map );
            }
            {
                Glib::ustring xml(
                    "<root>\
                        <map>\
                            <item>\
                                <first>1</first>\
                                <second>2</second>\
                            </item>\
                            <item>\
                                <first>3</first>\
                                <second>4</second>\
                            </item>\
                        </map>\
                    </root>");

                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );

                std::map< uint32_t , uint32_t , std::greater< uint32_t > > map;
                
                try
                {
                    decoder.read_container( 
                        "map" , 
                        "item" , 
                        "first" , 
                        "second" ,
                        map);
                }
                catch( std::exception& e )
                {
                    std::cout << e.what( ) << std::endl;
                    CPPUNIT_ASSERT( false );
                }
                
                std::map< uint32_t , uint32_t , std::greater< uint32_t > > map_ref;
                map_ref.insert( std::make_pair( 1 , 2 ) );
                map_ref.insert( std::make_pair( 3 , 4 ) );
                
                CPPUNIT_ASSERT( map_ref == map );
            }
        }

        void XmlDecoderTest::test_system( void )
        {
            Glib::ustring xml(
                "<root>\
                    <primitive>\
                        <node1>1</node1>\
                    </primitive>\
                    <classlike>\
                        <primitive1>\
                            <node1>2</node1>\
                        </primitive1>\
                        <primitive2>\
                            <node1>3</node1>\
                        </primitive2>\
                    </classlike>\
                    <set>\
                        <item><node1>4</node1></item>\
                        <item><node1>5</node1></item>\
                    </set>\
                </root>" );
            
            xmlpp::DomParser parser;
            parser.parse_memory( xml );
            XmlDecoder decoder( parser.get_document( ) );

            SysClass test;
            try
            {
                test.decode( decoder );
            }
            catch( XmlDecoderException& e )
            {
                std::cout << e.what( ) << std::endl;
                CPPUNIT_ASSERT( false );
            }
            
            CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , test._primitive._value );
            CPPUNIT_ASSERT_EQUAL( uint32_t( 2 ) , test._classlike._primitive1._value );
            CPPUNIT_ASSERT_EQUAL( uint32_t( 3 ) , test._classlike._primitive2._value );
            CPPUNIT_ASSERT_EQUAL( uint32_t( 4 ) , test._set.begin( )->_value );
            CPPUNIT_ASSERT_EQUAL( uint32_t( 5 ) , (++test._set.begin( ))->_value );
        }
        
        CppUnit::TestSuite *XmlDecoderTest::getSuite( void )
        {
             CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
             suite->addTest(
                new CppUnit::TestCaller<XmlDecoderTest>(
                    "XmlDecoderTest::test_primitive",
                    &XmlDecoderTest::test_primitive) );
             suite->addTest(
                new CppUnit::TestCaller<XmlDecoderTest>(
                    "XmlDecoderTest::test_classlike",
                    &XmlDecoderTest::test_classlike) );
             suite->addTest(
                new CppUnit::TestCaller<XmlDecoderTest>(
                    "XmlDecoderTest::test_rootnode",
                    &XmlDecoderTest::test_rootnode) );
             suite->addTest(
                new CppUnit::TestCaller<XmlDecoderTest>(
                    "XmlDecoderTest::test_container",
                    &XmlDecoderTest::test_container) );
             suite->addTest(
                new CppUnit::TestCaller<XmlDecoderTest>(
                    "XmlDecoderTest::test_pair",
                    &XmlDecoderTest::test_pair) );
             suite->addTest(
                new CppUnit::TestCaller<XmlDecoderTest>(
                    "XmlDecoderTest::test_map",
                    &XmlDecoderTest::test_map) );
             suite->addTest(
                new CppUnit::TestCaller<XmlDecoderTest>(
                    "XmlDecoderTest::test_system",
                    &XmlDecoderTest::test_system) );
   
             return suite; 
        }
    }
}

