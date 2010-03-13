#include <TypeTest.h>

#include <stdint.h>

namespace
{
int plus(int a , int b)
{
    return a + b;
}

class FunctionPtrTest
{
public:
    int plus_member_nonconst(int a , int b)
    {
        return a + b;
    }

    int plus_member_const(int a , int b) const 
    {
        return a + b;
    }

    static int plus_static(int a , int b)
    {
        return a + b;
    }
};

struct Copy
{
    Copy(int i)
    :   _i(i)
    { }

    Copy(const Copy& copy)
    {
        _i = copy._i;
        --_i;
    }

    Copy& operator=(const Copy& copy)
    {
        _i = copy._i;
        ++_i;
        return *this;
    }

    int _i;
};

struct A
{
    A(void)
    {
        std::cout << "A::A() "  ;
        ++iter;
    }
    
    virtual void print(void)
    { 
        std::cout << "A::print()" << std::endl;
    }
    ~A(void)
    {
        std::cout << "A::~A() " ;
    }

    static uint32_t iter;
};

uint32_t A::iter = 0;

struct B1 : public virtual A
{
    B1(void)
    { 
        std::cout << "B1::B1() "  ;
    }

    virtual void print(void)
    {
        std::cout << "B1::print()" << std::endl;
    }
    ~B1(void)
    {
        std::cout << "B1::~B1() " ;
    }
};

struct B2 : public virtual A
{
    B2(void)
    { 
        std::cout << "B2::B2() " ;
    }

    ~B2(void)
    {
        std::cout << "B2::~B2() ";
    }
};

struct B3 : public A
{
    B3(void)
    { 
        std::cout << "B3::B3() " ;
    }

    ~B3(void)
    {
        std::cout << "B3::~B3() ";
    }
};

struct C : B3 , virtual B2 ,  virtual B1
{
    C(void)
    {
        std::cout << "C::C() "  ;
    }
    ~C(void)
    {
        std::cout << "C::~C() " ;
    }
};
}



namespace maxmm
{
namespace test
{
namespace 
{
struct S1
{
    uint16_t _i1; 
    uint16_t _i2;
    S1(uint16_t i1 , uint16_t i2)
    : _i1(i1) , _i2(i2)
    { }
};

struct S2
{
    uint32_t _i1;
    uint16_t _i3;
    S2(
        uint32_t i1 , 
        uint16_t i3)
    :   _i1(i1) , _i3(i3)
    { }
};

struct SBase
{
    uint32_t _i1; 
    uint32_t _i2;
    SBase(uint32_t i1 , uint32_t i2)
    : _i1(i1) , _i2(i2)
    { }
    virtual ~SBase(void)
    { }
};

struct SDerived : public SBase
{
    uint32_t _i3;
    SDerived(uint32_t i1 , uint32_t i2 , uint32_t i3)
    :   SBase(i1 , i2) , _i3(i3)
    { }
};

struct SDerivedString : public SBase
{
    std::string _st;
    SDerivedString(
        uint32_t i1 , 
        uint32_t i2 , 
        const std::string &st)
    :   SBase(i1 , i2) , _st(st)
    { }
};
} // unnamed namespace.

TypeTest::TypeTest(void)
{

}

TypeTest::~TypeTest(void)
{

}

void TypeTest::setUp(void)
{

}

void TypeTest::tearDown(void)
{
    
}

void TypeTest::test_primitive_type_cast(void)
{
    {
        uint16_t from(10);
        uint8_t to = static_cast< uint8_t >(from);
        
        CPPUNIT_ASSERT_EQUAL(static_cast< uint8_t >(10) , to);
    }
    
    {
        uint16_t from(0xFFFF);
        uint8_t  to = static_cast< uint16_t >(from);

        uint8_t  expected(0xFF);

        CPPUNIT_ASSERT_EQUAL(expected , to);
    }
    
    {
        uint16_t from(0x1234);
        uint8_t  to = static_cast< uint16_t >(from);

        uint8_t  expected(0x34);

        CPPUNIT_ASSERT_EQUAL(expected , to);
    }
}

void TypeTest::test_reinterpret_cast(void)
{
    
    {
        S1 from(1 , 2);
        S2 *to = reinterpret_cast< S2* >(&from);
        
        CPPUNIT_ASSERT_EQUAL(uint32_t(0x00020001) , to->_i1);
    } 
    
    {
        S2 from(1 , 2);
        S1 *to = reinterpret_cast< S1* >(&from);
        
        CPPUNIT_ASSERT_EQUAL(uint16_t(1) , to->_i1);
        CPPUNIT_ASSERT_EQUAL(uint16_t(0) , to->_i2);
    }
    //
    // You can use reference and interpret cast.
    //
    {
        S2 from(1 , 2);
        S1 &to = reinterpret_cast< S1& >(from);
        
        CPPUNIT_ASSERT_EQUAL(uint16_t(1) , to._i1);
        CPPUNIT_ASSERT_EQUAL(uint16_t(0) , to._i2);
    }

    

}
void TypeTest::test_static_cast(void)
{
    
    //
    // You can use static cast from base to derived 
    // with pointer...
    //
    {
        SDerived sd(1 , 2 , 3);
        SBase *from = &sd;

        SDerived *to= static_cast< SDerived* >(from);
        
        CPPUNIT_ASSERT_EQUAL(uint32_t(1) , to->_i1);
        CPPUNIT_ASSERT_EQUAL(uint32_t(2) , to->_i2);
        CPPUNIT_ASSERT_EQUAL(uint32_t(3) , to->_i3);
    }
    //
    // or with reference ...
    //
    {
        SDerived sd(1 , 2 , 3);
        SBase& from = sd;
        SDerived &to = static_cast< SDerived& >(from);
    
        CPPUNIT_ASSERT_EQUAL(uint32_t(1) , to._i1);
        CPPUNIT_ASSERT_EQUAL(uint32_t(2) , to._i2);
        CPPUNIT_ASSERT_EQUAL(uint32_t(3) , to._i3);
    }

    // 
    // Static cast can be used even if the pointer is type is wrong.
    //
    {
        SDerivedString sdst(1 , 2 , "boom");
        SBase *from = &sdst;
        {
            SDerived *to = static_cast< SDerived* >(from);
            
            CPPUNIT_ASSERT(0 != to);
            CPPUNIT_ASSERT_EQUAL(uint32_t(1) , to->_i1);
            CPPUNIT_ASSERT_EQUAL(uint32_t(2) , to->_i2);
        }
        
        {
            SDerived &to = static_cast< SDerived& >(*from);
            
            CPPUNIT_ASSERT_EQUAL(uint32_t(1) , to._i1);
            CPPUNIT_ASSERT_EQUAL(uint32_t(2) , to._i2);
        }
        //
        // Dyanmic cast with pointer returns 0 in case of an invalid
        // cast.
        //
        {
            SDerived *to = dynamic_cast< SDerived* >(from);
        
            CPPUNIT_ASSERT(0 == to);
        }
        
        //
        // Dynamic cast with reference throws an std::bad_cast
        // exception in case of an invalid cast.
        //
        {
            bool success = false;
            SBase &ref = *from;
            try
            {
                SDerived &to = dynamic_cast< SDerived& >(ref);
            }
            catch(std::bad_cast& e)
            {
                success = true;
            }
            CPPUNIT_ASSERT(success);
        }
    }
}

void TypeTest::test_const_cast(void)
{
    {
        S1 s1(1 , 2);
        const S1 &s1_cref = s1;
        
        S1 &s1_ref = const_cast< S1& >(s1_cref);
    }

    // You cannot remove const using dynamic cast. To go from a const
    // base reference to child non-const reference you must cast twice.
    // (One dynamic cast and one const_cast)
    {
        SDerived s_derived(1 , 2 , 3);
        const SBase &s_base_cref = s_derived;
        
        SDerived &s_cast = 
            const_cast< SDerived& >(
                dynamic_cast< const SDerived& >(s_base_cref));
    }
    
    {
        SDerived s_derived(1 , 2 , 3);
        const SBase &s_base_cref = s_derived;
        
        SDerived &s_cast = 
            static_cast< SDerived& >(
                const_cast<  SBase& >(s_base_cref));
    }

}

void TypeTest::test_const_ptr(void)
{
    char original[] = "max is a moron";
    char second[] = "max is still a moron";

    const char * cptr = original;
    const char * const cStcptr = original; 
    char * const ptrc = original;

    //
    // array content modification. 
    // --------------------------
    // (const prevous to * is relevant).
    //
    
    // possible
    // --------
    ptrc[0] = 'b';

    // NOT possible
    // -----------
    // cptr[0] = 'b';
    // cStcptr[0] = 'c';
    

    //
    // Pointer modification.
    // ---------------------
    // (const after the * is relevant).
    //

    // possible
    // -------
    cptr = second;

    // NOT possible
    // -----------
    // ptrc = second;
    // cStcptr = second;
}


void TypeTest::test_function_ptr(void)
{
    //
    // Generic C syntax declaration of a function pointer is 
    // return_type (*function_name)(param1_type , param2_type)
    //
    {
        int(* function_ptr)(int , int) = &plus;
        CPPUNIT_ASSERT_EQUAL(int(3) , function_ptr(1 , 2));        
    }
    
    //
    // C++ function pointer to a non const member function of a class.
    //
    {
        int (FunctionPtrTest::*nonconst_member_function)(int , int)
            = &FunctionPtrTest::plus_member_nonconst;
        FunctionPtrTest  function_class;
        FunctionPtrTest* function_class_ptr = &function_class;

        CPPUNIT_ASSERT_EQUAL(
            int(3) , 
            (function_class.*nonconst_member_function)(1 , 2));
        CPPUNIT_ASSERT_EQUAL(
            int(3) , 
            (function_class_ptr->*nonconst_member_function)(1 , 2));
    }
    
    //
    // C++ function pointer to const member function of a class.
    // 
    {
        int(FunctionPtrTest::*const_member_function)(int , int) const
            = &FunctionPtrTest::plus_member_const;
        
        FunctionPtrTest function_class;
        FunctionPtrTest *function_class_ptr;

        CPPUNIT_ASSERT_EQUAL(
            int(3), 
            (function_class.*const_member_function)(1 , 2));
        
        CPPUNIT_ASSERT_EQUAL(
            int(3),
            (function_class_ptr->*const_member_function)(1 , 2)); 
    }

    //
    // static method in C++ are the same as C function pointer.
    //
    {
        int(*function_ptr)(int , int) = &FunctionPtrTest::plus_static;

        CPPUNIT_ASSERT_EQUAL(
            int(3) , 
            (function_ptr(1 , 2)));
    }
}

void TypeTest::test_assignment(void)
{
    {
        Copy copy = 1;
        CPPUNIT_ASSERT_EQUAL(1 , copy._i);
    }
    {
        Copy c1 = 1 ;
        Copy c2(2) ;
        Copy c3 = (c2 = c1);

        CPPUNIT_ASSERT_EQUAL(1 , c3._i);
    }
}

void TypeTest::test_virtual_inheritance(void)
{
    {
        std::cout << std::endl;
        C c;
        std::cout << std::endl;
    }
    {
        B1 b;
        b.print();
    }
    {
        B2 b;
        b.print();
    }
}

void TypeTest::test_union(void)
{
    {

        union test_union
        {
            char _c;
            uint32_t _d;
        };
        
        std::cout << sizeof(test_union) << std::endl;

        {
            test_union tu;
            tu._d = 0xFFFFFF53;

            std::cout << tu._d << std::endl;
            std::cout << tu._c << std::endl;
        
            tu._d = 0xFFFFFE53;

            std::cout << tu._c << std::endl;
        }
    }
    {
        
        class UTest
        {
        public:
            UTest()
            {
            }

            uint32_t inc(void)
            {
                return ++_i;
            }

            void reset(void)
            {
                _i = 0;
            }
        private:
            uint32_t _i;
        };


        union test_union
        {
            uint32_t _i;
            char _class_alloc[sizeof(UTest)];
        };

        std::cout << sizeof(UTest) << std::endl;
        std::cout << sizeof(test_union) << std::endl;
        
        test_union buffer;
        UTest *utest = new (&buffer._class_alloc[0]) UTest();
        utest->reset();
        std::cout << utest->inc() << std::endl;
        std::cout << utest->inc() << std::endl;

        {
            test_union bufferbad;
            std::cout << (void*)(&bufferbad._class_alloc[0]) << std::endl;
            utest = new (&bufferbad._class_alloc[0]) UTest();
            utest->reset();
            std::cout << utest->inc() << std::endl;

            UTest *utest2 = new (&bufferbad._class_alloc[0]) UTest();
            std::cout << utest2->inc() << std::endl;
        }
        test_union bufferbad;
        std::cout << (void*)(&bufferbad._class_alloc[0]) << std::endl;
        //
        // This only works with luck.
        // the memory allocated on the stack within the previous block
        // is no longer a valid one.
        // I believe it is still valid because the memory has not been
        // re-used for something else so the utest object 
        // can stilll work.
        //
        std::cout << utest->inc() << std::endl;
    }
}

CppUnit::TestSuite *TypeTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_primitive_type_cast",
            &TypeTest::test_primitive_type_cast));
     suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_reinterpret_cast",
            &TypeTest::test_reinterpret_cast));
     suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_static_cast", 
            &TypeTest::test_static_cast));
     suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_const_cast" ,
            &TypeTest::test_const_cast));
     suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_const_ptr" ,
            &TypeTest::test_const_ptr));
     suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_function_ptr" ,
            &TypeTest::test_function_ptr));
     suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_assignment" ,
            &TypeTest::test_assignment));
     suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_virtual_inheritance" ,
            &TypeTest::test_virtual_inheritance));
    suite->addTest(
        new CppUnit::TestCaller<TypeTest>(
            "TypeTest::test_union" ,
            &TypeTest::test_union));

    
    return suite; 
}
}
}
