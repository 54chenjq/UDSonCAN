#pragma once

class CPhysicalLayer;

// CCANConfigDialog �Ի���

class CCANConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCANConfigDialog)

public:
	CCANConfigDialog(CPhysicalLayer &physicalLayer, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCANConfigDialog();

// �Ի�������
	enum { IDD = IDD_CANCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	enum
	{
		DEVICETYPECOUNT		= 22,	// �豸����������
		DEVICEINDEXCOUNT	= 8,	// �豸����������
		BAUDRATETYPECOUNT	= 16,	// Ԥ�貨����������
	};

	int m_nDeviceType;
	int m_nDeviceIndex;
	int m_nCANIndex;
	CString m_csAccCode;
	CString m_csAccMask;
	int m_nBaudRateType;
	CString m_csTiming0;
	CString m_csTiming1;
	int m_nFilter;
	int m_nMode;

	CPhysicalLayer &m_physicalLayer;

	void _LoadConfig();
	virtual void OnOK();

	void _UpdateUIBaudRate();
public:
	afx_msg void OnCbnSelchangeComboCanconfigBaudratetype();
};
