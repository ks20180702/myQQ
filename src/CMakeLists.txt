include_directories(../include)
include_directories(/usr/local/unixODBC/include)

include_directories(./connectDb/include)
include_directories(./commandObj/include)
include_directories(./dataObj/include)
include_directories(./myServer/include)

# aux_source_directory(. SRC_LISTS)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#数据模块
set( DATAOBJ 
    ./dataObj/msg.cpp
    ./dataObj/user.cpp)

#指令模块
set( COMMANDOBJ 
    ./commandObj/cmdCreateFactory.cpp
    ./commandObj/loginCmd.cpp
    ./commandObj/dataMsgCmd.cpp
    ./commandObj/userChangeCmd.cpp
    ./commandObj/heartRequestCmd.cpp
    ./commandObj/heartMsgCmd.cpp
    ./commandObj/friendshipChangeCmd.cpp)

#数据库模块
set( CONNECTDB 
    ./connectDb/otlUse.cpp)
    # ./connectDb/conPgsql.cpp)

#服务器模块
set( MYSERVER 
    ./myServer/serverQQ.cpp)

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

add_compile_options("< <<<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("< <<<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
# add_compile_options(" /utf-8 ")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)