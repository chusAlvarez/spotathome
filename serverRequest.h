#pragma once
#include  <string>
#include  <vector>
#include <cpprest/http_client.h>
#include "serverAnswerValidator.h"

class serverRequest
{
  public:
    serverRequest(std::string _method, std::string _endpoint, std::string _headers = "", std::string _body = "");
    ~serverRequest();
    
    bool call();
    std::string method(){return m_method;}
    std::string endpoint(){return m_endpoint;}
    std::string headers(){return m_headers;}
    std::string body(){return m_body;} 
private:
    std::string m_method;
    std::string m_endpoint;
    std::string m_headers;
    std::string m_body; 
    web::http::methods m_internal_method;
    web::http::client::http_client* m_client;
    web::http::http_request* m_request;
    serverAnswerValidator* m_validator;
    
    
    
};
