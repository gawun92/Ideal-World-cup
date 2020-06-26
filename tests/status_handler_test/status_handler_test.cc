#include "config_parser.h"
#include "data_structures.h"
#include "make_response.h"
#include "status_request_handler.h"
#include "gtest/gtest.h"

class status_handler_test : public ::testing::Test {
protected:
    NginxConfigStatement statement1_;
    NginxConfig config1_;
    std::string prefix1_;
    void SetUp() override {
        prefix1_ = "/status";
    }
};

TEST_F(status_handler_test, StatusInitTest) {
    status_request_handler *handler = status_request_handler::Init(prefix1_, config1_);
    EXPECT_NE(handler, nullptr);
    delete handler;
}

TEST_F(status_handler_test, StatusInitializeTest) {
    status_request_handler *handler = status_request_handler::Init(prefix1_, config1_);
    base_request_handler::add_to_all_handlers("some handler", "some path");
    request req;
    response resp = handler->do_handle(req);
    EXPECT_EQ(resp.status, response::ok);
    delete handler;
}
