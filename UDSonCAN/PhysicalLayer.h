#pragma once

#include <vector>
#include <list>

#include "ControlCAN\ControlCAN.h"

#define TESTCODE		// ��������Ӳ��֧Ԯʱ����ʹ�õ�������롣

typedef std::vector<BYTE> BYTEVector;

class CDataLinkLayer;
class CDiagnosticControl;

class CPhysicalLayer
{
public:
	enum : UINT
	{
		MAXDATALENGTH			= 8,			// CAN ֡������ݳ��ȡ�
		CANRECEIVECOUNT			= 10,			// ÿ��������֡����
		CANRECEIVECYCLE			= 10,			// �������ڡ�
		CANRECEIVEFAILEDFALG	= 0xFFFFFFFF	// ����ʧ�ܱ�־��
	};

	enum SendType
	{
		Normal		= 0,
		Single,
		LoopBack,
		SingleLoopBack,
	};

	CPhysicalLayer(void);
	virtual ~CPhysicalLayer(void);

	DWORD GetDeviceType() const;
	void SetDeviceType(DWORD dwDeviceType);
	
	DWORD GetDeviceIndex() const;
	void SetDeviceIndex(DWORD dwDeviceIndex);

	DWORD GetCANIndex() const;
	void SetCANIndex(DWORD dwCANIndex);

	DWORD GetAccCode() const;
	void SetAccCode(DWORD dwAccCode);

	DWORD GetAccMask() const;
	void SetAccMask(DWORD dwAccMask);

	UCHAR GetFilter() const;
	void SetFilter(UCHAR chFilter);

	UINT GetBaudRateType() const;
	void SetBaudRateType(UINT nBaudRateType);

	UCHAR GetTiming0() const;
	void SetTiming0(UCHAR chTiming0);

	UCHAR GetTiming1() const;
	void SetTiming1(UCHAR chTiming1);

	UCHAR GetMode() const;
	void SetMode(UCHAR chMode);

	BOOL OpenDevice();
	BOOL CloseDevice();
	BOOL IsDeviceOpened() const;

	UINT GetCANCount() const;

	BOOL StartCAN();
	BOOL ResetCAN();
	BOOL IsCANStarted() const;

	BOOL Transmit(UINT nID, const BYTEVector &vbyData, SendType sendType = SendType::Normal, BOOL bRemoteFrame = FALSE, BOOL bExternFrame = FALSE, BOOL bConfirmReserveAddress = FALSE);
	
	void SetDataLinkLayer(CDataLinkLayer &dataLinkLayer);
	void SetDiagnosticControl(CDiagnosticControl &diagnosticControl);

#ifdef TESTCODE
	CCriticalSection m_csectionReceiveData;
	VCI_CAN_OBJ m_canObj;
	ULONG m_lReceivedLength;
#endif
protected:
	struct ConfirmBuffer
	{
		UINT nID;						// �� Confirm �� ID��
		BYTE byDataFirstFrame;			// ���ݵ�һ֡��������֤��չ��ַ��
		BOOL bConfirmReverseAddress;	// Confirm ʱ��תԴ��ַ��Ŀ���ַ����� FC��
	};
	typedef std::list<ConfirmBuffer *> PConfirmBufferList;

	BOOL m_bDeviceOpened;
	BOOL m_bCANStarted;

	DWORD m_dwDeviceType;
	DWORD m_dwDeviceIndex;
	DWORD m_dwDeviceReserved;
	DWORD m_dwCANIndex;

	UINT m_nBaudRateType;

	VCI_INIT_CONFIG m_initConfig;
	
	CDataLinkLayer *m_pDataLinkLayer;
	CDiagnosticControl *m_pDiagnosticControl;
	
	CWinThread *m_pReceiveThread;
	BOOL _StartThread();
	void _StopThread();
	static UINT _ReceiveThread(LPVOID lpParam);

	CCriticalSection m_csectionTransmit;
	PConfirmBufferList m_lpConfirmBuffer;
	CEvent m_eventConfirm;
	CWinThread *m_pConfirmThread;
	/***************************************************************
	 * ��֤�̣߳���ֹ���ֹ��������ջ:
	 * ���磬����㷢������֡������������������أ����������������
	 * �¸�����֡�����µ���ջ���
	 ***************************************************************/
	static UINT _ConfirmThread(LPVOID lpParam);

	CEvent m_eventExitReceive;
	CEvent m_eventConfirmThreadExited;
	CEvent m_eventReceiveThreadExited;
};

