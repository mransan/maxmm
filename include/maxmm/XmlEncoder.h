/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_XmlEncoder_h 
#define maxmm_XmlEncoder_h

#include <maxmm/detail/Xml_detail.h>
#include <maxmm/NullableValue.h>
#include <boost/bind.hpp>

#include <algorithm>
#include <list>
#include <set>
#include <sstream>
#include <stdexcept>

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
        
        //! \brief Constructor
        //!
        //! \param document - xml++ document object
        //!
        XmlEncoder(xmlpp::Document &document)
        : _current_node(document.get_root_node()) 
        {
        
        }


        //! \brief write an Xml element.
        //! 
        //! \param value_name: the name of the element
        //! \param  value: the object to encode in the xml element
        //!
        template<typename T>
        void write_element(
            const Glib::ustring &value_name, 
            const T &value)
        {
            typename detail::XmlSwitcher<T>::XmlType xml_type;
            this->write_element(xml_type, value_name, value );
        }
        
        //! \brief write an Xml element that can exist or not.
        //!
        //! \param value_name:  name of the element to create
        //! \param value: nullable value to write.
        //!
        template<typename T>
        void write_element(
            const Glib::ustring &value_name,
            const maxmm::NullableValue<T> &value)
        {
            if(true == value.null())
            {
                return;
            }
            
            typename detail::XmlSwitcher<T>::XmlType xml_type;
            this->write_element(xml_type, value_name, value.value()); 
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
            std::for_each(
                container.begin(),
                container.end(),
                bind(
                    f,
                    this,
                    boost::cref(item_name),
                    boost::cref(first_name),
                    boost::cref(second_name),
                    _1));
        }
    
        template<typename T>
        static std::ostream& to_stream(
            std::ostream &stream, 
            T const& value,
            std::string const& root_node)
        {
            xmlpp::Document document;
            {
                document.create_root_node(root_node);
            }
            maxmm::XmlEncoder encoder(document);
            value.encode(encoder);
            stream << document.write_to_string();

            return stream ;
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
                value_text <<value;
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
