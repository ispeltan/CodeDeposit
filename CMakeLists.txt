cmake_minimum_required(VERSION 3.21)
project(FriendOperator)
project(SortTest)
project(HeapTest)
project(SearchTest)
project(UnionFindTest)
project(GraphTest)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.h)

add_executable(FriendOperator TestSrc/friend_oprator.cpp ${src_files})
target_include_directories(FriendOperator PRIVATE include Tools)

add_executable(SortTest TestSrc/sort_test.cpp ${src_files})
target_include_directories(SortTest PRIVATE include Tools)

add_executable(HeapTest TestSrc/heap_test.cpp ${src_files})
target_include_directories(HeapTest PRIVATE include Tools)

add_executable(SearchTest TestSrc/search_test.cpp ${src_files})
target_include_directories(SearchTest PRIVATE include Tools)

add_executable(UnionFindTest TestSrc/unionfind_test.cpp ${src_files})
target_include_directories(UnionFindTest PRIVATE include Tools)

add_executable(GraphTest TestSrc/GraphTest/graph_test.cpp ${src_files})
target_include_directories(GraphTest PRIVATE include Tools)