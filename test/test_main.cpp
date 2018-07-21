#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cpprest/json.h>
#include "../serverListManager.h"
#include "../serverRequest.h"
#include "../serverAnswerValidator.h"
inline bool check_func(bool condition, std::string expr, std::string file, std::string func, int line) 
{
    if (!condition) 
    {
        std::cerr << "condition '" << expr << "' in " << file << ':' << line << " (" << func << ") FAILED" << std::endl;
	return false;
    }else
        std::cerr << "condition '" << expr << "' in " << file << ':' << line << " (" << func << ") PASS" << std::endl;
    
    return true;
}

#define check(x) check_func((x), #x, __FILE__, __func__, __LINE__)

bool test_answerServerValidator()
{
  serverAnswerValidator* defaultValidator = serverAnswerValidator::getDefault();
  serverAnswerValidator codeValidator(200);
  
  utility::stringstream_t mystream;

  mystream << "{\"response_ok\": {\"code\": 200}}";
  web::json::value myjson = web::json::value::parse(mystream); 
  
  serverAnswerValidator codeValidatorBySON(myjson);
  
  mystream << "{\"response_ok\": {\"headers\": [{\"key\": \"healtcheck\",\"value\" : \"ok\"}]}}";
  myjson = web::json::value::parse(mystream); 
  
  serverAnswerValidator headersValidator(myjson);
  
  mystream << "{\"response_ok\": {\"code\": 200,\"headers\" : [{\"key\": \"itsok\",\"value\" : \"yes\"}],\"body\" : \"{\\\"itsok\\\": \\\"yes\\\"}\"}}";
  myjson = web::json::value::parse(mystream); 
  
  serverAnswerValidator allValidator(myjson);
  
  mystream << "{\"response_ok\": {\"mode\": \"any\",\"code\": 200,\"headers\" : [{\"key\": \"itsok\",\"value\" : \"yes\"}],\"body\" : \"{\\\"itsok\\\": \\\"yes\\\"}\"}}";
  
  myjson = web::json::value::parse(mystream); 
  
  serverAnswerValidator anyValidator(myjson);
  
  web::http::http_response playresponse;
  playresponse.set_status_code(200);
  playresponse.set_body("empty");
  
  bool res = check(defaultValidator->checkResponse(&playresponse));
  
  res = res && check(codeValidator.checkResponse(&playresponse));
  res = res && check(codeValidatorBySON.checkResponse(&playresponse));
  res = res && check(!headersValidator.checkResponse(&playresponse));
  res = res && check(!allValidator.checkResponse(&playresponse));
  res = res && check(anyValidator.checkResponse(&playresponse));
  
  playresponse.set_status_code(666);
  playresponse.set_body("{\"itsok\": \"yes\"}");
  
  
  res = res && check(!defaultValidator->checkResponse(&playresponse));
  res = res && check(!codeValidator.checkResponse(&playresponse));
  res = res && check(!codeValidatorBySON.checkResponse(&playresponse));
  res = res && check(!headersValidator.checkResponse(&playresponse));
  res = res && check(!allValidator.checkResponse(&playresponse));
  res = res && check(anyValidator.checkResponse(&playresponse));

  playresponse.set_body("");
  playresponse.headers().add("itsok","yes");

  res = res && check(!defaultValidator->checkResponse(&playresponse));
  res = res && check(!codeValidator.checkResponse(&playresponse));
  res = res && check(!codeValidatorBySON.checkResponse(&playresponse));
  res = res && check(!headersValidator.checkResponse(&playresponse));
  res = res && check(!allValidator.checkResponse(&playresponse));
  res = res && check(anyValidator.checkResponse(&playresponse));
  
  return res;  
}

bool  test_serverRequest()
{
  
  serverRequest myreq("GET","http://api.wunderground.com/api/Your_Key/conditions/q/CA/San_Francisco.json");
  
  bool res = check(myreq.call());
  
  serverRequest myreqMinvalid("INVALID","http://myapp.com/check");
  
  res = res && check(!myreqMinvalid.call());
  
  serverRequest myreqEPinvalid("GET","INVALID");
  
  res = res && check(!myreqEPinvalid.call());
  
  return res;
}
    
bool test_serverListManager()
{
   serverListManager listmanager;	
    
   listmanager.addtoList(new serverRequest("GET","http://myapp.com/check"),true);
   
   serverRequest* myone = listmanager.getNext();
   
   bool res = check((myone->method() == "GET") && (myone->endpoint() == "http://myapp.com/check") &&  (myone->body() == ""));
   
   myone = listmanager.getNext();
   res =  res && check((myone->method() == "GET") && (myone->endpoint() == "http://myapp.com/check") && (myone->body() == ""));

   listmanager.addtoList(new serverRequest("POST","https://euwest1.otherapp.io/checks","{\"checker\": \"spotahome\"}"),true);
   
   
   myone = listmanager.getNext();
   res =  res && check((myone->method() == "GET") && (myone->endpoint() == "http://myapp.com/check") && (myone->body() == ""));
   
   myone = listmanager.getNext();
   res =  res && check((myone->method() == "POST") && (myone->endpoint() == "https://euwest1.otherapp.io/checks") && (myone->body() == "{\"checker\": \"spotahome\"}"));
   
   myone = listmanager.getNext();
   res =  res && check((myone->method() == "GET") && (myone->endpoint() == "http://myapp.com/check") && (myone->body() == ""));
   return res;
}

bool test()
{
    bool res = test_serverListManager();
    res = res && test_answerServerValidator();
    return res && test_serverRequest();
}

int main(int argc, char **argv)
{
    assert(test() == true);
    return 0;
}