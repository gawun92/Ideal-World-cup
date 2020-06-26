add_test( handle_dispatch_test.handle_dispatch_test_1 /usr/src/projects/gerrit-masters/build/bin/handle_dispatch_test [==[--gtest_filter=handle_dispatch_test.handle_dispatch_test_1]==] --gtest_also_run_disabled_tests)
set_tests_properties( handle_dispatch_test.handle_dispatch_test_1 PROPERTIES WORKING_DIRECTORY /usr/src/projects/gerrit-masters/tests)
set( handle_dispatch_test_TESTS handle_dispatch_test.handle_dispatch_test_1)
