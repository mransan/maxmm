/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <XmlEncoderTest.h>
#include <maxmm/NullableValue.h>
#include <maxmm/XmlEncoder.h>
#include <libxml/tree.h>

#include <set>

namespace 
{
    struct XmlPrimitiveMandatory
    {
        XmlPrimitiveMandatory(uint32_t value)
        :   _value(value)
        { }
        
        bool operator<(const XmlPrimitiveMandatory &v) const 
        {
            return _value <v._value ;
        }
        void encode(maxmm::XmlEncoder &encoder) const
        {
             encoder.write_element("node1" , _value);
        }

        uint32_t _value;
    };
    
    struct XmlClassLike
    {
        XmlClassLike(void)
        :   _primitive1(2) , _primitive2(3)
        { }

        void encode(maxmm::XmlEncoder &encoder)  
        {
            encoder.write_element("primitive1" , _primitive1);
            encoder.write_element("primitive2" , _primitive2);
        }

        XmlPrimitiveMandatory _primitive1;
        XmlPrimitiveMandatory _primitive2;
    };
    
    
    struct Compare
    {
        bool operator()(uint32_t lhs , uint32_t rhs)
        {
            std::cout <<"less" <<std::endl;
            return lhs <rhs ;
        }
    };

    class Choice
    {
    public:
        Choice();


    private:
        union selection
        {
            maxmm::StackBuffer<XmlClassLike> _classlike;
            maxmm::StackBuffer<XmlPrimitiveMandatory> _primitive;
        };

        enum selection_id
        {
           CLASSLIKE = 1
          ,PRIMITIVE = 2
          ,INVALID
        };
    
    };

}


namespace maxmm
{
    namespace test
    {
        XmlEncoderTest::XmlEncoderTest(void)
        { }

        XmlEncoderTest::~XmlEncoderTest(void)
        { }

        void XmlEncoderTest::setUp(void)
        { }

        void XmlEncoderTest::tearDown(void)
        { }
        
        void XmlEncoderTest::test_primitive(void)
        {
            //
            // Test that a valid xml is correclty decoded.
            //
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }
                XmlEncoder encoder(document);
                
                XmlPrimitiveMandatory test(1);
                try
                {
                    test.encode(encoder);
                }
                catch(...)
                {
                    CPPUNIT_ASSERT(false);
                }
                Glib::ustring expected = 
                    "<?xml version=\"1.0\"?>\n"
                    "<root><node1>1</node1></root>\n";

                Glib::ustring xml = document.write_to_string(); 
            
                CPPUNIT_ASSERT_EQUAL(expected, xml);
            }
        }
        
        void XmlEncoderTest::test_classlike(void)
        {
            
            //Test that a valid xml is correctly decoded.
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }
                
                XmlEncoder encoder(document);
                
                XmlClassLike test;
                try
                {
                    test.encode(encoder);
                }
                catch(...)
                {
                    CPPUNIT_ASSERT(false);
                }
                
                Glib::ustring expected = 
                    "<?xml version=\"1.0\"?>\n"
                    "<root>"
                      "<primitive1>"
                        "<node1>2</node1>"
                      "</primitive1>"
                      "<primitive2>"
                        "<node1>3</node1>"
                      "</primitive2>"
                    "</root>\n";
                
                Glib::ustring xml = document.write_to_string();

                CPPUNIT_ASSERT_EQUAL(expected, xml);
            }
        }
        
        //void XmlEncoderTest::test_rootnode(void)
        //{

        //   //
        //   // Test that a top level class is correctly decoded.
        //   //
        //   {
        //        Glib::ustring xml("<root><node1>12</node1></root>");
        //        
        //        xmlpp::DomParser parser;
        //        parser.parse_memory(xml);
        //        XmlEncoder encoder(parser.get_document());
        //        
        //        XmlPrimitiveOptional test;
        //        try
        //        {
        //            encoder.write_element(test);
        //        }
        //        catch(...)
        //        {
        //            CPPUNIT_ASSERT(false);
        //        }
        //        
        //        CPPUNIT_ASSERT(12  == test._value);
        //    }
        //    
        //    //
        //    // Test that an invalid root node is correctly detected.
        //    //
        //    {
        //        Glib::ustring xml("<invalid><node1>12</node1></invalid>");
        //        
        //        xmlpp::DomParser parser;
        //        parser.parse_memory(xml);
        //        XmlEncoder encoder(parser.get_document());
        //        
        //        XmlPrimitiveOptional test;
        //        int except = 0;
        //        try
        //        {
        //            encoder.write_element(test);
        //        }
        //        catch(maxmm::XmlEncoderException & e)
        //        {
        //            std::cout <<e.what() <<std::endl;
        //            except = 1;
        //        }
        //        catch(std::exception &)
        //        {
        //            except = 2;
        //        }
        //        
        //        CPPUNIT_ASSERT(1  == except);
        //    }
        //}
        //

        void XmlEncoderTest::test_container(void)
        {
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }

                XmlEncoder encoder(document);

                std::vector<uint32_t> list;
                {
                    list.push_back(1);
                    list.push_back(2);
                    list.push_back(3);
                }
                try
                {
                    encoder.write_container("list" , "item" , list);
                }
                catch(std::exception& e)
                {
                    std::cout <<e.what() <<std::endl;
                    CPPUNIT_ASSERT(false);
                }
                
                Glib::ustring xml = document.write_to_string();

                Glib::ustring expected = 
                    std::string("<?xml version=\"1.0\"?>\n") 
                  + std::string("<root>")
                  + std::string(  "<list>")
                  + std::string(    "<item>1</item><item>2</item><item>3</item>")
                  + std::string(  "</list>")
                  + std::string("</root>\n");

                CPPUNIT_ASSERT_EQUAL(expected , xml);
            }
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }
                
                XmlEncoder encoder(document);

                std::set<uint32_t> set;
                {
                    set.insert(3);
                    set.insert(2);
                    set.insert(1);
                }
                try
                {
                    encoder.write_container("set" , "item" , set);
                }
                catch(std::exception& e)
                {
                    std::cout <<e.what() <<std::endl;
                    CPPUNIT_ASSERT(false);
                }
                
                Glib::ustring expected = 
                    std::string("<?xml version=\"1.0\"?>\n") 
                  + std::string("<root>")
                  + std::string(  "<set>")
                  + std::string(    "<item>1</item><item>2</item><item>3</item>")
                  + std::string(  "</set>")
                  + std::string("</root>\n");

                Glib::ustring xml = document.write_to_string();

                CPPUNIT_ASSERT_EQUAL(expected, xml); 
            } 
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }
                
                XmlEncoder encoder(document);

                std::set<uint32_t, std::greater<uint32_t> > set;
                {
                    set.insert(3);
                    set.insert(2);
                    set.insert(1);
                }
                try
                {
                    encoder.write_container("set" , "item" , set);
                }
                catch(std::exception& e)
                {
                    std::cout <<e.what() <<std::endl;
                    CPPUNIT_ASSERT(false);
                }
                
                Glib::ustring expected = 
                    std::string("<?xml version=\"1.0\"?>\n") 
                  + std::string("<root>")
                  + std::string(  "<set>")
                  + std::string(    "<item>3</item><item>2</item><item>1</item>")
                  + std::string(  "</set>")
                  + std::string("</root>\n");

                Glib::ustring xml = document.write_to_string();

                CPPUNIT_ASSERT_EQUAL(expected, xml); 
            } 
        }
        
        void XmlEncoderTest::test_pair(void)
        {
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }
                XmlEncoder encoder(document);

                std::pair<uint32_t , uint32_t> pair;
                pair.first = 12;
                pair.second = 13;
                try
                {
                    encoder.write_element("pair","first", "second", pair);
                }
                catch(...)
                {
                    CPPUNIT_ASSERT(false);
                }

                std::cout <<document.write_to_string() <<std::endl; 
                
            }
        }
       
        void XmlEncoderTest::test_map(void)
        {
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }
                XmlEncoder encoder(document);

                std::map<uint32_t , uint32_t> map;
                map.insert(std::make_pair<uint32_t, uint32_t>(1, 1));
                map.insert(std::make_pair<uint32_t, uint32_t>(2, 2));
                map.insert(std::make_pair<uint32_t, uint32_t>(3, 3));

                try
                {
                    encoder.write_container(
                        "map" , 
                        "item" , 
                        "first" , 
                        "second" ,
                        map);
                }
                catch(std::exception& e)
                {
                    std::cout <<e.what() <<std::endl;
                    CPPUNIT_ASSERT(false);
                }
                
                Glib::ustring expected = 
                    std::string("<?xml version=\"1.0\"?>\n") 
                  + std::string("<root>")
                  + std::string(  "<map>")
                  + std::string(    "<item><first>1</first><second>1</second></item>")
                  + std::string(    "<item><first>2</first><second>2</second></item>")
                  + std::string(    "<item><first>3</first><second>3</second></item>")
                  + std::string(  "</map>")
                  + std::string("</root>\n");

                Glib::ustring xml = document.write_to_string();
                CPPUNIT_ASSERT_EQUAL(expected, xml);
            }
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }

                XmlEncoder encoder(document);

                std::map<uint32_t , uint32_t , std::greater<uint32_t> > map;
                map.insert(std::make_pair<uint32_t, uint32_t>(1, 1));
                map.insert(std::make_pair<uint32_t, uint32_t>(2, 2));
                map.insert(std::make_pair<uint32_t, uint32_t>(3, 3));

                try
                {
                    encoder.write_container(
                        "map" , 
                        "item" , 
                        "first" , 
                        "second" ,
                        map);
                }
                catch(std::exception& e)
                {
                    CPPUNIT_ASSERT(false);
                }
                
                Glib::ustring expected = 
                    std::string("<?xml version=\"1.0\"?>\n") 
                  + std::string("<root>")
                  + std::string(  "<map>")
                  + std::string(    "<item><first>3</first><second>3</second></item>")
                  + std::string(    "<item><first>2</first><second>2</second></item>")
                  + std::string(    "<item><first>1</first><second>1</second></item>")
                  + std::string(  "</map>")
                  + std::string("</root>\n");

                Glib::ustring xml = document.write_to_string();
                CPPUNIT_ASSERT_EQUAL(expected, xml);
            }
        }
            
        void XmlEncoderTest::test_nullable(void)
        {
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }
                
                XmlEncoder encoder(document);

                NullableValue<uint32_t>  value(3);
                encoder.write_element("value", value);

                std::cout << document.write_to_string() << std::endl;
            }
            {
                xmlpp::Document document;
                {
                    document.create_root_node("root");
                }
                
                XmlEncoder encoder(document);

                NullableValue<uint32_t> value;
                encoder.write_element("value", value);

                std::cout << document.write_to_string() << std::endl;
            }
        }
                
        CppUnit::TestSuite *XmlEncoderTest::getSuite(void)
        {
             CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
             suite->addTest(
                new CppUnit::TestCaller<XmlEncoderTest>(
                    "XmlEncoderTest::test_primitive",
                    &XmlEncoderTest::test_primitive));
             suite->addTest(
                new CppUnit::TestCaller<XmlEncoderTest>(
                    "XmlEncoderTest::test_classlike",
                    &XmlEncoderTest::test_classlike));
             //suite->addTest(
             //   new CppUnit::TestCaller<XmlEncoderTest>(
             //       "XmlEncoderTest::test_rootnode",
             //       &XmlEncoderTest::test_rootnode));
             suite->addTest(
                new CppUnit::TestCaller<XmlEncoderTest>(
                    "XmlEncoderTest::test_container",
                    &XmlEncoderTest::test_container));
             suite->addTest(
                new CppUnit::TestCaller<XmlEncoderTest>(
                    "XmlEncoderTest::test_pair",
                    &XmlEncoderTest::test_pair));
             suite->addTest(
                new CppUnit::TestCaller<XmlEncoderTest>(
                    "XmlEncoderTest::test_map",
                    &XmlEncoderTest::test_map));
             suite->addTest(
                new CppUnit::TestCaller<XmlEncoderTest>(
                    "XmlEncoderTest::test_nullable",
                    &XmlEncoderTest::test_nullable));
  
             return suite; 
        }
    }
}

