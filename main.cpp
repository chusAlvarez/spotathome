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
#include <cpprest/http_listener.h>
#include "serverListManager.h"

using namespace std;

bool runserver = true;
void sig_handler (int signum) 
{ 
  runserver = false;
}
//UTILITY FUNCTIONS

bool readFromFile(std::string myfile, web::json::value& res_json)
{
  try 
  {
    std::ifstream  json_file(myfile, std::ifstream::in);
    std::stringstream filestream;
    filestream << json_file.rdbuf();
    res_json = web::json::value::parse(filestream);
    json_file.close();

  }
  catch (web::json::json_exception excep) 
  {
    cout << excep.what() << endl;
    return false;
  }
  
  return true;

}     

bool fillRequestList(serverListManager* request,web::json::value * res_json)
{
    web::json::array servers = res_json->as_array();
  
    web::json::value server;
    std::string method;
    std::string endpoint;
    std::string body;
    std::map<std::string,std::string> headers;

  for(web::json::array::iterator it = servers.begin();it != servers.end();it++)
  {
      server = (*it).at("request");
      method = server.at("method").as_string();
      endpoint = server.at("endpoint").as_string();
  
      if(server.has_field("body"))
	  body = server.at("body").as_string();
      else
	  body = "";
      
      if(server.has_field("headers"))
      {
	  headers.clear();
	  web::json::array jsonheaders = server.at("headers").as_array();
	  std::string key;
	  std::string value;
	  for ( std::vector<web::json::value>::iterator ith = jsonheaders.begin(); ith < jsonheaders.end();ith++)
	  {
	    key = (*ith).at("key").as_string();
	    value = (*ith).at("value").as_string();
	    headers.insert(std::pair<std::string,std::string>(key,value));
	  }
      }
      
      serverRequest* newreq = new serverRequest(method, endpoint, headers,body);
      if( (*it).has_field("description"))
	newreq->setDescription((*it).at("description").as_string());
      
      
      
      if((*it).has_field("response_ok"))
      {
	  newreq->setValidator((*it));
      }
      request->addtoList(newreq);
  }
  return true;
}
void handle_live(web::http::http_request request)
{
   cout << "gets the call....:" << endl;
   request.reply(200, "ok");
}

void* setListener(void *)
{
   web::http::experimental::listener::http_listener listener("http://0.0.0.0:3000/healthz");

   listener.support("GET",  handle_live);
   listener.support("POST", handle_live);
   listener.support("PUT",  handle_live);
   listener.support("DEL",  handle_live);

   try
   {
      listener.open()
         .then([&listener]() {cout << "starting server..." << endl;})
         .wait();
      while (runserver);     
      
   }
   catch (exception const & e)
   {
      cout << "Unable to set listener....." << endl;
   }

}
int main(int argc, char **argv)
{
   if(argc != 2)
   {
     cout << "Usage:" << endl;
     cout << "		" << argv[0] << "configurationfile.json" << endl;
     return 0;
   }
   
   //Handlers to stop the server
   
   signal (SIGINT, sig_handler);
   signal (SIGHUP, sig_handler);
   signal (SIGTERM, sig_handler);
  
   web::json::value configjson;

   // Configuration read 
   if(!readFromFile(argv[1], configjson))
   {
      std::cerr << "unable to run manager, cant read file" << argv[1] << std::endl;
      return false;
   
   }
   
   serverListManager listmanager;

   if(!fillRequestList(&listmanager, &configjson) || listmanager.size() == 0)
   {
      std::cerr << "unable to check list manager, cant read request list or empty" << std::endl;
      return false;
   }   

   
   //We build the notificator to send data to our server
   
   std::map<std::string,std::string>  notiheaders;
   notiheaders.insert(std::pair<std::string,std::string>("Authorization", "Bearer 38f51854b1a282d8e9acdb74710fccc0ba3eb4db"));
   serverRequest notificator("POST", "https://interview-notifier-svc.spotahome.net/api/v1/notification", notiheaders);
   
   //set listener
   pthread_t listener_thread;
   if(pthread_create(&listener_thread, NULL, setListener, NULL)) 
   {
	  cout << "Unable to set listener....." << endl;

   }

   //Now run until a signal stop us....
   while(runserver)
   {
     serverRequest* nextrequest = listmanager.getNext();
     if(!nextrequest->call())
     {
       std::string send_info = "{\"service\":\"";
       send_info.append(nextrequest->endpoint());
       send_info.append("\",\"description\":\"");
       send_info.append(nextrequest->description());
       send_info.append("\"}");
       if(!notificator.call(send_info))
	 cout << "WE HAVE NOT CONNECTION WITH THE SERVER. Data not sended:" << send_info << endl;
       
     } else  
	cout << "SUCCESS:" << nextrequest->endpoint() << endl;
     sleep(2); //We wait between calls....
   }
    return 0;
}
