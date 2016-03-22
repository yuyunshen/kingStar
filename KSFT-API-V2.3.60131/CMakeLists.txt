cmake_minimum_required(VERSION 3.2)
project(KSFT_API_V2_3_60131)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    sample/KSMarketDataAPI_Sample/MdUserApi_Sample.cpp
    sample/KSTradeAPI_Sample/TraderApi_Sample.cpp
    sdk/inc/KSMarketDataAPI.h
    sdk/inc/KSTradeAPI.h
    sdk/IncEx/KSCosApi.h
    sdk/IncEx/KSMdApiEx.h
    sdk/IncEx/KSOptionApi.h
    sdk/IncEx/KSTraderApiEx.h
    sdk/IncEx/KSUserApiDataTypeEx.h
    sdk/IncEx/KSUserApiStructEx.h
    sdk/IncEx/KSVocApi.h
    sdk/IncEx/KSVocApiDataType.h
    sdk/IncEx/KSVocApiStruct.h
    sdk/IncEx/KSVocMdApi.h)

add_executable(KSFT_API_V2_3_60131 ${SOURCE_FILES})