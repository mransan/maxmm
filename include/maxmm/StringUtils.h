/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_StringUtils_h
#define maxmm_StringUtils_h

#include <boost/utility.hpp>

#include <string>
#include <vector>

namespace maxmm
{
    class StringUtils : public boost::noncopyable 
    {
        public:

            //! \brief default token handler that store tokens in an array.
            //!
            struct DefaultTokenHandler
            {
                void operator( )(
                    const std::string &str ,
                    std::string::size_type start , 
                    std::string::size_type end )
                {
                    _tokens.push_back( str.substr( start , end - start ) );
                }
                
                //! \brief return copy of the token array.
                //!
                std::vector< std::string > tokens( void )
                { 
                    return _tokens;
                }
                
                private:
                    std::vector< std::string > _tokens;
            };
            
            //! \brief tokenise a string and return the token handler.
            //!
            //! for this method to work the token handler MUST have a default
            //! constructor.
            //!
            //! \param str the string to tokenise.
            //! \param delimiter the token delimiter.
            //! \return copy of the token handler used.
            //!
            template< typename TokenHandler >
            static TokenHandler tokenise( 
                const std::string &str , 
                const char delimiter = ',' )
            {
                TokenHandler handler;
                StringUtils::tokenise<TokenHandler>( str , handler , delimiter );
                return handler;
            }

            //! \brief tokenise a string using a given token handler.
            //!
            //! \param str the string to tokenise.
            //! \param handler the token handler.
            //! \param delimiter the token delimiter.
            //!
            template< typename TokenHandler >
            static void tokenise( 
                const std::string & str  , 
                TokenHandler &handler ,
                const char delimiter = ',')
            {
                std::string::size_type start = 0;
                std::string::size_type end = str.find( delimiter , start );
                std::size_t size = str.size( );
                
                bool found = false;
                while(  ( std::string::npos != end )
                        &&
                        ( start != size ) )
                {
                    handler( str , start , end );
                    start = ++end;
                    end = str.find( delimiter , start );
                    found = true;
                }
                if( found == true  && ( start != str.size( ) ) )
                {
                    handler(  str , start , size );
                } 
            }

        private:
            StringUtils( void )
            { }
    };
}



#endif
