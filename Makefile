# Default
all: cashka

# Clean
clean:
	rm -rf obj
	rm -f cashka cashka.pid

# Cashka
cashka: main.cpp \
		obj \
		inc/cashka.h \
		obj/cashka.o \
		inc/options.h \
		obj/options.o \
		obj/options_check.o \
		obj/options_cli.o \
		obj/options_config.o \
		inc/pm.h \
		obj/pm.o
	g++ -Wall -o cashka obj/cashka.o obj/options.o obj/options_check.o obj/options_cli.o obj/options_config.o obj/pm.o main.cpp

# obj-dir
obj:
	mkdir -p obj

# Common functions
obj/cashka.o: inc/cashka.h inc/cashka.cpp
	g++ -Wall -c inc/cashka.cpp -o obj/cashka.o

# Options
obj/options.o: inc/options.h inc/options.cpp
	g++ -Wall -c inc/options.cpp -o obj/options.o

obj/options_check.o: inc/options.h inc/options_check.cpp
	g++ -Wall -c inc/options_check.cpp -o obj/options_check.o
	
obj/options_cli.o: inc/options.h inc/options_cli.cpp
	g++ -Wall -c inc/options_cli.cpp -o obj/options_cli.o

obj/options_config.o: inc/options.h inc/options_config.cpp
	g++ -Wall -c inc/options_config.cpp -o obj/options_config.o

# Process
obj/pm.o: inc/pm.h inc/options.h inc/pm.cpp
	g++ -Wall -c inc/pm.cpp -o obj/pm.o

