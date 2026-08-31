# Top level script

# decide version (>3.5) + must be a nix package
cmake_minimum_required(VERSION 3.2.0) 

# makes sure compilers and linkers work
# semantic versioning to enforce version numbers?
project(
    App
    LANGUAGES C
)

# create executable from set of source files
add_executable(MyProg
  platform/tests/Src/adc_test.c
)

# can add libraries, static, shared, modules
# -> static libraries on unix get compiled to .a 
# -> module libraries get linked dynamically at runtime

# most of my dependencies will probably be public

# ctest, can use to run multiple tests, not sure how this would work w hw tho
# add_test()

# cpack, can pack for releases