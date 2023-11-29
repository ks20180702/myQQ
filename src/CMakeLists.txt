include_directories(../include)
include_directories(/usr/local/unixODBC/include)

include_directories(./correspondence/include)
include_directories(./connectDb/include)
include_directories(./myObjInfo/include)
include_directories(./commandObj/include)

aux_source_directory(. SRC_LISTS)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#指令模块
set( COMMANDOBJ 
    ./commandObj/loginCmd.cpp
    ./commandObj/userChangeCmd.cpp)

#数据库模块
set( CONNECTDB 
    ./connectDb/otlUse.cpp)
    # ./connectDb/conPgsql.cpp)

#自定义对象模块(用户对象，消息对象)
set( MYOBJINFO 
    ./myObjInfo/user.cpp
    ./myObjInfo/msg.cpp)

#通信模块
# set( CORRESPONDENCE_LISTS 
#     ./correspondence/ServerQQ.c)


link_directories("/usr/local/psqlodbc/lib")

add_executable(main main.cpp ${SRC_LISTS} ${CONNECTDB} ${MYOBJINFO} ${COMMANDOBJ})

target_link_libraries(main /usr/local/psqlodbc/lib/psqlodbca.so)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)