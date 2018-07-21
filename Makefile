CXXFLAGS=-std=c++11
SPOTATHOME_OBJECTS= serverListManager.cpp serverRequest.cpp serverAnswerValidator.cpp
LIBS=-lcpprest -lboost_system -lcrypto -lssl  -lpthread

export LD_LIBRARY_PATH=/usr/local/lib/

default: test

buildmain:
	g++ -g -o spotathome main.cpp $(SPOTATHOME_OBJECTS) $(CXXFLAGS) $(LIBS)
	
buildtest:
	g++ -g -o test/test_spotathome test/test_main.cpp $(SPOTATHOME_OBJECTS) $(CXXFLAGS) $(LIBS)

test: buildtest
	test/test_spotathome || exit 1


