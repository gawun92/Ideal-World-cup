# !/bin/bash

echo "http {
    # this is a comment
    server {
        port 8080;
        
        location \"/echo\" EchoHandler {
            root \"../data/echo_data\";
        }

        location \"/static\" StaticHandler{
            root \"../data/static_data\";
        }    

        location \"/status\" StatusHandler{
        }

        location \"/ucla\" ReverseProxyHandler{
            host \"www.ucla.edu\";
            host_port 80;
        }
        
        location \"/columbia\" ReverseProxyHandler{
            host \"www.columbia.edu\";
            host_port 80;
        }

        location \"/health\" HealthHandler{
        }

        location \"/itwc\" ITWCHandler{
            root \"./tests/itwc_test/data/ITWC\";
        }
    }
}" > test_config

$"bin/server" test_config &

SERVER_PID=$!
echo $SERVER_PID

sleep 1 

curl http://localhost:8080/wedonthavethis/file.file -i -o test_output -s
cat test_output

diff $"../tests/test_integration/integration_test_1" test_output

if [[ $? -eq 0 ]]; then
    echo "SUCCESS";
else
    echo "FAIL";
    kill -INT $SERVER_PID
    exit 1;
fi

curl http://localhost:8080/static/
if [[ $? -eq 0 ]]; then
    echo "SUCCESS";
else
    echo "FAIL";
    kill -INT $SERVER_PID
    exit 1;
fi

curl http://localhost:8080/echo/
if [[ $? -eq 0 ]]; then
    echo "SUCCESS";
else
    echo "FAIL";
    kill -INT $SERVER_PID
    exit 1;
fi

curl http://localhost:8080/echo/test.jpg
if [[ $? -eq 0 ]]; then
    echo "SUCCESS";
else
    echo "FAIL";
    kill -INT $SERVER_PID
    exit 1;
fi

req1=$'GET /echo/ HTTP/1.1'
echo "$req1" | nc localhost 8080 & 
REQ_PID_1=$!
kill -INT $REQ_PID_1
sleep 1

curl http://localhost:8080/static/
if [[ $? -eq 0 ]]; then
    echo "SUCCESS";
else
    echo "FAIL";
    kill -INT $SERVER_PID
    exit 1;
fi
kill -INT $SERVER_PID
