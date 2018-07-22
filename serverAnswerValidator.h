#pragma once
#include  <string>
#include  <map>
#include <cpprest/http_client.h>

class serverAnswerValidator
{
  public:
    serverAnswerValidator();
    serverAnswerValidator(web::json::value json);
    serverAnswerValidator(int code);
    ~serverAnswerValidator(){};
    
    bool checkResponse(web::http::http_response* response);
    static serverAnswerValidator* getDefault() { return serverAnswerValidator::m_defaut;}
    
  private:
    static serverAnswerValidator* m_defaut;
    int m_code;
    std::string m_body;
    bool m_validAny;
    void initToEmpty();
    bool checkAll(web::http::http_response* response);
    bool checkAny(web::http::http_response* response);
    bool checkBody(web::http::http_response* response);
    bool checkHeaders(web::http::http_response* response);
    std::map<std::string,std::string> m_headers;
      
};

class validatorFactory
{
  public:
      
      ~validatorFactory();
      static serverAnswerValidator* getValidator(web::json::value json);
      
private:
      validatorFactory();
      serverAnswerValidator* internal_getValidator(web::json::value json);
      std::list<serverAnswerValidator*> m_list;
      static validatorFactory* mysingleton;
};
