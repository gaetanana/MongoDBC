cmake_minimum_required(VERSION 3.17)
project(MongoDBC C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_GENERATOR_PLATFORM x32)

# Include des directories C
include_directories("${CMAKE_SOURCE_DIR}/Dependances/DriverMongoDB/mongo-c-driver-1.22.1/mongo-c-driver-1.22.1/build/install/include/libmongoc-1.0")
include_directories("${CMAKE_SOURCE_DIR}/Dependances/DriverMongoDB/mongo-c-driver-1.22.1/mongo-c-driver-1.22.1/build/install/include/libbson-1.0")
include_directories("${CMAKE_SOURCE_DIR}/Dependances/DriverXML/include")
include_directories("${CMAKE_SOURCE_DIR}/Dependances/Jansson/jansson/build/install/include")
include_directories("${CMAKE_SOURCE_DIR}/Dependances/dirent/dirent/include")
include_directories("${CMAKE_SOURCE_DIR}/Dependances/uthash/src")

add_executable(MongoDBC main.c)

link_directories("${CMAKE_SOURCE_DIR}/Dependances/DriverMongoDB/mongo-c-driver-1.22.1/mongo-c-driver-1.22.1/build/install/lib")
link_directories("${CMAKE_SOURCE_DIR}/Dependances/DriverXML/lib")
link_directories("${CMAKE_SOURCE_DIR}/Dependances/Jansson/jansson/build/install/lib/Release")

# Link libraries
target_link_libraries(MongoDBC "${CMAKE_SOURCE_DIR}/Dependances/DriverMongoDB/mongo-c-driver-1.22.1/mongo-c-driver-1.22.1/build/install/lib/bson-1.0.lib" "${CMAKE_SOURCE_DIR}/Dependances/DriverMongoDB/mongo-c-driver-1.22.1/mongo-c-driver-1.22.1/build/install/lib/mongoc-1.0.lib")

target_link_libraries(MongoDBC "${CMAKE_SOURCE_DIR}/Dependances/DriverXML/lib/libxml2.lib")
target_link_libraries(MongoDBC "${CMAKE_SOURCE_DIR}/Dependances/Jansson/jansson/build/install/lib/Release/jansson.lib")
