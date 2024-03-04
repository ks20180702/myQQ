include_directories(../include)
include_directories(/usr/local/unixODBC/include)

include_directories(./correspondence/include)
include_directories(./connectDb/include)
include_directories(./commandObj/include)
include_directories(./dataObj/include)
include_directories(./myServer/include)
include_directories(./myClient/include)

# aux_source_directory(. SRC_LISTS)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#数据模块
set( DATAOBJ 
    ./dataObj/msg.cpp
    ./dataObj/user.cpp
    ./dataObj/userNotRecvMsg.cpp)

#指令模块
set( COMMANDOBJ 
    ./commandObj/cmdCreateFactory.cpp
    ./commandObj/loginCmd.cpp
    ./commandObj/userChangeCmd.cpp
    ./commandObj/friendshipChangeCmd.cpp)

#数据库模块
set( CONNECTDB 
    ./connectDb/otlUse.cpp)
    # ./connectDb/conPgsql.cpp)

#服务器模块
set( MYSERVER 
    ./myServer/serverQQ.cpp)
#客户端模块
set( MYCLIENT
    ./myClient/clientQQ.cpp)

link_directories("/usr/local/psqlodbc/lib")

set(MAINFILE demoServer.cpp)
set(MAINNAME ser)
# set(MAINFILE main.cpp)
# set(MAINNAME main)

add_executable(${MAINNAME} ${MAINFILE} ${SRC_LISTS} ${CONNECTDB} ${MYOBJINFO} 
    ${COMMANDOBJ} ${DATAOBJ} ${MYSERVER} ${MYCLIENT})
# add_executable(main main.cpp ${SRC_LISTS} ${CONNECTDB} ${MYOBJINFO} 
#     ${COMMANDOBJ} ${DATAOBJ} ${MYSERVER} ${MYCLIENT})

target_link_libraries(${MAINNAME} /usr/local/psqlodbc/lib/psqlodbca.so)
target_link_libraries(${MAINNAME} -lpthread)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)