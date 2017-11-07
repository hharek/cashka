##### Default #####
CXX = g++
CXXFLAGS = -Wall

all: build/cashka build/cashka-cli

clean:
	rm -rf obj
	rm -f build/cashka
	rm -f build/cashka-cli

debug: CXXFLAGS += -g
debug: build/cashka build/cashka-cli

##### Dir #####
obj:
	mkdir -p obj
	mkdir -p obj/cashka
	mkdir -p obj/cashka-cli

##### cashka #####
build/cashka: 	obj \
				cashka/main.cpp \
				cashka/cashka.h \
				cashka/options.h \
				cashka/pm.h \
				cashka/server.h \
				obj/cashka/cashka.o \
				obj/cashka/options.o \
				obj/cashka/options_check.o \
				obj/cashka/options_cli.o \
				obj/cashka/options_config.o \
				obj/cashka/pm.o \
				obj/cashka/server.o \
				build/cashka.json
	${CXX} ${CXXFLAGS} -o build/cashka \
						obj/cashka/cashka.o \
						obj/cashka/options.o \
						obj/cashka/options_check.o \
						obj/cashka/options_cli.o \
						obj/cashka/options_config.o \
						obj/cashka/pm.o \
						obj/cashka/server.o \
						cashka/main.cpp

# Common functions
obj/cashka/cashka.o: cashka/cashka.h cashka/cashka.cpp
	${CXX} ${CXXFLAGS} -c cashka/cashka.cpp -o obj/cashka/cashka.o

# Options
obj/cashka/options.o: cashka/options.h cashka/options.cpp
	${CXX} ${CXXFLAGS} -c cashka/options.cpp -o obj/cashka/options.o

obj/cashka/options_check.o: cashka/options.h cashka/options_check.cpp
	${CXX} ${CXXFLAGS} -c cashka/options_check.cpp -o obj/cashka/options_check.o

obj/cashka/options_cli.o: cashka/options.h cashka/options_cli.cpp
	${CXX} ${CXXFLAGS} -c cashka/options_cli.cpp -o obj/cashka/options_cli.o

obj/cashka/options_config.o: cashka/options.h cashka/options_config.cpp
	${CXX} ${CXXFLAGS} -c cashka/options_config.cpp -o obj/cashka/options_config.o

# Process
obj/cashka/pm.o: cashka/pm.h cashka/options.h cashka/pm.cpp
	${CXX} ${CXXFLAGS} -c cashka/pm.cpp -o obj/cashka/pm.o

# Server
obj/cashka/server.o: cashka/pm.h cashka/server.h cashka/options.h cashka/server.cpp
	${CXX} ${CXXFLAGS} -c cashka/server.cpp -o obj/cashka/server.o

# Config
build/cashka.json:
	cp cashka/cashka.json build/cashka.json

##### cashka-cli #####
build/cashka-cli: 	obj \
					cashka-cli/main.cpp
	${CXX} ${CXXFLAGS} -o build/cashka-cli \
						cashka-cli/main.cpp
