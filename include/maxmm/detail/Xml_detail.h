/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_Xml_detail_h 
#define maxmm_Xml_detail_h

#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>
#include <libxml/tree.h>

namespace maxmm
{
    namespace detail
    {
        //! \brief Exception safety container that make sure that a pointer is
        //! reset to the value it had at construct time.
        //!
        struct NodeReseter
        {
            NodeReseter( xmlpp::Node *& current_node )
            :   _current_node( current_node ) , _rec( current_node )
            { }

            ~NodeReseter( void )
            {
                _current_node = _rec ;
            }

            private:
                xmlpp::Node *&_current_node;
                xmlpp::Node  *_rec;
        };
        
        //! \brief Exception safety for libxml tree manipulation.
        //!
        //! The class does :
        //! \li on constructor the exchangee node is moved to a new parent.
        //! \li on destructor the exchangee is moved back to its original
        //!  parent.
        //!
        struct NodeExchanger
        {
            NodeExchanger( 
                xmlpp::Node *exchangee , 
                xmlpp::Node *prev_parent , 
                xmlpp::Node *new_parent )
            :   _exchangee( exchangee ) , 
                _prev_parent( prev_parent ) , 
                _new_parent( new_parent )
            {
                xmlUnlinkNode( exchangee->cobj( ) );
                xmlAddChild( new_parent->cobj( ) , exchangee->cobj( ) );
            }

            ~NodeExchanger( void )
            {
                xmlUnlinkNode( _exchangee->cobj( ) );
                xmlAddChild( _prev_parent->cobj( ) , _exchangee->cobj( ) );
            }

            xmlpp::Node *_exchangee;
            xmlpp::Node *_prev_parent;
            xmlpp::Node *_new_parent;
        };
        
        //! \brief Alexandrescu well known class.
        //!
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
        

        //! \brief define the 2 different behavior of container for inserting
        //! element.
        //!
        enum EXmlContainerBehavior
        {
            use_push_back ,
            use_insert
        };
           
        //! \brief utility class to decide if a container uses push_back or
        //! insert for adding new element.
        //!
        //! By default the behavior is insert. If the container uses push_back
        //! it needs to be specialized. For automatic specialization the
        //! following macro is provided: MAKE_CONTAINER_USE_PUSH_BACK.
        //! Note that this macro MUST be used outside any scope.
        //!
        template < template < typename Y > class T , typename Y >
        struct XmlBehaviorSwitcher
        {
            typedef Int2Type< use_insert > XmlContainerBehavior;
        };
    }
}


#define MAKE_TYPE_XML_PRIMITIVE( type ) \
namespace maxmm\
{\
namespace detail\
{\
template< >\
    struct XmlSwitcher< type >\
    {\
        typedef Int2Type< primitive > XmlType;\
    };\
}\
}

#define MAKE_CONTAINER_USE_PUSH_BACK( container ) \
namespace maxmm\
{\
namespace detail\
{\
template< typename  Y >\
    struct XmlBehaviorSwitcher< container , Y >\
    {\
        typedef Int2Type< use_push_back > XmlContainerBehavior;\
    };\
}\
}\


#endif
