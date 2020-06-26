#include "reverse_proxy_handler.h"
#include "config_parser.h"
#include "gtest/gtest.h"
#include "boost_networking_injector.h"


//Tests the config_check function defined in reverse_proxy_handler

class config_check_test : public ::testing::Test {
protected:
    NginxConfig config;

    std::shared_ptr<NginxConfigStatement> test_statement_root;
    std::shared_ptr<NginxConfigStatement> test_statement_host;
    std::shared_ptr<NginxConfigStatement> test_statement_port;

    std::shared_ptr<NginxConfigStatement> test_statement_poor_format;
    std::shared_ptr<NginxConfigStatement> test_statement_bad_host_1;
    std::shared_ptr<NginxConfigStatement> test_statement_bad_host_2;
    std::shared_ptr<NginxConfigStatement> test_statement_bad_host_3;
    std::shared_ptr<NginxConfigStatement> test_statement_bad_host_4;
    std::shared_ptr<NginxConfigStatement> test_statement_bad_port_1;
    std::shared_ptr<NginxConfigStatement> test_statement_bad_port_2;
    std::shared_ptr<NginxConfigStatement> test_statement_bad_port_3;

    std::string host;
    int port;

    void SetUp() override{

        test_statement_root = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_root->tokens_.push_back("root");
        test_statement_root->tokens_.push_back("\"./www/data\"");

        test_statement_host = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_host->tokens_.push_back("host");
        test_statement_host->tokens_.push_back("\"http:/www.ucla.edu\"");

        test_statement_port = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_port->tokens_.push_back("host_port");
        test_statement_port->tokens_.push_back("8000");

        test_statement_poor_format = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_poor_format->tokens_.push_back("host_port");

        test_statement_bad_host_1 = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_bad_host_1->tokens_.push_back("host");
        test_statement_bad_host_1->tokens_.push_back("");

        test_statement_bad_host_2 = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_bad_host_2->tokens_.push_back("host");
        test_statement_bad_host_2->tokens_.push_back("\"\"");

        test_statement_bad_host_3 = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_bad_host_3->tokens_.push_back("host");
        test_statement_bad_host_3->tokens_.push_back("\"aaa");

        test_statement_bad_host_4 = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_bad_host_4->tokens_.push_back("host");
        test_statement_bad_host_4->tokens_.push_back("aaa\"");

        test_statement_bad_port_1 = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_bad_port_1->tokens_.push_back("host_port");
        test_statement_bad_port_1->tokens_.push_back("a1b2c3");

        test_statement_bad_port_2 = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_bad_port_2->tokens_.push_back("host_port");
        test_statement_bad_port_2->tokens_.push_back("-134598");

        test_statement_bad_port_3 = std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement);
        test_statement_bad_port_3->tokens_.push_back("host_port");
        test_statement_bad_port_3->tokens_.push_back("0");


    }
};


TEST_F(config_check_test, StandardReverseProxyConfig){
    config.statements_.push_back(test_statement_host);
    config.statements_.push_back(test_statement_port);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_TRUE(success);
    EXPECT_EQ(host, "http:/www.ucla.edu");
    EXPECT_EQ(port, 8000);

}

TEST_F(config_check_test, EmptyConfig){
    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);
}


TEST_F(config_check_test, TooManyStatements){
    config.statements_.push_back(test_statement_host);
    config.statements_.push_back(test_statement_host);
    config.statements_.push_back(test_statement_port);
    config.statements_.push_back(test_statement_port);


    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, IllegalStatementType){

    config.statements_.push_back(test_statement_port);
    config.statements_.push_back(test_statement_root);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, IllegalStatementFormat){

    config.statements_.push_back(test_statement_poor_format);
    config.statements_.push_back(test_statement_root);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, BadHostFormat1){

    config.statements_.push_back(test_statement_port);
    config.statements_.push_back(test_statement_bad_host_1);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, BadHostFormat2){

    config.statements_.push_back(test_statement_port);
    config.statements_.push_back(test_statement_bad_host_2);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, BadHostFormat3){

    config.statements_.push_back(test_statement_port);
    config.statements_.push_back(test_statement_bad_host_3);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, BadHostFormat4){

    config.statements_.push_back(test_statement_port);
    config.statements_.push_back(test_statement_bad_host_4);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, BadPortFormat1){

    config.statements_.push_back(test_statement_host);
    config.statements_.push_back(test_statement_bad_port_1);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, BadPortFormat2){

    config.statements_.push_back(test_statement_host);
    config.statements_.push_back(test_statement_bad_port_2);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, BadPortFormat3){

    config.statements_.push_back(test_statement_host);
    config.statements_.push_back(test_statement_bad_port_3);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, NoHost){

    config.statements_.push_back(test_statement_port);
    config.statements_.push_back(test_statement_port);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

TEST_F(config_check_test, NoPort){

    config.statements_.push_back(test_statement_host);
    config.statements_.push_back(test_statement_host);

    bool success = reverse_proxy_handler::config_check(&config, host, port);

    EXPECT_FALSE(success);

}

std::string url;	
std::string proxy_content;
int count = 0;	
response resp;

class reverse_proxy_test_fixture : public testing::Test {
	
protected:
    request req;
    NginxConfigParser parser;
    NginxConfig config;
	
    std::shared_ptr<boost_networking_injector> mock_ptr;
    std::shared_ptr<boost_networking_injector> redirect_ptr;
	
    class mock_injector : public boost_networking_injector {
        virtual void connect(std::string host, std::string port) const {
            url = host + ":" + port;
        }
	
        virtual void communicate(std::string request_string, response& resp, std::string host, std::string port) const {
            resp.status = response::status_type::ok;
            resp.data = proxy_content;
            resp.header_map.insert({"Content-Length", std::to_string(resp.data.size())});
            resp.header_map.insert({"Content-Type", "text/html"});
        }
	
    };
	
    class redirect_injector : public boost_networking_injector {
        virtual void connect(std::string host, std::string port) const {
            url += host + ":" + port + "\n";
        }

        virtual void communicate(std::string request_string, response& resp, std::string host, std::string port) const {
            if (count == 0) {
                resp.status = response::moved_permanently;
                resp.data = "null";
                resp.header_map.insert({"Content-Length", std::to_string(resp.data.size())});
                resp.header_map.insert({"Content-Type", "text/html"});
                resp.header_map.insert({"Location", "foo:8001"});
                count++;
	
            }
            else {
                resp.status = response::status_type::ok;
                resp.data = proxy_content;
                resp.header_map.insert({"Content-Length", std::to_string(resp.data.size())});
                resp.header_map.insert({"Content-Type", "text/html"});
            }
	
        }
	
    };
	
    void SetUp() override {
        req.method = "GET";
        req.uri = "/ucla/";
        req.http_version_major = 1;
        req.http_version_minor = 1;
        req.ip = "127.0.0.1";
        req.raw = "GET /ucla HTTP/1.1\r\n\r\n";
        req.headers_map.insert({"Content-Length", "0"});
        req.headers_map.insert({"Host", "http://foo"});
        parser.Parse("config_reverse", &config);

        url = "";
        count = 0;
	
        mock_ptr = std::make_shared<mock_injector>();
        redirect_ptr = std::make_shared<redirect_injector>();
	
    }
	
    void test(std::string expected_url, response expected_resp) {
        EXPECT_EQ(expected_url, url);
        EXPECT_EQ(expected_resp.status, resp.status);
        EXPECT_EQ(expected_resp.data, resp.data);
	
    }
	
};
	
TEST_F(reverse_proxy_test_fixture, HelloProxy) {
    proxy_content = "Hello World!";

    auto handler = new reverse_proxy_handler(std::string("/ucla"), config, "127.0.0.1", 80);
	
    resp = handler->do_handle(req, mock_ptr);
	
    response expected;
    expected.status = response::status_type::ok;
    expected.data = "Hello World!";
	
    test("127.0.0.1:80", expected);
    delete handler;
}
	
TEST_F(reverse_proxy_test_fixture, ReplacedURL) {
    proxy_content = "<a href=\"/foo\"></a>";
	
    auto handler = new reverse_proxy_handler(std::string("/ucla"), config, "127.0.0.1", 80);
	
    resp = handler->do_handle(req, mock_ptr);
	
    response expected;
    expected.status = response::status_type::ok;
    expected.data = "<a href=\"/ucla/foo\"></a>";
	
    test("127.0.0.1:80", expected);
    delete handler;
}
	
TEST_F(reverse_proxy_test_fixture, Redirect) {
    proxy_content = "I've redirected!";
	
    auto handler = new reverse_proxy_handler(std::string("/ucla"), config, "http://foo", 80);
	
    resp = handler->do_handle(req, redirect_ptr);
	
    response expected;
    expected.status = response::status_type::ok;
    expected.data = "I've redirected!";
	
    test("http://foo:80\nhttp://foo:8001\n", expected);
    delete handler;
}
