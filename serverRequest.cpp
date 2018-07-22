#include <iostream>
#include <cpprest/http_client.h>
#include "serverRequest.h"

void serverRequest::initempty()
{
    m_method = "";
    m_endpoint = "";
    m_headers.clear();
    m_body = "";
    m_description = "";
  
    m_client = NULL;
    m_request = NULL;
    m_validator = NULL;

}
serverRequest::serverRequest(std::string _method, std::string _endpoint, std::string _body)
{
    initempty();
    m_method = _method;
    m_endpoint = _endpoint;
    m_body = _body;

}

serverRequest::serverRequest(std::string _method, std::string _endpoint, std::map<std::string,std::string> _headers, std::string _body)
{
    initempty();
    m_method = _method;
    m_endpoint = _endpoint;
    m_headers = _headers;
    m_body = _body;
    
}
void serverRequest::setDescription(std::string serviceDescription)
{
  m_description = serviceDescription;
}

bool serverRequest::call()
{
  return call(m_body);
}

bool serverRequest::call(std::string body)
{
  bool res = false;
  
  try
  {
    if (!m_client)
    {
	  m_client = new web::http::client::http_client(m_endpoint);
	  m_request = new web::http::http_request(m_method);
	  m_request->set_request_uri(m_endpoint);
	  
	  for(std::map<std::string,std::string>::const_iterator it = m_headers.begin();it != m_headers.end();it++)
	  {
	      m_request->headers().add((*it).first,(*it).second);	
	  }  
    }
  
    if (!m_validator)
	  m_validator = serverAnswerValidator::getDefault();
    m_request->set_body(body);
    pplx::task<web::http::http_response> request_task = m_client->request(*m_request);
    request_task.wait();
    web::http::http_response response = request_task.get();
    res = m_validator->checkResponse(&response);
  }catch (std::exception e)
  {
      return false;
  }
  
  return res;
}
void serverRequest::setValidator(web::json::value json)
{
  m_validator = validatorFactory::getValidator(json);
}
serverRequest::~serverRequest()
{
  if(m_client != NULL)
  {
    delete(m_client);
    m_client = NULL;
    delete(m_request);
    m_request = NULL;
  } 
}

