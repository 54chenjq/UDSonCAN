
// UDSonCANDoc.cpp : CUDSonCANDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "UDSonCAN.h"
#endif

#include "UDSonCANDoc.h"

#include <propkey.h>

#include "DiagnosticControl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUDSonCANDoc

IMPLEMENT_DYNCREATE(CUDSonCANDoc, CDocument)

BEGIN_MESSAGE_MAP(CUDSonCANDoc, CDocument)
END_MESSAGE_MAP()


// CUDSonCANDoc ����/����

CUDSonCANDoc::CUDSonCANDoc()
	: m_diagnosticStorage(0)
	, m_pDiagnosticControl(NULL)
{
	// TODO: �ڴ����һ���Թ������

}

CUDSonCANDoc::~CUDSonCANDoc()
{
}

DiagnosticStorage::CSection *CUDSonCANDoc::GetDiagnosticStorage()
{
	return &m_diagnosticStorage;
}

CDiagnosticControl &CUDSonCANDoc::GetDiagnosticControl()
{
	return *m_pDiagnosticControl;
}

BOOL CUDSonCANDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	theApp.GetDiagnosticService().BuildDiagnosticStorage(&m_diagnosticStorage);
	if (NULL != m_pDiagnosticControl)
	{
		delete m_pDiagnosticControl;
	}
	m_pDiagnosticControl = new CDiagnosticControl();
	m_pDiagnosticControl->SetNetworkLayerWatchWnd(static_cast<CMainFrame *>(AfxGetMainWnd())->GetNetworkLayerWatchWnd());
	m_pDiagnosticControl->SetApplicationLayerWatchWnd(static_cast<CMainFrame *>(AfxGetMainWnd())->GetApplicationLayerWatchWnd());

	return TRUE;
}

// CUDSonCANDoc ���л�

//void CUDSonCANDoc::Serialize(CArchive &ar)
//{
//	if (ar.IsStoring())
//	{
//		// TODO: �ڴ���Ӵ洢����
//	}
//	else
//	{
//		// TODO: �ڴ���Ӽ��ش���
//	}
//}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CUDSonCANDoc::OnDrawThumbnail(CDC &dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont *pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont *pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CUDSonCANDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CUDSonCANDoc::SetSearchContent(const CString &value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CUDSonCANDoc ���

#ifdef _DEBUG
void CUDSonCANDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUDSonCANDoc::Dump(CDumpContext &dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUDSonCANDoc ����

BOOL CUDSonCANDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	
	return TRUE;
}


BOOL CUDSonCANDoc::OnOpenDocument(LPCTSTR lpszPathName)
{

	return TRUE;
}


void CUDSonCANDoc::OnCloseDocument()
{
	if (NULL != m_pDiagnosticControl)
	{
		delete m_pDiagnosticControl;
		m_pDiagnosticControl = NULL;
	}
	CDocument::OnCloseDocument();
}
