#include "config_parser.h"
#include "make_response.h"
#include "static_request_handler.h"
#include "gtest/gtest.h"

class static_handler_test : public ::testing::Test {
protected:
    NginxConfig config1_;
    NginxConfig config2_;

    void SetUp() override {
        std::shared_ptr<NginxConfigStatement> root = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        root->tokens_.push_back("root");
        root->tokens_.push_back("\"./\"");
        config1_.statements_.push_back(root);
    }
};

TEST_F(static_handler_test, StaticInitializeTest) {
    static_request_handler *handler = static_request_handler::Init("/static", config1_);
    EXPECT_NE(handler, nullptr);
    delete handler;
}

TEST_F(static_handler_test, StaticSimpleRequestTest) {
    static_request_handler *handler = static_request_handler::Init("/static", config1_);
    request req;
    req.method = "GET";
    req.uri = "/static/sample.txt";
    req.http_version_major = 1;
    req.http_version_minor = 0;
    req.raw = "/static/sample.txt HTTP/1.0";

    response resp = handler->do_handle(req);
    EXPECT_EQ(resp.status, response::ok);
    EXPECT_EQ(resp.data, "OK");
    delete handler;
}

TEST_F(static_handler_test, StaticAppendIndexTest) {
    static_request_handler *handler = static_request_handler::Init("/static", config1_);
    request req;
    req.method = "GET";
    req.uri = "/static/";
    req.http_version_major = 1;
    req.http_version_minor = 0;
    req.raw = "GET /static/ HTTP/1.0";

    response resp = handler->do_handle(req);
    EXPECT_EQ(resp.status, response::ok);
    std::string data = "index.html appended";
    EXPECT_EQ(resp.data, data);
    delete handler;
}

TEST_F(static_handler_test, StaticInvalidRequestTest) {
    static_request_handler *handler = static_request_handler::Init("/static", config1_);
    request req;
    req.method = "GET";
    req.uri = "/static/%W%RongFile.txt";
    req.http_version_major = 1;
    req.http_version_minor = 0;
    req.raw = "GET /static/%W%RongFile.txt HTTP/1.0";

    response resp = handler->do_handle(req);
    response expected_resp = response::status_to_response(response::bad_request);
    EXPECT_EQ(resp.status, response::bad_request);
    EXPECT_EQ(resp.data, expected_resp.data);
    delete handler;
}

TEST_F(static_handler_test, StaticBadRequestTest) {
    static_request_handler *handler = static_request_handler::Init("/static", config1_);
    request req;
    req.method = "GET";
    req.uri = "wrong_uri/";
    req.http_version_major = 1;
    req.http_version_minor = 0;
    req.raw = "GET /staticwrong_uri/ HTTP/1.0";

    response resp = handler->do_handle(req);
    response expected_resp = response::status_to_response(response::bad_request);

    EXPECT_EQ(resp.status, response::bad_request);
    EXPECT_EQ(resp.data, expected_resp.data);
    delete handler;
}

TEST_F(static_handler_test, StaticWrongFileTest) {
    static_request_handler *handler = static_request_handler::Init("/static", config1_);
    request req;
    req.method = "GET";
    req.uri = "/wrong_file.txt";
    req.http_version_major = 1;
    req.http_version_minor = 0;
    req.raw = "GET /static/wrong_file.txt HTTP/1.0";

    response resp = handler->do_handle(req);
    response expected_resp = response::status_to_response(response::not_found);

    EXPECT_EQ(resp.status, response::not_found);
    EXPECT_EQ(resp.data, expected_resp.data);
    delete handler;
}