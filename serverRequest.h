#pragma once
#include  <string>
#include  <map>
#include <cpprest/http_client.h>
#include "serverAnswerValidator.h"

class serverRequest
{
  public:
    serverRequest(std::string _method, std::string _endpoint, std::map<std::string,std::string> headers, std::string _body = "");
    serverRequest(std::string _method, std::string _endpoint, std::string _body = "");
    ~serverRequest();
    
    //To change the default body
    bool call(std::string body);
    bool call();
    void setValidator(serverAnswerValidator* validator);
    void setDescription(std::string serviceDescription);
    std::string method(){return m_method;}
    std::string endpoint(){return m_endpoint;}
    std::map<std::string,std::string>& headers(){return m_headers;}
    std::string body(){return m_body;} 
    std::string description(){return m_description;}
private:
    void initempty();
    std::string m_description;
    std::string m_method;
    std::string m_endpoint;
    std::map<std::string,std::string> m_headers;
    std::string m_body; 
    web::http::methods m_internal_method;
    web::http::client::http_client* m_client;
    web::http::http_request* m_request;
    serverAnswerValidator* m_validator;
    
    
    
};
