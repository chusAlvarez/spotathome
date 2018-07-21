#include <iostream>
#include <cpprest/http_client.h>
#include "serverAnswerValidator.h"

serverAnswerValidator* serverAnswerValidator::m_defaut = new serverAnswerValidator(200);
void serverAnswerValidator::initToEmpty()
{
    m_code = 0;
    m_validAny = false;
    m_body = "";
    m_headers.clear();
}
serverAnswerValidator::serverAnswerValidator()
{
   initToEmpty();
}
serverAnswerValidator::serverAnswerValidator(web::json::value json)
{
  initToEmpty();
  if(json.has_field("response_ok"))
  {
    web::json::value _json = json.at("response_ok");
    if (_json.has_field("code"))
      m_code = _json.at("code").as_integer();
    if (_json.has_field("mode"))
      m_validAny = (_json.at("mode").as_string() == "any");
    if (_json.has_field("body"))
      m_body = _json.at("body").as_string();
    if (_json.has_field("headers"))
    {
        web::json::array headers = _json.at("headers").as_array();
	
	std::string key;
	std::string value;
	for ( std::vector<web::json::value>::iterator it = headers.begin(); it < headers.end();it++)
	{
	  key = (*it).at("key").as_string();
	  value = (*it).at("value").as_string();
	  m_headers.insert(std::pair<std::string,std::string>(key,value));
	}
    }
  }
}
serverAnswerValidator::serverAnswerValidator(int code)
{
  initToEmpty();
  m_code = code;
}
bool serverAnswerValidator::checkHeaders(web::http::http_response* response)
{
  if(m_headers.size())
  {
    web::http::http_headers headers = response->headers();
    for(std::map<std::string,std::string>::const_iterator it = m_headers.begin();it != m_headers.end();it++)
    {
	web::http::http_headers::iterator ithead = headers.find((*it).first);
	if(ithead == headers.end())
	{
	  std::cout << "header " << (*it).first << "not found" << std::endl;
	  return false;
	}
	if((*ithead).second != (*it).second)
	{
	   std::cout << "header expected" <<  (*it).second << "received" << (*ithead).second << std::endl;
	  return false;	  
	}
    }   
  }
  return true;
}
bool serverAnswerValidator::checkBody(web::http::http_response* response)
{
    if(m_body.size())
    {
      if(response->body().is_valid())
      {      
	std::string body = response->extract_string(true).get();
	if (body != m_body)
	{
	  std::cout << "body expected" << m_body << "received" << body << std::endl;
	  return false;
	}
      }else
	  return false;
    }
    
    return true;
}
bool serverAnswerValidator::checkAll(web::http::http_response* response)
{

  if ((m_code) &&  ! (response->status_code() == m_code))
  {
    std::cout << "code expected" << m_code << "received" << response->status_code() << std::endl;
    return false;
  }
  
  if(!checkBody(response))
      return false;
  
  if(!checkHeaders(response))
      return false;

  return true;
}

bool serverAnswerValidator::checkAny(web::http::http_response* response)
{
    if(m_code && response->status_code() == m_code)
      return true;
  
    if(checkBody(response))
      return true;
    
  if(checkHeaders(response))
      return true;
    
    return false;
}

bool serverAnswerValidator::checkResponse(web::http::http_response* response)
{
  if(!m_code && !m_body.size() && !m_headers.size())
    return false;
  
  if (m_validAny)
      return checkAny(response);
    
  return checkAll(response);
}
