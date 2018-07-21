#include "serverListManager.h"
#include "serverRequest.h"
#include  <mutex>
#include <iostream>

std::mutex list_mutex;

serverListManager::serverListManager()
{
  m_index = 0;
} 
serverListManager::~serverListManager()
{
  
} 
void serverListManager::setServerList(std::vector<serverRequest>& serverRequest)
{
  list_mutex.lock();
    m_index= 0;
    m_serverRequest = serverRequest;
  list_mutex.unlock();
} 

serverRequest serverListManager::getNext()
{
      list_mutex.lock();
	serverRequest res("","");
	if(m_serverRequest.size()!= 0)
	{
	   res = m_serverRequest.at(m_index);
	   m_index++;
	   if (m_index == m_serverRequest.size())
	      m_index = 0;
	}
   list_mutex.unlock();
   return res;

} 
