add_test( health_handler_tester.health_return_test /usr/src/projects/gerrit-masters/build/bin/health_handler_test [==[--gtest_filter=health_handler_tester.health_return_test]==] --gtest_also_run_disabled_tests)
set_tests_properties( health_handler_tester.health_return_test PROPERTIES WORKING_DIRECTORY /usr/src/projects/gerrit-masters/tests/)
set( health_handler_test_TESTS health_handler_tester.health_return_test)
