#include <gtest/gtest.h>
#include <MQCloud/API.hpp>



TEST(APITestSuit, CreateBackEndTest) {
	//this test should work only in case a defaultBackend is presened
	using namespace MQCloud;
	BackEndFactory backeEnd;
	auto ctx = backeEnd.CreateContext();
    EXPECT_TRUE(ctx) << "context must
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
   RUN_ALL_TESTS();
	std::cin.get();
   return 0;
}