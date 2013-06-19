#pragma once

class CNetworkLayer;

// CNetworkLayerConfigDialog �Ի���

class CNetworkLayerConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNetworkLayerConfigDialog)

public:
	CNetworkLayerConfigDialog(CNetworkLayer &networkLayer, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNetworkLayerConfigDialog();

// �Ի�������
	enum { IDD = IDD_NETWORKLAYERCONFIG };

protected:
	BYTE m_bySeparationTimeMin;
	UINT m_nBlockSize;
	BYTE m_nWaitFrameTransimissionMax;
	UINT m_nNAs;
	UINT m_nNAr;
	UINT m_nNBs;
	UINT m_nNBr;
	UINT m_nNCs;
	UINT m_nNCr;

	CNetworkLayer &m_networkLayer;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void _LoadConfig();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
