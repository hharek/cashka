cmake_minimum_required(VERSION 3.7)
project(cashka)

set(CMAKE_CXX_STANDARD 11)

set(cashka_SOURCE_FILES
		cashka/main.cpp
		cashka/cashka.cpp
		cashka/cashka.h
		cashka/options.h
		cashka/options.cpp
		cashka/options_check.cpp
		cashka/options_cli.cpp
		cashka/options_config.cpp
		cashka/pm.cpp
		cashka/pm.h
		cashka/server.cpp
		cashka/server.h
        protocol/protocol.h
        protocol/protocol.cpp
		protocol/hello.h
		protocol/hello.cpp)

add_executable(cashka ${cashka_SOURCE_FILES})

set(cashka_cli_SOURCE_FILES
		cashka-cli/main.cpp
		cashka-cli/cashka-cli.cpp
		cashka-cli/cashka-cli.h
		cashka-cli/client.cpp
		cashka-cli/client.h
		cashka-cli/options.h
		cashka-cli/options.cpp
		cashka-cli/options_check.cpp
		cashka-cli/options_cli.cpp
		cashka-cli/options_config.cpp
        protocol/protocol.h
        protocol/protocol.cpp
		protocol/hello.h
		protocol/hello.cpp)

add_executable(cashka-cli ${cashka_cli_SOURCE_FILES})
