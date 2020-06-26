add_test( mime_type_test.mime_types_test_1 /usr/src/projects/gerrit-masters/build/bin/mime_type_test [==[--gtest_filter=mime_type_test.mime_types_test_1]==] --gtest_also_run_disabled_tests)
set_tests_properties( mime_type_test.mime_types_test_1 PROPERTIES WORKING_DIRECTORY /usr/src/projects/gerrit-masters/tests)
set( mime_type_test_TESTS mime_type_test.mime_types_test_1)
