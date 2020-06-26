// validation test
#include "gtest/gtest.h"
#include "request_validation.h"

class ValidationTest : public ::testing::Test {
protected:
   std::string test_string;
   std::string test_result;
   std::string expected_result;
};

TEST_F(ValidationTest, valid_test_get) {
    test_string = 
"GET /hello HTTP/1.1\nHost: 35.230.94.158\nUpgrade-Insecure-Requests: 1\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1 Safari/605.1.15\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: keep-alive\n\n";
    test_result = request_validation(test_string);

    expected_result = 
"HTTP/1.1 200 OK\nContent-Type: plain/text\nContent-Length: 349\n\nGET /hello HTTP/1.1\nHost: 35.230.94.158\nUpgrade-Insecure-Requests: 1\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1 Safari/605.1.15\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: keep-alive\n\n";

    EXPECT_TRUE(test_result == expected_result);
}

TEST_F(ValidationTest, invalid_test_form) {
    test_string = "Hi\n\n";
    test_result = request_validation(test_string);

    expected_result = "400 Bad Request\n\nHi\n\n";
    EXPECT_TRUE(test_result == expected_result);
}

TEST_F(ValidationTest, invalid_test_empty) {
    test_string = "\n\n";
    test_result = request_validation(test_string);

    expected_result = "400 Bad Request\n\n\n\n";
    EXPECT_TRUE(test_result == expected_result);
}

TEST_F(ValidationTest, invalid_test_header) {
    test_string = "GET /docs/index.html\nHost: www.nowhere123.com\n\n";
    test_result = request_validation(test_string);

    expected_result = "400 Bad Request\n\nGET /docs/index.html\nHost: www.nowhere123.com\n\n";
    EXPECT_TRUE(test_result == expected_result);


    test_string = "GET /docs/index.html HTTP/1.1\nHost:\n\n";
    test_result = request_validation(test_string);

    expected_result = "400 Bad Request\n\nGET /docs/index.html HTTP/1.1\nHost:\n\n";
    EXPECT_TRUE(test_result == expected_result);
}