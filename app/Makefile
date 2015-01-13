C++ = g++

ifndef os
   os = LINUX
endif

ifndef arch
   arch = IA32
endif

CCFLAGS = -std=c++11 -Wall -D$(os) -I../src -finline-functions -O3

ifeq ($(arch), IA32)
   CCFLAGS += -DIA32 #-mcpu=pentiumpro -march=pentiumpro -mmmx -msse
endif

ifeq ($(arch), POWERPC)
   CCFLAGS += -mcpu=powerpc
endif

ifeq ($(arch), IA64)
   CCFLAGS += -DIA64
endif

ifeq ($(arch), SPARC)
   CCFLAGS += -DSPARC
endif

LDFLAGS = -L../src -ludt-static -lstdc++ -lpthread -lm

ifeq ($(os), UNIX)
   LDFLAGS += -lsocket
endif

ifeq ($(os), SUNOS)
   LDFLAGS += -lrt -lsocket
endif

DIR = $(shell pwd)

APP = appserver appclient sendfile recvfile test cpptest

all: $(APP)

%.o: %.cpp
	$(C++) $(CCFLAGS) $< -c

appserver: appserver.o ../src/libudt-static.a
	$(C++) $^ -o $@ $(LDFLAGS)
appclient: appclient.o ../src/libudt-static.a
	$(C++) $^ -o $@ $(LDFLAGS)
sendfile: sendfile.o ../src/libudt-static.a
	$(C++) $^ -o $@ $(LDFLAGS)
recvfile: recvfile.o ../src/libudt-static.a
	$(C++) $^ -o $@ $(LDFLAGS)
test: test.o ../src/libudt-static.a
	$(C++) $^ -o $@ $(LDFLAGS)
cpptest: cpptest.o ../src/libudt-static.a
	$(C++) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o $(APP)

install:
	export PATH=$(DIR):$$PATH
