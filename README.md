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

## serverRequest.h serverRequest.cpp
### class serverRequest
Store the data to be sended to a server. Also internally store an object serverAnswerValidator to resolve if the response is ok or not

## serverAnswerValidator.h serverAnswerValidator.cpp
### class serverAnswerValidator
Make the checks to resolve if the response given by the server is ok or not
### class validatorFactory
build and erase the validators needed. internally store a singleton to keep track for all the build validators

## serverListManager.h serverListManager.cpp
### class serverListManager
Store a list of serverRequest, manage sucesive calls to implement a very simple round-robin management
NOTE: should be improved to be able to create its own serverRequest objects from json, currently is deleting objects it don't create and this is a dirty trick indeed....

## test/test_main.cpp
unit test file

## main.cpp
Main program:

- Read the configuration from json
- Create a serverRequest for each server in the configuration, and another one to notify the failures
- launch a secondary thread with its own healt endpoint
- Call all the servers in the order given by the serverListManager, and if fails send a message using the notifier
- Stop when a signal is received

# Continuos integration
## Configured in circleci (https://circleci.com) 
File in ./circleci/config.yml

- First job: Every time a commit is pushed in github, we
  - Build the test
  - Run the test
  - Build the binary
- Second job (if the First one ended with no trouble)
  - Make one docker google-cloud-compatible with
  - Install the source code and build the binary 
  - Push it to google cloud docker registration service
  - Launch kubernetes with k8s.yml configuration in google cloud

# TODO
- Configure a retry number in the json for each server
- Configure a retry number by the notificator (by the way, sometimes the service returns 404 instead 200... have a hard time until see it)
- Configure a priority (but this can be acomplished splitting the server list between some instances, so probably don't worth the time)
- Give more information about the failure (can't resolve name, cant reach ip, response given not a correct one...)
- Change dinamically the server list (but this can be acomplished changing the file in the docker and launching a new pod version, so probably don't worth also the time)
- Change the sdk... in retrospective cpprest was a bad idea (monstruous binary and lot of time linking. My bad)
- Refactor, refactor, refactor....








