#include <iostream>
#include <cassert>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <cpprest/json.h>
#include "serverListManager.h"

using namespace std;

//UTILITY FUNCTIONS

bool readFromFile(std::string myfile, web::json::value * res_json)
{
  try 
  {
    std::ifstream  json_file(myfile, std::ifstream::in);
    std::stringstream filestream;
    filestream << json_file.rdbuf();
    json_file.close();
    
    *res_json = web::json::value::parse(filestream);
  }
  catch (web::json::json_exception excep) 
  {
    return false;
  }
  
  return true;

}     

bool fillRequestList(std::list<serverRequest>* request,web::json::value * res_json)
{
  return false;
}

int main(int argc, char **argv)
{
   web::json::value configjson;

   if(!readFromFile(argv[0], &configjson))
   {
      std::cerr << "unable to run manager, cant read file" << std::endl;
      return false;
   
   }
   
   std::list<serverRequest> request;
   
   if(!fillRequestList(&request, &configjson) || request.size() == 0)
   {
      std::cerr << "unable to check list manager, cant read request list or empty" << std::endl;
      return false;
   }   
    return 0;
}
