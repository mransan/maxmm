/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_XmlEncoder_h 
#define maxmm_XmlEncoder_h

#include <maxmm/detail/Xml_detail.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <sstream>
#include <stdexcept>

#include <list>
#include <set>

namespace maxmm
{

    class XmlEncoderException : public std::runtime_error
    {
        public:
            
            XmlEncoderException(const std::string &string)
            :   std::runtime_error(string)
            {
            }
    };


    class XmlEncoder
    {
        public:
            

            XmlEncoder(const xmlpp::Document *document)
            : _current_node(document->get_root_node()) 
            {
            
            }
    
    
            template< typename T >
            void write_element(
                const Glib::ustring &value_name, 
                const T &value)
            {
                typename detail::XmlSwitcher< T >::XmlType xml_type;
                this->write_element(xml_type, value_name, value );
            }
            
            template<typename T, typename U>
            void write_element(
                const Glib::ustring &pair_name,
                const Glib::ustring &first_name,
                const Glib::ustring &second_name,
                const std::pair<T, U> &pair)
            {
                xmlpp::Element *value_element  
                    = _current_node->add_child(pair_name);
                detail::NodeReseter reseter(_current_node);
                _current_node = value_element; 

                this->write_element(first_name, pair.first);
                this->write_element(second_name, pair.second);
            }
            
            template<template<typename U,
                              typename V> class T,
                        
                     typename U,
                     typename V>
            void write_container(
                const Glib::ustring &container_name,
                const Glib::ustring &item_name,
                const T<U, V> &container)
            {
                xmlpp::Element *value_element  
                    = _current_node->add_child(container_name);
                detail::NodeReseter reseter(_current_node);
                _current_node = value_element; 
                
                typedef typename T<U, V>::value_type value_type;
                void (XmlEncoder::*f)(const Glib::ustring &, 
                                      const value_type&) = &XmlEncoder::write_element;
                
                using namespace boost;
                std::for_each(container.begin(),
                              container.end(),
                              bind(
                                f,
                                this,
                                boost::cref(item_name),
                                _1));
            } 
            
            template<typename U,
                     typename V,
                     typename W>
            void write_container(
                const Glib::ustring &container_name,
                const Glib::ustring &item_name,
                const std::set<U, V, W> &container)
            {
                xmlpp::Element *value_element  = _current_node->add_child(container_name);
                detail::NodeReseter reseter(_current_node);
                _current_node = value_element;     
                
                typedef typename std::set<U, V, W>::value_type value_type; 
                void (XmlEncoder::*f)(const Glib::ustring &,
                                      const value_type &) = &XmlEncoder::write_element;
                

                using namespace boost;
                std::for_each(container.begin(),
                              container.end(),
                              bind(
                                f,
                                this,
                                boost::cref(item_name),
                                _1));
            }

            template<typename U,
                     typename V,
                     typename W,
                     typename X>
            void write_container(
                const Glib::ustring &container_name,
                const Glib::ustring &item_name,
                const Glib::ustring &first_name,
                const Glib::ustring &second_name,
                const std::map<U, V, W, X> &container)
            {
                xmlpp::Element *value_element  = _current_node->add_child(container_name);
                detail::NodeReseter reseter(_current_node);
                _current_node = value_element;     
                
                typedef typename std::map<U, V, W, X>::value_type value_type; 
                void (XmlEncoder::*f)(const Glib::ustring &,
                                      const Glib::ustring &,
                                      const Glib::ustring &,
                                      const value_type &) = &XmlEncoder::write_element;
                
                using namespace boost;
                std::for_each(container.begin(),
                              container.end(),
                              bind(
                                f,
                                this,
                                boost::cref(item_name),
                                boost::cref(first_name),
                                boost::cref(second_name),
                                _1));
            }
            

        private:

            template<typename T>
            void write_element(
                detail::Int2Type<detail::classlike> xml_type,
                const Glib::ustring &value_name,
                const T &value)
            {
                xmlpp::Element *value_element = _current_node->add_child(value_name);
                // TODO - Maxime test return value;
                detail::NodeReseter reseter(_current_node);
                _current_node = value_element;
                
                value.encode(*this);
            }
    
                
            template<typename T>
            void write_element(
                detail::Int2Type<detail::primitive> xml_type,
                const Glib::ustring &value_name,
                const T &value)
            {
                xmlpp::Element *value_element = _current_node->add_child(value_name);
                //TODO - Maxime test return value;
                
                std::ostringstream value_text;
                {
                    value_text << value;
                    // TODO - Maxime test value_text is in a correct state.
                    // TODO - Maxime make sure that this works with
                    // Glib::ustring type as well.
                }
                
                xmlpp::TextNode *text_node = value_element->add_child_text(value_text.str());
                // TODO - Maxime test return value;
            
            } 
        
            xmlpp::Node *_current_node;
    };
    
}

#endif
