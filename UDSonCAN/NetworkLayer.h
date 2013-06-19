#pragma once

#include <mutex>
#include <list>
#include <vector>

#include "WatchWnd.h"

//#define NETWORKLAYER_NOAE

typedef std::vector<BYTE> BYTEVector;

class CApplicationLayer;
class CDataLinkLayer;
class CDiagnosticControl;

class CNetworkLayer
{
	typedef CTIDCWatchWnd::EntryType EntryType;
	typedef CWatchWnd::Color Color;
public:
	//enum struct MessageType : BYTE	// 15765-2: 5.3.1
	//{
	//	Unknown = 2,
	//	Diagnostics = 0,
	//	RemoteDiagnostics,
	//};

	//enum struct TargetAddressType : BYTE
	//{
	//	Unknown = 2,
	//	Physical = 0,
	//	Functional,
	//};

	enum struct Result : BYTE	// �������������
	{
		N_OK = 0,
		N_TIMEOUT_A,
		N_TIMEOUT_Bs,
		N_TIMEOUT_Cr,
		N_WRONG_SN,
		N_INVALID_FS,
		N_UNEXP_PDU,
		N_WFT_OVRN,
		N_BUFFER_OVFLW,
		N_ERROR
	};

	enum struct PCIType : BYTE	// ��֡ / ��֡ / ����֡ / ������֡��
	{
		Unknown = 4,
		SingleFrame = 0,
		FirstFrame,
		ConsecutiveFrame,
		FlowControl,
	};

	enum struct Status : BYTE	// ���� / �� / ����
	{
		Idle							= 0,
		ReceiveInProgress				= 1,
		TransmitInProgress				= 2
	};

	enum struct FlowControlType : BYTE	// ������֡���͡�
	{
		ContinueToSend = 0,
		Wait,
		Overflow
	};

	enum struct TimingType : BYTE	// �Ժ����¼����ж�ʱ��
	{
		As,
		Ar,
		Bs,
		Br,
		Cs,
		Cr,
		Idle
	};

	CNetworkLayer(void);
	virtual ~CNetworkLayer(void);

	UINT GetAs() const;
	void SetAs(UINT nAs);

	UINT GetAr() const;
	void SetAr(UINT nAr);

	UINT GetBs() const;
	void SetBs(UINT nBs);

	UINT GetBr() const;
	void SetBr(UINT nBr);

	UINT GetCs() const;
	void SetCs(UINT nCs);

	UINT GetCr() const;
	void SetCr(UINT nCr);

	// Status GetStatus() const;		// TODO: ��Ҫ�޸ġ�

	BYTE GetSeparationTimeMin() const;
	void SetSeparationTimeMin(BYTE bySeparationTimeMin);

	BYTE GetBlockSize() const;
	void SetBlockSize(BYTE byBlockSize);

	UINT GetWaitFrameTransimissionMax() const;
	void SetWaitFrameTransimissionMax(UINT nWaitFrameTransimissionMax);

	// 15765-2 5.2
	BOOL Request(INT32 nID, const BYTEVector &vbyData);

	BOOL RequestTesterPresent(INT32 nID);
	// L_Data.confirm
	void Confirm(INT32 nID);

	// L_Data.indication
	void Indication(INT32 nID, const BYTEVector &vbyData);

	void SetApplicationLayer(CApplicationLayer &applicationLayer);
	void SetDataLinkLayer(CDataLinkLayer &dataLinkLayer);
	void SetDiagnosticControl(CDiagnosticControl &diagnosticControl);
protected:
	enum : UINT
	{
		CANFRAMEDATALENGTHMAX				= 8,			// CAN ֡������ݳ��ȡ�
		DIAGNOSTICSFRAMEDATALENGTH			= 7,			// ���֡���ݳ��ȡ�
		REMOTEDIAGNOSTICSFRAMEDATALENGTH	= 6,			// Զ�����֡���ݳ��ȡ�
		TESETRPRESENTSERVICEID				= 0x3E,			// TP Service ID��
		TIMINGCYCLE							= 100			// ��鶨ʱ�����ڡ�
	};

	enum struct J1939ParameterGroupNumber : BYTE
	{
		MixedAddressingFunctional		= 205,
		MixedAddressingPhysical			= 206,
		NormalFixedAddressingPhysical	= 218,
		NormalFixedAddressingFunctional	= 219
	};


	struct MessageBuffer							// ��Ϣ���棬���˻�Ϊ��˫����
													// �Ǿ��д˹��ܵ���ƣ�Ŀǰȡ���˶� ECU ֧�֣�ֻ�Ե�һ ECU �����������ô˽ṹ��
	{
		MessageBuffer();
		BOOL IsBusy();								// [�̰߳�ȫ] ��æ��⡣
		BOOL IsRequest();							// [�̰߳�ȫ] �Ƿ��ͣ�����״̬�������״̬�·�������֡����

		// CCriticalSection csectionProcess;		// ��Ϣ�����ٽ����
		std::recursive_mutex rmutexMessageBuffer;	// ���ʻ�������
		Status status;
		INT32 nID;									// ID
		BYTEVector vbyData;							// ��Ҫ���ͣ��������ڽ��յ����ݡ�
		BYTEVector::size_type stLocation;			// ��Ҫ���ͻ���յ�λ�á�
		PCIType pciType;							// ���� / ��֤ / �����յ��� PCI ���ͣ�SF / FF / CF / FC��
		BYTE bySeparationTimeMin;					// ����֡���͵���С�ȴ�ʱ�䡣
		UINT nRemainderFrameCount;					// ������Ҫ���͵�֡����������Ҫ���յ�֡����
													// ����״̬��Ϊ 0 ��ʾ���ڵȴ�������֡��
													// ����״̬��Ϊ 0 ��ʾ��Ҫ����������֡��
		BYTE byExpectedSequenceNumber;				// �������ͻ��յ��ķֶ����кš�
		DWORD dwTimingStartTick;					// ��ʱ����ʼ��ʱ�䡣
		TimingType timingType;						// �Ժ����¼����ж�ʱ��

		void ResetTiming(TimingType timingType, CEvent &eventTiming);	// [�̰߳�ȫ] ���ö�ʱ����
		void ClearMessage();						// [�̰߳�ȫ] ��ֹ��Ϣ�շ���
	} m_messageBuffer;

	BYTE m_bySeparationTimeMin;					// ������֡�����ķ��ͼ��ʱ�䡣
	BYTE m_byBlockSize;							// ������֡�����ķ��Ϳ��С��

	UINT m_nWaitFrameTransimissionMax;			// 15765-2: 6.6, ���ɷ��ͷ���֤�ģ������Ƶȴ�֡���͵���������

	UINT m_anTimingParameters[6];				// ��ʱ���� As, Ar, Bs, Br, Cs, Cr

	CApplicationLayer *m_pApplicationLayer;
	CDataLinkLayer *m_pDataLinkLayer;
	CDiagnosticControl *m_pDiagnosticControl;

	CCriticalSection m_csectionProcess;			// �շ������ٽ����

	CWinThread *m_pTimingThread;				// ��ʱ�̵߳�ָ�룬���߳�Ӧ�Զ�ɾ����

	BOOL _FillBuffer(Status status, INT32 nID, const BYTEVector &vbyData);
	BOOL _Request();

	static UINT _TimingThread(LPVOID lpParam);
	CEvent m_eventTiming;

	CEvent m_eventStopThread;					// ָʾ��ֹͣ�̡߳�
	CEvent m_eventTimingThreadExited;			// ָʾ Timing �߳��Ѿ��˳���

	void _AddWatchEntry(EntryType entryType, INT32 nID, UINT nDescriptionID, Color color = Color::Black);
	void _AddWatchEntry(EntryType entryType, INT32 nID, const BYTEVector &vbyData, Color color = Color::Black);
	void _AddWatchEntry(EntryType entryType, INT32 nID, UINT nDescriptionID, int nData, Color color = Color::Black);

	void _StartThread();
	void _StopThread();
};
