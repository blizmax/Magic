cmake_minimum_required(VERSION 3.0)
project(Magic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(Library
    Include
    Include/Magic
    ThirdParty/Gzip                 #Gzip
    ThirdParty/RapidJSON/include    #RapidJSON
    ThirdParty/Asio/asio/include    #ASIO
)
include(Library.cmake)
include_directories(${Library})

#GZip
add_subdirectory(ThirdParty/Gzip)

set(SRC
#   Util
    Source/Magic.cpp
	Source/Magic/Log.cpp
    Source/Magic/Gzip.cpp
    Source/Magic/Timer.cpp
    Source/Magic/Crypto.cpp
	Source/Magic/Thread.cpp
	Source/Magic/Config.cpp
	Source/Magic/Plugin.cpp
    Source/Magic/IoPool.cpp
    Source/Magic/Socket.cpp
    Source/Magic/TcpServer.cpp
    Source/Magic/TimingWheel.cpp
#   DataBase
    Source/Magic/DB/MySql.cpp
#   Http
    Source/Magic/Http/Http.cpp
    Source/Magic/Http/Session.cpp
    Source/Magic/Http/HttpFile.cpp
    Source/Magic/Http/MultiPart.cpp
    Source/Magic/Http/HttpCache.cpp
    Source/Magic/Http/HttpSocket.cpp
    Source/Magic/Http/HttpServer.cpp
    Source/Magic/Http/HttpParser.cpp
    Source/Magic/Http/HttpServlet.cpp
    Source/Magic/Http/WebSocketMessage.cpp
    Source/Magic/Http/WebSocket.cpp
    Source/Magic/Http/WebSocketServer.cpp
#   Adapter
    Source/Magic/Adapter/WinUtil.cpp
    Source/Magic/Adapter/WinMutex.cpp
    Source/Magic/Adapter/WinPlugin.cpp
    Source/Magic/Adapter/LinuxUtil.cpp
    Source/Magic/Adapter/LinuxMutex.cpp
	Source/Magic/Adapter/LinuxPlugin.cpp
#   Ragel
    Source/Magic/Http/Util/Uri.rl.cpp
    Source/Magic/Http/Util/http11_parser.rl.cpp
    Source/Magic/Http/Util/httpclient_parser.rl.cpp
)

set(SRCS
    Build.cpp
)

add_library(Magic ${SRC})
target_link_libraries(Magic zlib)

#Test
    add_executable(Test Test/Test.cc)
    target_link_libraries(Test Magic)

#Examples-Plugin
    add_library(Test_PluginLib MODULE Examples/Test_PluginLib.cpp)
    add_executable(Test_Plugin Examples/Test_Plugin.cpp)
    target_link_libraries(Test_Plugin Magic)

#Examples-Server
    add_executable(Test_Server Examples/Test_Server.cpp)
    target_link_libraries(Test_Server Magic)