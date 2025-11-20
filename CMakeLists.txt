cmake_minimum_required(VERSION 3.0.0)
project(TontoCompiler)

# Configuração do C++
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(SOURCES parser.cpp tonto-compiler.cpp)

# Configuração do Flex
find_package(FLEX)
find_package(BISON)

FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc COMPILE_FLAGS "-+")
BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/parser.h")
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCES tonto-compiler.cpp ast.cpp)

# Executável depende também da saída do Flex e do Bison
add_executable(tonto-compiler ${SOURCES} ${FLEX_Lexer_OUTPUTS} ${BISON_Parser_OUTPUTS})
