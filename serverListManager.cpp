#include "serverListManager.h"
#include "serverRequest.h"
#include <iostream>

serverListManager::serverListManager()
{
  m_index = 0;
} 
serverListManager::~serverListManager()
{
  
} 
void serverListManager::setServerList(std::vector<serverRequest>& serverRequest)
{
    m_index= 0;
    m_serverRequest = serverRequest;
} 

serverRequest& serverListManager::getNext()
{
  int oldindex = m_index;
  if(m_serverRequest.size()!= 0)
  {
    m_index++;
    if (m_index == m_serverRequest.size())
	      m_index = 0;
  }
  return m_serverRequest.at(oldindex);
} 
