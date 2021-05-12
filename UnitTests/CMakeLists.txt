cmake_minimum_required(VERSION 3.14)
project(VU_OP_Uzd_3) 

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests 
        tests.cpp) # add this executable

target_link_libraries(tests PRIVATE gtest) # link google test to this executable
