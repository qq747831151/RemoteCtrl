#pragma once
#include "pch.h"
#include "framework.h"
class CServerSocket
{
public:
	static CServerSocket* getInstance()
	{
		if (m_instance == NULL)//��̬����û��thisָ��,�����޷�ֱ�ӷ��ʳ�Ա����
		{
			m_instance = new CServerSocket();
		}
		return m_instance;

	}
	bool InitScoket()
	{
		if (m_sock == -1)return false;
		sockaddr_in serv_addr;
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;//���е�IP��������������
		serv_addr.sin_port = htons(9527);//host to net unsigned short 

		//��
		if (bind(m_sock, (const sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		{
			return false;
		}
		//����
		if (listen(m_sock, 1) == -1)
		{
			return false;
		}
		return true;
	}
	bool AcceptClient()
	{
		sockaddr_in  client_adr;
		int cli_sz = sizeof(client_adr);
		m_client = accept(m_sock, (sockaddr*)&client_adr, &cli_sz);
		if (m_client == -1)  return false;
		return true;


	}
	//������Ϣ
	int DealCommand()
	{
		if (m_client == -1)return false;
		char buffer[1024] = "";
		while (true)
		{
			int len = recv(m_client, buffer, sizeof(buffer), 0);
			if (len <= 0)
			{
				return -1;
			}
			//TODO
		}
	}
	//������Ϣ
	bool Send(const char* pData, size_t nSize)
	{
		if (m_client == -1)return false;

		return send(m_client, pData, nSize, 0) > 0;
	}
private:
	SOCKET m_client;
	SOCKET m_sock;
	CServerSocket& operator=(const CServerSocket& ss)
	{

	}
	CServerSocket(const CServerSocket& ss)
	{
		m_sock = ss.m_sock;
		m_client = ss.m_client;
	}
	CServerSocket()
	{
		m_client = INVALID_SOCKET;//-1
		if (InitSockEnv() == FALSE)
		{
			MessageBox(NULL, _T("�޷���ʼ���׽��ֻ���,������������"), _T("��ʼ������!"), MB_OK | MB_ICONERROR);
			exit(0);
		}
		m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	~CServerSocket()
	{
		closesocket(m_sock);
		WSACleanup();
	}
	BOOL InitSockEnv()
	{
		//����Windows socket 2.x����
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		if (WSAStartup(ver, &dat) != 0)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}

	}
	static void releaseInstance()
	{
		if (m_instance != NULL)
		{
			CServerSocket* tmp = m_instance;
			m_instance = NULL;
			delete tmp;
		}
	}
	static CServerSocket* m_instance;
	class CHelper
	{
	public:
		CHelper()
		{
			CServerSocket::getInstance();
		}
		~CHelper()
		{
			CServerSocket::releaseInstance();
		}
	};
	static CHelper m_helper;


};


