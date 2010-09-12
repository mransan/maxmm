#include <NeuronTest.h>

#include <maxmm/nnet/Neuron.h>

namespace maxmm
{
namespace test
{

void NeuronTest::test_constructor_and_accessor(void)
{
    using namespace maxmm::nnet;
    
    {
        Neuron n(3);
        
        CPPUNIT_ASSERT_EQUAL((unsigned int)(3), n.num_of_inputs());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(3), n.weights().size()); 
    }
    
    {
        std::vector<double> weights(3, 1);
        Neuron n(weights);
        
        CPPUNIT_ASSERT_EQUAL((unsigned int)(3), n.num_of_inputs());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(3), n.weights().size()); 
    
        CPPUNIT_ASSERT(weights == n.weights());
    }

}


CppUnit::TestSuite *NeuronTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<NeuronTest>(
            "NeuronTest::test_constructor_and_accessor",
            &NeuronTest::test_constructor_and_accessor));
    return suite;
}

} // namespace test
} // namespace maxmm
