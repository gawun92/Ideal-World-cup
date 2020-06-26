#!/bin/bash

#Run 2 servers and see that request outputs are same for both servers.

TARGET_FILE=../build/bin/server

if [ $# -eq 2 ];
then
  case $1 in
    -s)
      TARGET_FILE=$2
      ;;
    *) 
      echo "USAGE: integration_test.sh [-s server]"
      echo "Bad Command Line Argument: $1 $2"
      exit 1
      ;;
  
  esac

else
  if [ $# -ne 0 ];
  then
    echo "USAGE: integration_test.sh [-s server]"
    echo "Illegal number of arguments: $#"
    exit 1
  fi
fi

#Run 2 servers in the background
$TARGET_FILE test_configs/test_config_reverse &

server_1_pid=$!


$TARGET_FILE test_configs/test_config_reverse_2 &

server_2_pid=$!

#Exit code
code=0

sleep 1

#Static test

curl -S localhost:8080/reverse/static/index.html -o "reverse_static.txt"
curl -S localhost:8081/static/index.html -o "static.txt"


diff reverse_static.txt static.txt

test_failure=$?

if [ $test_failure -eq 0 ]; 
then
    echo "Reverse Proxy Static test success"
else
    echo "Reverse Proxy Static test failure"
    code=1
fi


rm "reverse_static.txt" "static.txt"

kill -INT $server_1_pid
kill -INT $server_2_pid

if [ $code -eq 0 ];
then
    echo "All tests passed"
else
    echo "Some tests failed"
fi

exit $code
