#include <iostream>
#include <cpprest/http_client.h>
#include "serverRequest.h"


serverRequest::serverRequest(std::string _method, std::string _endpoint, std::string _headers, std::string _body)

{
    m_method = _method;
    m_endpoint = _endpoint;
    m_headers = _headers;
    m_body = _body;
    
//    m_request.headers().add(L"MyHeaderField", L"MyHeaderValue");
    m_client = NULL;
    m_request = NULL;
    m_validator = NULL;
    
}

bool serverRequest::call()
{
  bool res = false;
  
  try
  {
    if (!m_client)
    {
	  m_client = new web::http::client::http_client(m_endpoint);
	  m_request = new web::http::http_request(m_method);
	  m_request->set_request_uri(m_endpoint);
    }
  
    if (!m_validator)
	  m_validator = serverAnswerValidator::getDefault();
    
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

serverRequest::~serverRequest()
{
  if(m_client != NULL)
  {
    delete(m_client);
    m_client = NULL;
    delete(m_request);
    m_request = NULL;
  }
  
  if((m_validator != NULL) && (m_validator != serverAnswerValidator::getDefault()))
  {
    delete (m_validator);
    m_validator = NULL;
  }
}

