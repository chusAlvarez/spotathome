#pragma once
#include  <string>
#include  <vector>
#include "serverRequest.h"

class serverListManager
{
  public:
    serverListManager();
    ~serverListManager();
    serverRequest getNext();
    void setServerList(std::vector<serverRequest>& serverRequest);
  private:
    std::vector<serverRequest> m_serverRequest;
    int m_index;
};
