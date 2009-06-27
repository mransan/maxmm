/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/


#ifndef maxmm_ma_Assignment_h 
#define maxmm_ma_Assignment_h

#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>

#include <vector>

namespace maxmm{ namespace test{ class AssignmentTest; } }
namespace maxmm{ namespace ma{ template<typename VARIABLE,
                                        typename VALUE> class Assignment; } }

template<typename VALUE>
bool operator==(const maxmm::ma::Assignment<VALUE> &lhs,
                const maxmm::ma::Assignment<VALUE> &rhs);

namespace maxmm
{
namespace ma
{

template<typename VALUE>
class Assignment 
{
friend class maxmm::test::AssignmentTest;
public:
    explicit Assignment(void);
    explicit Assignment(VARIABLE var, VALUE val);

    VARIABLE variable(void) const;
    VALUE value(void) const; 
    
    void assign(VARIABLE variable, VALUE value);

    bool set(void) const;
    void decode(const maxmm::XmlDecoder &decoder);
    void encode(maxmm::XmlEncoder &encoder) const;

private:
    VARIABLE _variable;
    VALUE _value;
    bool _set;
};

template<typename VALUE>
Assignment<VALUE>::Assignment(void)
:   _set(false)
{

}

template<typename VALUE>
Assignment<VALUE>::Assignment(VARIABLE var, VALUE val)
:   _variable(var),
    _value(val),
    _set(true)
{

}

template<typename VALUE>
VARIABLE Assignment<VALUE>::variable(void) const
{
    return _variable;
}

template<typename VALUE>
VALUE Assignment<VALUE>::value(void) const
{
    return _value;
}

template<typename VALUE>
void Assignment<VALUE>::decode(const maxmm::XmlDecoder &decoder)
{
    decoder.read_element("variable", _variable);
    decoder.read_element("value", _value);
    _set = true;
}

template<typename VALUE>
void Assignment<VALUE>::encode(maxmm::XmlEncoder &encoder) const
{
    if(false == _set)
    {
        return;
    }
    
    encoder.write_element("variable", _variable);
    encoder.write_element("value", _value);
}

template<typename VALUE>
void Assignment<VALUE>::assign(VARIABLE variable, VALUE value)
{
    _variable = variable;
    _value = value;
    _set = true;
}


} // namespace ma
} // namespace maxmm


template<typename VALUE>
bool operator==(const maxmm::ma::Assignment<VALUE> &lhs,
                const maxmm::ma::Assignment<VALUE> &rhs)
{
    if( (lhs._set == false) || (rhs._set == false) )
    {
        return false;
    }

    return ( (lhs._variable == rhs._variable) && 
             (lhs._value == rhs._value) );
}

#endif
