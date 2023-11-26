include_directories(../include)
include_directories(/usr/local/unixODBC/include)

include_directories(./correspondence/include)
include_directories(./connectDb/include)
include_directories(./userInfo/include)

aux_source_directory(. SRC_LISTS)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#通信模块
# set( CORRESPONDENCE_LISTS 
#     ./correspondence/ServerQQ.c)

#数据库模块
set( CONNECTDB 
    ./connectDb/otlUse.cpp)
    # ./connectDb/conPgsql.cpp)

#用户对象模块
set( USERINFO 
    ./userInfo/user.cpp)
# ./connectDb/conPgsql.cpp)

link_directories("/usr/local/psqlodbc/lib")

# add_executable(main  ${CONNECTDB})
add_executable(main main.cpp ${SRC_LISTS} ${CONNECTDB} ${USERINFO})

target_link_libraries(main funShared)
target_link_libraries(main /usr/local/psqlodbc/lib/psqlodbca.so)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)