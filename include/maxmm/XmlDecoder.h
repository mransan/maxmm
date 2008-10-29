/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_XmlDecoder_h 
#define maxmm_XmlDecoder_h

#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>

#include <sstream>
#include <stdexcept>


namespace maxmm
{
    
    //! \brief generic exception class for xml decoding.
    //!
    class XmlDecoderException : public std::runtime_error
    {
        public :
            XmlDecoderException( const std::string & string )
            :   std::runtime_error( string )
            { }
    };
    
    template < int X >
    struct Int2Type { enum { value = X }; };

    //! \brief define the 2 xml typed. 
    //!
    //! * primitive correspond to type which are like <node>the value of the
    //! type</node>. The primitive type has no further decomposition, it is the
    //! end leaf of the class decomposition tree.
    //! * classlike corresponds to type that decompose themselves into other
    //! types, either classlike or primitive.
    //!
    enum EXmlType
    {
        primitive,
        classlike
    };
    
    //! \brief utility class to decide if a type is primitive or classlike.
    //!
    //! By default the T is classlike, if one decide that T should be primitive
    //! then this class should be specialize and the typedef made accordingly.
    //!
    template < typename T >
    struct XmlSwitcher
    {
        typedef Int2Type< classlike > XmlType;
    };

    //! \bried decode an Xml document into a class hierarchy.
    //!
    class XmlDecoder
    {
        public:
            
            //! \brief Constructor.
            //!
            //! \param document the xml document.
            //!
            XmlDecoder( const xmlpp::Document  *document)
            :   _document( document ),
                _current_node( _document->get_root_node( ) )
            { }
            
            //! \brief method for Xml classes to register an element.
            //!
            //! \param value_name the name of the Xml node to read.
            //! \param value the variable to assign the xml value.
            //! \param optional indicate if the xml node is optional or not. If
            //! the node/value is not optional and the node is not found in the
            //! xml document an exception will be thrown.
            //!
            template< typename T >
            void read_element( 
                const Glib::ustring &value_name , 
                T &value , 
                bool optional = false )
            {
                typename XmlSwitcher< T >::XmlType xml_type;
                this->read_element( xml_type, value_name , value , optional );
            }
            
            //! \brief read the top level value of the xml document.
            //! 
            //! This method make sure the root node correspond to the one
            //! indicated by the class T if not throws an exception.
            //!
            //! \param value the value matching the root node.
            //!
            template< typename  T >
            void read_element(
                T& value )
            {
                if( T::root_node == _current_node->get_name( ) )
                {
                    xmlpp::Node * current_node_rec = _current_node;
                    try
                    {
                        value.decode( *this );
                    }
                    catch( std::exception & )
                    {
                        _current_node = current_node_rec;
                        throw;
                    }
                }
                else
                {
                    std::ostringstream error;
                    error   << "invalid root node : \"" 
                            << _current_node->get_name( ) 
                            << "\" , instead of \"" 
                            << T::root_node 
                            << "\"";
                    throw XmlDecoderException( error.str( ) );
                }
            }
        private:
            
            template< typename T >
            void read_element( 
                Int2Type< classlike > xml_type,
                const Glib::ustring &value_name,
                T &value , 
                bool optional)
            {
                xmlpp::Element *value_element = this->get_value_element( value_name , optional );
                if( 0 == value_element ) 
                {
                    return ;
                }
               
                xmlpp::Node *current_node_rec = _current_node;
                _current_node = value_element;
                value.decode( *this );
                _current_node = current_node_rec;
            }
                    
            template< typename T >
            void read_element( 
                Int2Type< primitive > xml_type,
                const Glib::ustring &value_name , 
                T &value , 
                bool optional )
            {
                xmlpp::Element *value_element = this->get_value_element( value_name , optional );
                if( 0 == value_element ) 
                {
                    return ;
                }

                xmlpp::TextNode *value_textnode = value_element->get_child_text( );
                if( 0 == value_textnode )
                {
                    std::ostringstream error;
                    error << "node " << value_name << " does not contain any value" << std::endl;
                    throw XmlDecoderException( error.str( ) );
                }
                
                std::istringstream value_string( value_textnode->get_content( ).raw( ) );
                
                value_string >> value;
            }

            
            xmlpp::Element * get_value_element( const Glib::ustring& value_name , bool optional )
            {
                xmlpp::Node::NodeList nodes = _current_node->get_children( value_name );
                
                //
                // make sure only one children is found.
                //
                switch( nodes.size( ) )
                {
                    case 0:
                        {
                            if( true == optional )
                            { 
                                return 0;
                            }
                            std::ostringstream error;
                            error   <<  "child " 
                                    << value_name 
                                    << " not found in the Xml";

                            throw XmlDecoderException( error.str( ) );
                        }
                        break;
                    case 1:
                        break;
                    default:
                         {
                            std::ostringstream error;
                            error   <<  "more than one child found for " 
                                    << value_name;

                            throw XmlDecoderException( error.str( ) );
                        }
                        break;
                }
                
                //
                // Make sure the node found is an element.
                //
                xmlpp::Element * value_element = 
                    dynamic_cast< xmlpp::Element * >( nodes.front( ) );
            
                if( 0 == value_element )
                {
                    std::ostringstream error;
                    error << value_name << " is not an Xml node." << std::endl;
                    throw XmlDecoderException( error.str( ) );
                }
                return value_element;            
            }
        private:
            const xmlpp::Document *_document;
            xmlpp::Node *_current_node;
    };
}

#define MAKE_TYPE_XML_PRIMITIVE( type ) \
namespace maxmm\
{\
template< >\
    struct XmlSwitcher< type >\
    {\
        typedef Int2Type< primitive > XmlType;\
    };\
}

MAKE_TYPE_XML_PRIMITIVE( uint8_t )
MAKE_TYPE_XML_PRIMITIVE( uint16_t )
MAKE_TYPE_XML_PRIMITIVE( uint32_t )
MAKE_TYPE_XML_PRIMITIVE( uint64_t )

MAKE_TYPE_XML_PRIMITIVE( int8_t )
MAKE_TYPE_XML_PRIMITIVE( int16_t )
MAKE_TYPE_XML_PRIMITIVE( int32_t )
MAKE_TYPE_XML_PRIMITIVE( int64_t )

MAKE_TYPE_XML_PRIMITIVE( std::string )

#endif
