CC = g++
CCFLAGS = -ansi -Wall -Wno-deprecated -O2 -DNDEBUG -fPIC -I../include/ 
#LDFLAGS = -rdynamic -L /lib64 -l pthread
LDFLAGS = -rdynamic -l pthread

BOOST_INCLUDE = -DBOOST_HAS_THREADS
BOOST_LDFLAGS = -lboost_system-mt -lboost_serialization-mt -lboost_thread-mt  -lboost_date_time-mt -lboost_filesystem-mt -lboost_iostreams-mt

GMAGICK_INCLUDE = $(shell GraphicsMagick++-config --cppflags --cxxflags)
GMAGICK_LDFLAGS = $(shell GraphicsMagick++-config --ldflags --libs)

SOURCES = Main.o Work.o

all: apoph

apoph: $(SOURCES)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(BOOST_LDFLAGS) $(GMAGICK_LDFLAGS) -o apoph $(SOURCES)

Main.o:	Main.cc
	$(CC) -c $(CCFLAGS) $(BOOST_INCLUDE) Main.cc -o Main.o

Work.o:	Work.cc
	$(CC) -c $(CCFLAGS) $(BOOST_INCLUDE) $(GMAGICK_INCLUDE) Work.cc -o Work.o

strip:
	strip apoph

clean:
	rm -f apoph *.o

install:
	cp apoph /usr/local/bin/
