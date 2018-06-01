#include<gtest/gtest.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../client_proto.h"
#include <pthread.h>

TEST(server,connection)
{
    ASSERT_EQ(exec_test1(),0);
}

TEST(server,send_recv)
{
    ASSERT_EQ(exec_test2(),0);
}

TEST(server,send_results)
{
    ASSERT_EQ(exec_test3(),0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
