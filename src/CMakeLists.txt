include_directories(../include)
include_directories(/usr/local/unixODBC/include)

include_directories(./correspondence/include)
include_directories(./connectDb/include)
include_directories(./myObjInfo/include)

aux_source_directory(. SRC_LISTS)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#通信模块
# set( CORRESPONDENCE_LISTS 
#     ./correspondence/ServerQQ.c)

#数据库模块
set( CONNECTDB 
    ./connectDb/otlUse.cpp)
    # ./connectDb/conPgsql.cpp)

#自定义对象模块(用户对象，消息对象)
set( myObjInfo 
    ./myObjInfo/user.cpp
    ./myObjInfo/msg.cpp)
# ./connectDb/conPgsql.cpp)

link_directories("/usr/local/psqlodbc/lib")

add_executable(main main.cpp ${SRC_LISTS} ${CONNECTDB} ${myObjInfo})

# target_link_libraries(main funShared)
target_link_libraries(main /usr/local/psqlodbc/lib/psqlodbca.so)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)