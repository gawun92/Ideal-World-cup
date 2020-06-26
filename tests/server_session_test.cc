#include "gtest/gtest.h"
#include "server_session.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <string>

// TEST(session_test_temp, SimpleSession) {
//     boost::asio::io_service io_service;
//     session* new_session = new session(io_service);
//     new_session->start();
//     // using namespace boost::system;
//     // const error_code ec = errc::make_error_code(errc::success);
//     // new_session->handle_read(ec, 0);

//     delete new_session;
// }


TEST(session_test_temp2, SimpleSession) {
    std::string new_input = "alskmfdklsmf rgrsfg dfg \r\nwkdmakwdm \r\n\n";
    std::string result = "";
    char c_input[new_input.size() + 1];
    strcpy(c_input, new_input.c_str());
    EXPECT_FALSE(process_input(result, c_input, new_input.size()));
}
