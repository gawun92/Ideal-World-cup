# CMake generated Testfile for 
# Source directory: /usr/src/projects/gerrit-masters
# Build directory: /usr/src/projects/gerrit-masters/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/usr/src/projects/gerrit-masters/build/config_parser_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/log_handler_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/mime_type_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/request_validation_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/handle_dispatch_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/response_parser_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/html_parser_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/reverse_proxy_handler_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/health_handler_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/data_structures_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/echo_handler_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/static_handler_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/status_handler_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/make_response_test[1]_include.cmake")
include("/usr/src/projects/gerrit-masters/build/not_found_test[1]_include.cmake")
add_test(integration_test "bash" "/usr/src/projects/gerrit-masters/tests/test.sh")
add_test(reverse_proxy_integration_test "/usr/src/projects/gerrit-masters/tests/reverse_proxy_integration_test.sh" "-s" "/usr/src/projects/gerrit-masters/build/bin/server")
set_tests_properties(reverse_proxy_integration_test PROPERTIES  WORKING_DIRECTORY "/usr/src/projects/gerrit-masters/tests")
subdirs("googletest")
