#include <AgentTest.h>

#include <maxmm/ma/Agent.h>

namespace maxmm
{
namespace test
{
void AgentTest::setUp(void)
{

}

void AgentTest::tearDown(void)
{

}

void AgentTest::test_constructor(void)
{
    using namespace maxmm::ma;
    {
        Agent agent(AgentId(0));
        
        CPPUNIT_ASSERT_EQUAL(uint32_t(0), agent._id._id);
        
        {
            agent.add_outlink(AgentId(1));
            agent.add_outlink(AgentId(2));
        }
        
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), agent._outlinks.size());
        CPPUNIT_ASSERT_EQUAL(
            OutLink(AgentId(0), AgentId(1)), 
            agent._outlinks.at(0));
        
        std::cout << agent << std::endl;
    }

}

void AgentTest::test_accessors(void)
{
    using namespace maxmm::ma;
    {
        Agent agent(AgentId(0));
        CPPUNIT_ASSERT_EQUAL(AgentId(0), agent.id());

        {
            OutLink link(AgentId(0), AgentId(1));
            CPPUNIT_ASSERT_EQUAL(AgentId(0), link.from());
            CPPUNIT_ASSERT_EQUAL(AgentId(1), link.to());
        }
    }
}




CppUnit::TestSuite *AgentTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<AgentTest>(
            "AgentTest::test_constructor",
            &AgentTest::test_constructor));
    
    suite->addTest(
        new CppUnit::TestCaller<AgentTest>(
            "AgentTest::test_accessors",
            &AgentTest::test_accessors));

    return suite;
}

} // namespace test
} // namespace maxmm
