cmake_minimum_required(VERSION 3.16)
project(realm-cli)

find_package(cpprealm)
add_executable(realm-cli src/main.cpp)
target_link_libraries(realm-cli cpprealm)

install(TARGETS realm-cli DESTINATION bin)
