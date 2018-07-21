#include "serverListManager.h"
#include "serverRequest.h"
#include <iostream>

serverListManager::serverListManager()
{
  m_index = 0;
} 
serverListManager::~serverListManager()
{
  std::vector<serverRequest*>::iterator it = m_serverRequest.begin();
  while(it != m_serverRequest.end())
  {
      delete((*it));
      m_serverRequest.erase(it);
      it =  m_serverRequest.begin();
  }
  
} 
void serverListManager::addtoList(serverRequest* request,bool reset)
{
  m_serverRequest.push_back(request);
  if(reset)
    m_index = 0;
} 

serverRequest* serverListManager::getNext()
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
