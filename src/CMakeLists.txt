project(MyMVC)
add_library(anarionmvc
        http/Request.cpp
        http/Response.cpp
        http/HttpApplet.cpp
        http/HttpDispatcher.cpp
        http/HttpChannel.cpp
        http/StaticHandler.cpp
        http/Cookie.cpp
        http/ErrorHandler.cpp
        http/HttpContext.cpp
        http/RequestRouter.cpp

        listener/Listener.cpp
        listener/StreamListener.cpp
        listener/TcpPortListener.cpp
        listener/TcpLoggerInfo.cpp

        testapp/HelloApplet.cpp

        parser/MapParser.cpp

        resource/Payload.cpp
        resource/FilePayload.cpp
#        resource/HtmlPayload.cpp
#        resource/CssPayload.cpp
#        resource/JsPayload.cpp
        resource/CompressedPayload.cpp
        resource/TextPayload.cpp
        resource/LRUPayloadManager.cpp
        resource/JsonPayload.cpp
        resource/StaticResources.cpp
        resource/WebConfigResources.cpp

        service/BackgroundService.cpp

        logger/MvcLogger.cpp

        controller/Controller.cpp

        )

add_library(anarionbase STATIC IMPORTED)
set_target_properties(anarionbase PROPERTIES IMPORTED_LOCATION ../lib/libanarionbase.a)
target_link_libraries(anarionmvc anarionbase pthread z)

#include_directories(../../MyCppBase/include)
include_directories(../include)
include_directories(../include/base)