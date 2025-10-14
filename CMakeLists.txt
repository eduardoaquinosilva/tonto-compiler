cmake_minimum_required(VERSION 3.0.0)
project(TontoCompiler)

# Configuração do C++
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(SOURCES parser.cpp tonto-compiler.cpp)

# Configuração do Flex
find_package(FLEX)
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc COMPILE_FLAGS "-+")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR})

# Executável depende também da saída do Flex
add_executable(tonto-compiler ${SOURCES} ${FLEX_Lexer_OUTPUTS})
