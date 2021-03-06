#pragma once
#include  <string>
#include  <vector>
#include "serverRequest.h"

class serverListManager
{
  public:
    serverListManager();
    ~serverListManager();
    bool size(){return m_serverRequest.size();}
    //This throw a excepction if used with the list empty....
    serverRequest* getNext();
    void addtoList(serverRequest* request,bool reset = false);
  private:
    std::vector<serverRequest*> m_serverRequest;
    int m_index;
};
