
// UDSonCAN.h : UDSonCAN Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "DiagnosticService.h"

// CUDSonCANApp:
// �йش����ʵ�֣������ UDSonCAN.cpp
//

class CUDSonCANApp : public CWinAppEx
{
public:
	CUDSonCANApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual DiagnosticService::CServiceManager &GetDiagnosticService();

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
	DiagnosticService::CServiceManager *m_pDiagnosticService;
};

extern CUDSonCANApp theApp;
