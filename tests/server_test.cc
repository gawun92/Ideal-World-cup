#include "server_server.h"
#include "config_parser.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"


TEST(ServerTest, SimpleTest) {
	boost::asio::io_service io_service;
	server s(io_service, 8080);
	EXPECT_EQ(1,1);
}