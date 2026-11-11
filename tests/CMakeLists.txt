add_executable(vitalcomm_tests
    test_crc32.cpp
)
target_link_libraries(vitalcomm_tests PRIVATE
    vitalcomm vitalcomm_warnings GTest::gtest_main GTest::gmock
)
if(VITALCOMM_SANITIZE)
    target_link_libraries(vitalcomm_tests PRIVATE vitalcomm_sanitize)
endif()

include(GoogleTest)
gtest_discover_tests(vitalcomm_tests)
