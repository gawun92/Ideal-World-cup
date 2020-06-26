#include "response_parser.h"
#include "gtest/gtest.h"
#include "make_response.h"

class response_parser_test_fixture : public testing::Test {
    protected:
    
    void SetUp() {
        rsp = {};
        excess = "";
    }

    std::string excess;
    response rsp;
    response_parser parser;
};

TEST_F(response_parser_test_fixture, http_response) {
    response resp;
    const std::string http = "  ";
    bool valid = parser.validate_http_version(http, resp);
    EXPECT_FALSE(valid);
}

TEST_F(response_parser_test_fixture, status_response) {
    response resp;
    const std::string status1 = "  ";
    bool valid1 = parser.validate_status_code(status1, resp);
    const std::string status2 = "AA";
    bool valid2 = parser.validate_status_code(status2, resp);
    const std::string status3 = "A  A";
    bool valid3 = parser.validate_status_code(status3, resp);
    EXPECT_FALSE(valid1);
    EXPECT_FALSE(valid2);
    EXPECT_FALSE(valid3);
}

TEST_F(response_parser_test_fixture, simple_200_response) {
    const std::string simple_response = "HTTP/1.1 200 OK\r\n\r\n";
    
    bool is_complete = parser.parse(simple_response, rsp, excess);
    EXPECT_TRUE(is_complete);
    EXPECT_EQ(rsp.status, response::status_type::ok);
    EXPECT_TRUE(rsp.header_map.empty());
    EXPECT_TRUE(rsp.data.empty());
    EXPECT_TRUE(excess.empty());
}

TEST_F(response_parser_test_fixture, html_404_response) {
    const std::string not_found_body = "<html><h1>404 Not found</h1><p>Requested file not found</p></html>";
    const std::string not_found_response = "HTTP/1.1 404 Not found\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(not_found_body.size()) + "\r\n\r\n" + not_found_body;

    bool is_complete = parser.parse(not_found_response, rsp, excess);
    EXPECT_TRUE(is_complete);
    EXPECT_EQ(rsp.status, response::status_type::not_found);
    EXPECT_TRUE(rsp.header_map.find("Content-Length") != rsp.header_map.end() && rsp.header_map.at("Content-Length") == std::to_string(not_found_body.size()));
    EXPECT_TRUE(rsp.header_map.find("Content-Type") != rsp.header_map.end() && rsp.header_map.at("Content-Type") == "text/html");
    EXPECT_EQ(rsp.header_map.size(), 2);
    EXPECT_EQ(rsp.data, not_found_body);
    EXPECT_TRUE(excess.empty());

}

TEST_F(response_parser_test_fixture, incomplete_response) {
    const std::string incomplete_response = "HTTP/1.1 200 OK";

    bool is_complete = parser.parse(incomplete_response, rsp, excess);
    EXPECT_FALSE(is_complete);
}

TEST_F(response_parser_test_fixture, incomplete_body) {
    const std::string incomplete_body = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\nContent-Type: text/plain\r\n\r\n";

    bool is_complete = parser.parse(incomplete_body, rsp, excess);
    EXPECT_FALSE(is_complete);
    EXPECT_EQ(rsp.status, response::status_type::ok);
    EXPECT_TRUE(rsp.header_map.find("Content-Length") != rsp.header_map.end() && rsp.header_map.at("Content-Length") == "2");
    EXPECT_TRUE(rsp.header_map.find("Content-Type") != rsp.header_map.end() && rsp.header_map.at("Content-Type") == "text/plain");
    EXPECT_EQ(rsp.data, "");
}

TEST_F(response_parser_test_fixture, response_has_excess) {
    const std::string response_complete = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\nContent-Type: text/plain\r\n\r\n";
    const std::string response_excess = "hello world!";

    bool is_complete = parser.parse(response_complete + response_excess, rsp, excess);
    EXPECT_TRUE(is_complete);
    EXPECT_EQ(rsp.status, response::status_type::ok);
    EXPECT_EQ(rsp.data, response_excess.substr(0, 2));
    EXPECT_TRUE(rsp.header_map.find("Content-Length") != rsp.header_map.end() && rsp.header_map.at("Content-Length") == "2");
    EXPECT_TRUE(rsp.header_map.find("Content-Type") != rsp.header_map.end() && rsp.header_map.at("Content-Type") == "text/plain");
    EXPECT_EQ(excess, response_excess.substr(2));
}

TEST_F(response_parser_test_fixture, response_no_content_length_but_excess) {
    const std::string response_complete = "HTTP/1.1 200 OK\r\n\r\n";
    const std::string response_excess = "hello world!";

    bool is_complete = parser.parse(response_complete + response_excess, rsp, excess);
    EXPECT_TRUE(is_complete);
    EXPECT_EQ(rsp.status, response::status_type::ok);
    EXPECT_EQ(rsp.data, response_excess);
    EXPECT_EQ(excess, "");
}
TEST_F(response_parser_test_fixture, find_headers_with_no_headers) {
    const std::string headers = "";
    parser.find_headers(headers, rsp);
    EXPECT_TRUE(rsp.header_map.empty());
}

TEST_F(response_parser_test_fixture, find_headers_with_headers) {
    const std::string headers = "Content-Type: text/html\r\nContent-Length: 302";
    
    parser.find_headers(headers, rsp);

    for (auto header : rsp.header_map) {
        std::cout << header.first << std::endl;
        std::cout << header.second << std::endl;
    }

    EXPECT_EQ(rsp.header_map.size(), 2);
    EXPECT_TRUE(rsp.header_map.find("Content-Type") != rsp.header_map.end() && rsp.header_map.at("Content-Type") == "text/html");
    EXPECT_TRUE(rsp.header_map.find("Content-Length") != rsp.header_map.end() && rsp.header_map.at("Content-Length") == "302");
}


TEST_F(response_parser_test_fixture, status_code_check_200) {
    const std::string status_line = "HTTP/1.1 200 OK";
    
    bool is_valid = parser.validate_status_code(status_line, rsp);

    EXPECT_TRUE(is_valid);
    EXPECT_EQ(rsp.status, response::ok);
}

TEST_F(response_parser_test_fixture, status_code_check_404) {
    const std::string status_line = "HTTP/1.1 404 Not found";
    
    bool is_valid = parser.validate_status_code(status_line, rsp);

    EXPECT_TRUE(is_valid);
    EXPECT_EQ(rsp.status, response::not_found);
}

TEST_F(response_parser_test_fixture, letters_in_status_code_check) {
    const std::string status_line = "HTTP/1.1 2we00 OK";
    
    bool is_valid = parser.validate_status_code(status_line, rsp);

    EXPECT_FALSE(is_valid);
}

TEST_F(response_parser_test_fixture, large_number_status_code_check) {
    const std::string status_line = "HTTP/1.1 20290 OK";
    
    bool is_valid = parser.validate_status_code(status_line, rsp);

    EXPECT_FALSE(is_valid);
}

TEST_F(response_parser_test_fixture, negative_number_status_code_check) {
    const std::string status_line = "HTTP/1.1 -200 OK";
    
    bool is_valid = parser.validate_status_code(status_line, rsp);

    EXPECT_FALSE(is_valid);
}

TEST_F(response_parser_test_fixture, valid_http_version) {
    const std::string status_line = "HTTP/1.1 200 OK";

    bool is_valid = parser.validate_http_version(status_line, rsp);

    EXPECT_TRUE(is_valid);
}

TEST_F(response_parser_test_fixture, valid_http_2_0) {
    const std::string status_line = "HTTP/2.0 200 OK";

    bool is_valid = parser.validate_http_version(status_line, rsp);

    EXPECT_TRUE(is_valid);
}

TEST_F(response_parser_test_fixture, bad_http_version) {
    const std::string status_line = "HTTP/1.ba 200 OK";

    bool is_valid = parser.validate_http_version(status_line, rsp);

    EXPECT_FALSE(is_valid);
}

TEST_F(response_parser_test_fixture, valid_headers) {
    rsp.header_map = { {"Content-Type", "text/html"}, {"Content-Length", "302"}};
    
    bool is_valid = parser.validate_headers(rsp);

    EXPECT_TRUE(is_valid);
}

TEST_F(response_parser_test_fixture, negative_content_length_header) {
    rsp.header_map = { {"Content-Length", "-302"}};
    
    bool is_valid = parser.validate_headers(rsp);

    EXPECT_FALSE(is_valid);
}

TEST_F(response_parser_test_fixture, non_numeric_length_in_header) {
    rsp.header_map = { {"Content-Length", "3asdf02"}};
    
    bool is_valid = parser.validate_headers(rsp);

    EXPECT_FALSE(is_valid);
}