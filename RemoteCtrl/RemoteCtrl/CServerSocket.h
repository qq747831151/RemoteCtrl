#pragma once
#include "pch.h"
#include "framework.h"

//���ݰ�
class CPacket
{
public:
	CPacket() :sHead(0), nLength(0), sCmd(0), sSum(0) {}
	CPacket(const CPacket& pack)
	{
		sHead = pack.sHead;
		nLength = pack.nLength;
		strData = pack.strData;
		sCmd = pack.sCmd;
		sSum = pack.sSum;
	}
	CPacket(const BYTE* pData, size_t& nSize)
	{
		size_t i = 0;//����λ��
		for (; i < nSize; i++)
		{
			//�ҵ���ͷ
			if (*(WORD*)(pData + i) == 0xFEFF)
			{
				sHead = *(WORD*)(pData + i);
				i = i + 2;
				break;
			}

		}
		//����ʧ�� 4��length���ֽ� 2cmd 2sum
		if (i + 4 + 2 + 2 > nSize)
		{//�����ݿ��ܲ�ȫ,���߰�ͷδ��ȫ�����յ�
			nSize = 0;
			return;
		}
		nLength = *(WORD*)(pData + i);
		i += 4;

		if (nLength + i > nSize)
		{//��δ��ȫ���յ�,�ͷ���,����ʧ��
			nSize = 0;
			return;
		}
		sCmd = *(WORD*)(pData + i);
		i + 2;
		if (nLength > 4)
		{
			strData.resize(nLength - 2 - 2);
			memcpy((void*)strData.c_str(), pData + i, nLength - 4);
			i += nLength - 4;
		}
		sSum = *(WORD*)(pData + i); i += 2;
		WORD sum = 0;
		for (size_t i = 0; i < strData.size(); i++)
		{
			sum += BYTE(strData[i]) & 0xFF;

		}
		if (sum = sSum)
		{
			nSize = i;//head�����ֽ� lenth4 data....
			return;
		}
		nSize = 0;
	}
	~CPacket()
	{

	}
	CPacket& operator=(const CPacket& pack)
	{
		if (this != &pack)
		{
			sHead = pack.sHead;
			nLength = pack.nLength;
			strData = pack.strData;
			sCmd = pack.sCmd;
			sSum = pack.sSum;
		}
		return *this;
	}
public:
	WORD sHead;//��ͷ  �̶�λ0xEF FF
	DWORD nLength;//������(�ӿ������ʼ,����У�����)
	WORD sCmd;//��������
	std::string strData;//������
	WORD sSum;//��У��
};


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
#define BUFFER_SIZE 4096
	//������Ϣ
	int DealCommand()
	{
		if (m_client == -1)return -1;
		//char buffer[1024] = "";
		char* buffer = new char[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		size_t index = 0;
		while (true)
		{
			size_t len = recv(m_client, buffer + index, BUFFER_SIZE - index, 0);
			if (len <= 0)
			{
				return -1;
			}
			index += len;
			len = index;
			m_packet = CPacket((BYTE*)buffer, len);
			if (len > 0)
			{
				memmove(buffer, buffer + len, BUFFER_SIZE - len);
				index -= len;
				return m_packet.sCmd;
			}
		}
		return -1;
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
	CPacket m_packet;
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


