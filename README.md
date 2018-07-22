# spotathome
code check for spotathome
# Test and build
$make test
This buid and execute the unitary test
$make buildmain
build the main binary
$make clean
erase objects and binaries, keep only the code files

# Classes and files

##serverRequest.h serverRequest.cpp
### class serverRequest
Store the data to be sended to a server. Also internally store an object serverAnswerValidator to resolve if the response is ok or not

##serverAnswerValidator.h serverAnswerValidator.cpp
### class serverAnswerValidator
Make the checks to resolve if the response given by the server is ok or not
###class validatorFactory
build and erase the validators needed. internally store a singleton to keep track for all the build validators

##serverListManager.h serverListManager.cpp
### class serverListManager
Store a list of serverRequest, manage sucesive calls to implement a very simple round-robin management
NOTE: should be improved to be able to create its own serverRequest objects from json, currently is deleting objects it don't create and this is a dirty trick indeed....

## test/test_main.cpp
unit test file

##main.cpp
Main program:
-Read the configuration from json
-Create a serverRequest for each server in the configuration, and another one to notify the failures
-launch a secondary thread with its own healt endpoint
-Call all the servers in the order given by the serverListManager, and if fails send a message using the notifier
-Stop when a signal is received








