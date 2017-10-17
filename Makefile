# Default
all: cashka

# Clean
clean:
	rm -f lib/*
	rm -f cashka cashka.pid

# Cashka
cashka: main.cpp \
		inc/cashka.h \
		lib/cashka.o \
		inc/options.h \
		lib/options.o \
		lib/options_check.o \
		lib/options_cli.o \
		lib/options_config.o \
		inc/pm.h \
		lib/pm.o
	g++ -Wall -o cashka lib/cashka.o lib/options.o lib/options_check.o lib/options_cli.o lib/options_config.o lib/pm.o main.cpp

# Common functions
lib/cashka.o: inc/cashka.h inc/cashka.cpp
	g++ -Wall -c inc/cashka.cpp -o lib/cashka.o

# Options
lib/options.o: inc/options.h inc/options.cpp
	g++ -Wall -c inc/options.cpp -o lib/options.o

lib/options_check.o: inc/options.h inc/options_check.cpp
	g++ -Wall -c inc/options_check.cpp -o lib/options_check.o
	
lib/options_cli.o: inc/options.h inc/options_cli.cpp
	g++ -Wall -c inc/options_cli.cpp -o lib/options_cli.o

lib/options_config.o: inc/options.h inc/options_config.cpp
	g++ -Wall -c inc/options_config.cpp -o lib/options_config.o

# Process
lib/pm.o: inc/pm.h inc/options.h inc/pm.cpp
	g++ -Wall -c inc/pm.cpp -o lib/pm.o

