#pragma once

#include <vector>
#include <list>

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
	enum struct MessageType : BYTE	// 15765-2: 5.3.1
	{
		Unknown = 2,
		Diagnostics = 0,
		RemoteDiagnostics,
	};

	enum struct TargetAddressType : BYTE
	{
		Unknown = 2,
		Physical = 0,
		Functional,
	};

	enum struct Result : BYTE
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

	enum struct PCIType : BYTE
	{
		Unknown = 4,
		SingleFrame = 0,
		FirstFrame,
		ConsecutiveFrame,
		FlowControl,
	};

	enum struct Status : BYTE
	{
		TransmitInProgress = 0,
		ReceiveInProgress,
		Idle
	};

	enum struct FlowControlType : BYTE
	{
		ContinueToSend = 0,
		Wait,
		Overflow
	};

	enum struct TimingType : BYTE
	{
		As,
		Ar,
		Bs,
		Br,
		Cs,
		Cr,
		Idle
	};								// �Ժ����¼����ж�ʱ��

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

	BOOL IsFullDuplex() const;
	void SetFullDuplex(BOOL bFullDuplex);

	BYTE GetSeparationTimeMin() const;
	void SetSeparationTimeMin(BYTE bySeparationTimeMin);

	BYTE GetBlockSize() const;
	void SetBlockSize(BYTE byBlockSize);

	UINT GetWaitFrameTransimissionMax() const;
	void SetWaitFrameTransimissionMax(UINT nWaitFrameTransimissionMax);

	// 15765-2 5.2
	BOOL Request(MessageType messageType, BYTE bySourceAddress, BYTE byTargetAddress, TargetAddressType targetAddressType, BYTE byAddressExtension, const BYTEVector &vbyData);

	BOOL RequestTesterPresent(BYTE bySourceAddress, BYTE byTargetAddress, TargetAddressType targetAddressType, BYTE byAddressExtension);
	// L_Data.confirm
	void Confirm(UINT nID, BYTE byAddressExtension, BOOL bReverseAddress = FALSE);

	// L_Data.indication
	void Indication(UINT nID, const BYTEVector &vbyData);

	void SetApplicationLayer(CApplicationLayer &applicationLayer);
	void SetDataLinkLayer(CDataLinkLayer &dataLinkLayer);
	void SetDiagnosticControl(CDiagnosticControl &diagnosticControl);
protected:
	enum : UINT
	{
		CANFRAMEDATALENGTHMAX				= 8,			// CAN ֡������ݳ��ȡ�
		DIAGNOSTICSFRAMEDATALENGTH			= 7,			// ���֡���ݳ��ȡ�
		REMOTEDIAGNOSTICSFRAMEDATALENGTH	= 6,			// Զ�����֡���ݳ��ȡ�
		J1939IDTEMPLET						= 0x18000000,	// SAE J1939 ID ģ�塣
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


	struct Message								// ������Ϣ��ISO 15765-2: 6.8��
	{
		Message();

		CCriticalSection csectionProcess;		// ��Ϣ�����ٽ����

		union Address							// ��ַ��Ϣ��
		{
			struct
			{
				BYTE bySourceAddress				: 8;
				BYTE byTargetAddress				: 8;
				TargetAddressType targetAddressType : 7;
				BOOL bTesterPresent					: 1;
				BYTE byAddressExtension				: 8;		// �޷�ȷ�������ʽ���ݲ�֧�ִ���չ��ַ��
			} bitField;
			UINT nUnionAddress;
		} address;
		
		class Compare
		{
		public:
			Compare(UINT nAddress);
			BOOL operator()(const Message *pMessage) const;
		protected:
			UINT m_nAddress;
		};

		Status status;							// ָʾ�� / �� / ���е�״̬��
		BYTEVector vbyData;						// ��Ҫ���ͣ��������ڽ��յ����ݡ�
		BYTEVector::size_type stLocation;		// ��Ҫ���ͻ���յ�λ�á�
		MessageType messageType;				// ��Ϣ���ͣ����֡ / Զ�����֡��
		PCIType pciType;						// ���� Confirm �� PCI ���ͣ�SF / FF / CF / FC��
		BYTE bySeparationTimeMin;				// ����֡���͵���С�ȴ�ʱ�䡣
		UINT nRemainderFrameCount;				// ������Ҫ���͵�֡����������Ҫ���յ�֡����
												// ����״̬��Ϊ 0 ��ʾ���ڵȴ�������֡��
												// ����״̬��Ϊ 0 ��ʾ��Ҫ����������֡��
		BYTE byExpectedSequenceNumber;			// �������ͻ��յ��ķֶ����кš�
		DWORD dwTimingStartTick;				// ��ʱ����ʼ��ʱ�䡣
		TimingType timingType;					// �Ժ����¼����ж�ʱ��

		void ResetTiming(TimingType timingType, CEvent &eventTiming);
		void AbortMessage();					// ��ֹ��Ϣ�շ���
	};

	typedef std::list<Message *> PMessageList;
	PMessageList m_lpMessage;

	BOOL m_bFullDuplex;							// �Ƿ�����ȫ˫��״̬��

	BYTE m_bySeparationTimeMin;					// ������֡�����ķ��ͼ��ʱ�䡣
	BYTE m_byBlockSize;							// ������֡�����ķ��Ϳ��С��

	UINT m_nWaitFrameTransimissionMax;			// 15765-2: 6.6, ���ɷ��ͷ���֤�ģ������Ƶȴ�֡���͵���������

	UINT m_anTimingParameters[6];				// ��ʱ���� As, Ar, Bs, Br, Cs, Cr

	CApplicationLayer *m_pApplicationLayer;
	CDataLinkLayer *m_pDataLinkLayer;
	CDiagnosticControl *m_pDiagnosticControl;

	CCriticalSection m_csectionProcess;			// �շ������ٽ����
	CCriticalSection m_csectionMessageList;		// ��Ϣ�����ٽ����

	CWinThread *m_pTimingThread;				// ��ʱ�̵߳�ָ�룬���߳�Ӧ�Զ�ɾ����

	Message *FindMessage(BOOL bAddIfNotFound, BYTE bySourceAddress, BYTE byTargetAddress, TargetAddressType targetAddressType, BYTE byAddressExtension, BOOL bTesterPresent = FALSE);	// ����һ��ָ������Ϣ���棬�������������ӣ�����ʼ������
	MessageType GetMessageType(BYTE byPF) const;
	TargetAddressType GetTargetAddressType(BYTE byPF) const;

	BOOL _Request(PCIType pciType, MessageType messageType, BYTE bySourceAddress, BYTE byTargetAddress, TargetAddressType targetAddressType, BYTE byAddressExtension, const BYTEVector &vbyData);

	static UINT _TimingThread(LPVOID lpParam);
	CEvent m_eventTiming;

	CEvent m_eventStopThread;					// ָʾ��ֹͣ�̡߳�
	CEvent m_eventTimingThreadExited;			// ָʾ Timing �߳��Ѿ��˳���

	void _AddWatchEntry(EntryType entryType, UINT nID, UINT nDescriptionID, Color color = Color::Black);
	void _AddWatchEntry(EntryType entryType, UINT nID, const BYTEVector &vbyData, Color color = Color::Black);
	void _AddWatchEntry(EntryType entryType, UINT nID, UINT nDescriptionID, int nData, Color color = Color::Black);

	void _StartThread();
	void _StopThread();
};
