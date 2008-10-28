/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <LibXmlTest.h>

#include <maxmm/XmlDecoder.h>

namespace 
{
    struct XmlPrimitiveMandatory
    {
        XmlPrimitiveMandatory( void )
        :   _value( 0 )
        { }

        void decode( maxmm::XmlDecoder & decoder )
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

        void decode( maxmm::XmlDecoder & decoder )
        {
             decoder.read_element( "node1" , _value , true);
        }

        uint32_t _value;
    };
    const Glib::ustring XmlPrimitiveOptional::root_node("root"); 
    
    struct XmlClassLike
    {
        XmlClassLike( void )
        :   _primitive( )
        { }

        void decode( maxmm::XmlDecoder & decoder )
        {
            decoder.read_element( "primitive" , _primitive , true );
        }

        XmlPrimitiveMandatory _primitive;
    };
}


namespace maxmm
{
    namespace test
    {
        LibXmlTest::LibXmlTest( void )
        { }

        LibXmlTest::~LibXmlTest( void )
        { }

        void LibXmlTest::setUp( void )
        { }

        void LibXmlTest::tearDown( void )
        { }

        void LibXmlTest::test_simple( void )
        {
            {
                Glib::ustring xml("<root><node1>12</node1></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                std::cout << parser.get_document( )->get_root_node( )->get_name( ) << std::endl;
            }

            {
                Glib::ustring xml("<root><node1>12</node1></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveMandatory test;
                test.decode( decoder );

                CPPUNIT_ASSERT( test._value == 12 );
            }
            
            {
                Glib::ustring xml("<root><node2>12</node2></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveMandatory test;
                try
                {
                    test.decode( decoder );
                }
                catch( maxmm::XmlDecoderException& e )
                {
                    std::cout << e.what( ) << std::endl; 
                }
                catch( ... )
                {
                    CPPUNIT_ASSERT( false );
                }
            }
            
            {
                Glib::ustring xml("<root><node1>12</node1><node1>13</node1></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveMandatory test;
                try
                {
                    test.decode( decoder );
                }
                catch( maxmm::XmlDecoderException& e )
                {
                    std::cout << e.what( ) << std::endl; 
                }
                catch( ... )
                {
                    CPPUNIT_ASSERT( false );
                }
            }
            
            {
                Glib::ustring xml("<root><node1><boom></boom></node1></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveMandatory test;
                try
                {
                    test.decode( decoder );
                }
                catch( maxmm::XmlDecoderException& e )
                {
                    std::cout << e.what( ) << std::endl; 
                }
                catch( ... )
                {
                    CPPUNIT_ASSERT( false );
                }
            }
            
            {
                Glib::ustring xml("<root></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlPrimitiveOptional test;
                try
                {
                    test.decode( decoder );
                }
                catch( maxmm::XmlDecoderException& e )
                {
                    CPPUNIT_ASSERT( false );
                }
                catch( ... )
                {
                    CPPUNIT_ASSERT( false );
                }
            }

            {
                Glib::ustring xml("<root><primitive><node1>12</node1></primitive></root>");
                
                xmlpp::DomParser parser;
                parser.parse_memory( xml );
                XmlDecoder decoder( parser.get_document( ) );
                
                XmlClassLike test;
                test.decode( decoder );

                CPPUNIT_ASSERT( test._primitive._value == 12 );
            }
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
        
        CppUnit::TestSuite *LibXmlTest::getSuite( void )
        {
             CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
             suite->addTest(
                new CppUnit::TestCaller<LibXmlTest>(
                    "LibXmlTest::test_simple",
                    &LibXmlTest::test_simple) );
         
             return suite; 
        }
   
    }
}

