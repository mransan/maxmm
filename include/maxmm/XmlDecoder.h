/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_XmlDecoder_h 
#define maxmm_XmlDecoder_h

#include <maxmm/detail/Xml_detail.h>
#include <maxmm/NullableValue.h>
#include <sstream>
#include <stdexcept>

#include <list>
#include <set>

namespace maxmm
{
    
//! \brief generic exception class for xml decoding.
//!
class XmlDecoderException : public std::runtime_error
{
    public :
        XmlDecoderException(const std::string & string)
        :   std::runtime_error(string)
        { }
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
        XmlDecoder(const xmlpp::Document  *document)
        :   _current_node(document->get_root_node())
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
            const Glib::ustring &value_name, 
            T &value, 
            bool optional = false) const 
        {
            typename detail::XmlSwitcher< T >::XmlType xml_type;
            this->read_element(xml_type, value_name, value, optional);
        }
            

        template<typename T>
        void read_element(
            const Glib::ustring &value_name,
            maxmm::NullableValue<T> &value)
        {
            typename detail::XmlSwitcher<T>::XmlType xml_type;
            bool found = this->read_element(xml_type, value_name, value.makeValue(), true);
            if(false == found)
            {
                value.makeNull();
            }
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
            T& value) const 
        {
            if(T::root_node == _current_node->get_name())
            {
                detail::NodeReseter reseter(_current_node);
                value.decode(*this);
            }
            else
            {
                std::ostringstream error;
                error   << "invalid root node : \"" 
                        << _current_node->get_name() 
                        << "\", instead of \"" 
                        << T::root_node 
                        << "\"";
                throw XmlDecoderException(error.str());
            }
        }
        
        template< typename T, typename U >
        void read_element(
            const Glib::ustring &item_name,
            const Glib::ustring &first_name, 
            const Glib::ustring &second_name,
            std::pair< T, U > &pair, 
            bool optional = false) const
        {
            xmlpp::Element * container_element 
                = this->get_value_element(
                    item_name, 
                    optional);
            if(0 == container_element)
            {
                return ;
            }
        
            detail::NodeReseter reseter(_current_node); 
            _current_node = container_element;    
            this->read_element(first_name, pair.first, optional);
            this->read_element(second_name, pair.second, optional);
        }
      
        template< template< typename U, 
                            typename V > class T,
                  typename U,
                  typename V  >
        void read_container(
            const Glib::ustring &container_name,
            const Glib::ustring &item_name,
            T< U, V > &container,
            bool optional = false) const 
        {
            this->read_single_entry_container(
                container_name, 
                item_name, 
                container, 
                detail::Int2Type< detail::use_push_back >(),
                optional); 
        }

        template< typename U,
                  typename V,
                  typename W>
        void read_container(const Glib::ustring &container_name, 
                            const Glib::ustring &item_name,
                            std::set< U, V, W > &container,
                            bool optional = false) const
        {
            this->read_single_entry_container(
                container_name, 
                item_name, 
                container,
                detail::Int2Type< detail::use_insert >(), 
                optional);
        }

        
        
        //! \brief read a double-entry container such as std::map
        //!
        //! Due to template specification, it does not yet support std::set with custom 
        //! comparaison operator.
        //!
        template< typename T >
        void read_container(
            const Glib::ustring container_name, 
            const Glib::ustring item_name,
            const Glib::ustring first_name,
            const Glib::ustring second_name,
            T &container, 
            bool optional = false) const
        {
            xmlpp::Element * container_element 
                = this->get_value_element(
                    container_name, 
                    optional);
            if(0 == container_element)
            {
                return ;
            }
        
            container.clear();
            
            xmlpp::Node::NodeList nodes 
                 = container_element->get_children(item_name);
            if(true == nodes.empty())
            {
                std::ostringstream error;
                error << "no item found for set " << container_name ;
                throw XmlDecoderException(error.str());
            }
            
            detail::NodeReseter reseter(_current_node);                

            for(xmlpp::Node::NodeList::iterator 
                    itr  = nodes.begin() ;
                    itr != nodes.end() ;
                    ++itr)
            {
                // 
                // If an error occurs here then it means that the contains
                // in the sequence does NOT have a default constructor.
                //
                std::pair<
                    typename T::key_type,
                    typename T::mapped_type > new_pair;
                
                _current_node = *itr;
                this->read_element(first_name, new_pair.first, optional);
                this->read_element(second_name, new_pair.second, optional);
                _current_node = 0;
                container.insert(new_pair);
            }
        }

    private:
        
        //! \brief read a single entry container(set, list, vector) from an XML. 
        //!
        //! Due to template specification, it does not yet support std::set with custom 
        //! comparaison operator.
        //!
        //! \param container_name the xml tag of the container.
        //! \param item_name the xml tag of each item of the container.
        //! \param container the container itself.
        //! \param optional indicate if the container is optional or not. If
        //! it is optional and the container_name xml node is not found an
        //! exception is thrown.
        //! 
        //! \verbatim 
/*
<container_name>
    <item_name>
        < ... ></ ... > <!-- xml decomposition of the item element.
    </item_name>
    <item_name>
        < ... ></ ... >
    </item_name>
</container_name> \endverbatim */ 
        //!
        //!
        //!
        template< typename U, typename V >
        void read_single_entry_container(
            const Glib::ustring &container_name, 
            const Glib::ustring &item_name, 
            U  &container, 
            V behavior,
            bool optional = false) const 
        {
            xmlpp::Element * container_element 
                = this->get_value_element(
                    container_name, 
                    optional);
            if(0 == container_element)
            {
                return ;
            }
        
            container.clear();
            
            xmlpp::Node::NodeList nodes 
                 = container_element->get_children(item_name);
            if(true == nodes.empty())
            {
                std::ostringstream error;
                error << "no item found for set " << container_name ;
                throw XmlDecoderException(error.str());
            }
            
            detail::NodeReseter reseter(_current_node);                
            //typename 
            //    detail::XmlBehaviorSwitcher< T, Y >::XmlContainerBehavior 
            //        container_behavior;

            for(xmlpp::Node::NodeList::iterator 
                    itr  = nodes.begin() ;
                    itr != nodes.end() ;
                    ++itr)
            {
                // 
                // If an error occurs here then it means that the contains
                // in the sequence does NOT have a default constructor.
                //
                typename U::value_type new_item;
                
                //
                // In order to reuse the this->read_element method we must
                // provide a node with a single child, the child being the
                // current node of the list.
                //
                xmlpp::Document fake_document;
                fake_document.create_root_node("fake");
                {
                    //
                    // This insure that the document remains the same if an
                    // exception is thrown.
                    //
                    detail::NodeExchanger exchanger(
                        *itr, 
                        container_element,
                        fake_document.get_root_node());
                                    
                    _current_node = fake_document.get_root_node();
                    this->read_element(item_name, new_item);
                    _current_node = 0;
                    XmlDecoder::generic_insert(behavior, container, new_item);
                }
            }
        }

        
        //! \brief insert a new item in a container using the push_back
        //! method.
        //!
        //! \param container_behavior this paramerter is just to provide a
        //! different method signature. (see Int2Type from alexandrescu).
        //! \param container the container in which the new element is
        //! inserted.
        //! \param value the item to insert.
        //!
        template< class T >
        static void generic_insert(
            detail::Int2Type< detail::use_push_back > container_behavior, 
            T& container, 
            const typename T::value_type& value) 
        {
            container.push_back(value);
        }
        
        //! \brief insert a new item in a container using the insert
        //! method.
        //!
        //! \param container_behavior this paramerter is just to provide a
        //! different method signature. (see Int2Type from alexandrescu).
        //! \param container the container in which the new element is
        //! inserted.
        //! \param value the item to insert.
        //!
        template< class T >
        static void generic_insert(
            detail::Int2Type< detail::use_insert > container_behavior,
            T& container, 
            typename T::value_type& value)
        {
            container.insert(value);
        }
        

        
        template< typename T >
        bool read_element(
            detail::Int2Type< detail::classlike > xml_type,
            const Glib::ustring &value_name,
            T &value, 
            bool optional) const 
        {
            xmlpp::Element *value_element = this->get_value_element(value_name, optional);
            if(0 == value_element) 
            {
                return false;
            }
            
            detail::NodeReseter reseter(_current_node);
            _current_node = value_element;
            value.decode(*this);
            return true;
        }
                
        template< typename T >
        bool read_element(
            detail::Int2Type< detail::primitive > xml_type,
            const Glib::ustring &value_name, 
            T &value, 
            bool optional) const 
        {
            xmlpp::Element *value_element = this->get_value_element(value_name, optional);
            if(0 == value_element) 
            {
                return false;
            }

            xmlpp::TextNode *value_textnode = value_element->get_child_text();
            if(0 == value_textnode)
            {
                std::ostringstream error;
                error << "node " << value_name << " does not contain any value" << std::endl;
                throw XmlDecoderException(error.str());
            }
            
            std::istringstream value_string(value_textnode->get_content().raw());
            
            value_string >> value;
            return true;
        }

         
        xmlpp::Element * get_value_element(const Glib::ustring& value_name, bool optional) const 
        {
            xmlpp::Node::NodeList nodes = _current_node->get_children(value_name);
            
            //
            // make sure only one children is found.
            //
            switch(nodes.size())
            {
                case 0:
                    {
                        if(true == optional)
                        { 
                            return 0;
                        }
                        std::ostringstream error;
                        error   <<  "child " 
                                << value_name 
                                << " not found in the Xml";

                        throw XmlDecoderException(error.str());
                    }
                    break;
                case 1:
                    break;
                default:
                    {
                        std::ostringstream error;
                        error   <<  "more than one child found for " 
                                << value_name;

                        throw XmlDecoderException(error.str());
                    }
                    break;
            }
            
            //
            // Make sure the node found is an element.
            //
            xmlpp::Element * value_element = 
                dynamic_cast< xmlpp::Element * >(nodes.front());
        
            if(0 == value_element)
            {
                std::ostringstream error;
                error << value_name << " is not an Xml node." << std::endl;
                throw XmlDecoderException(error.str());
            }
            return value_element;            
        }
        
    private:
         mutable xmlpp::Node *_current_node;
};
}



#endif
