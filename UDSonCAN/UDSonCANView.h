
// UDSonCANView.h : CUDSonCANView ��Ľӿ�
//

#pragma once

#include "DiagnosticService.h"
#include "RayPropertyGridCtrl.h"

class CUDSonCANView : public CView
{
// ����
protected: // �������л�����
	CUDSonCANView();
	DECLARE_DYNCREATE(CUDSonCANView)
	virtual ~CUDSonCANView();

// ����
public:
	CUDSonCANDoc *GetDocument() const;

protected:
	CRayPropertyGridCtrl m_propertyGridCtrl;
// ����
public:
	void SwitchService(UINT nServiceID);
	void SwitchProperty(CMFCPropertyGridProperty *pProp);
	BYTE PickupSubItemID(LPCTSTR lpszSubItemString) const;

// ��д
public:
	virtual void OnDraw(CDC *pDC);  // ��д�Ի��Ƹ���ͼ
protected:

// ʵ��
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

protected:
	UINT m_nServiceID;

	void InitPropertyGridCtrl();
	void PushEnumerateData(DiagnosticStorage::CSection *pStorageSection, LPCTSTR lpszDataString);
	void PushBinaryData(DiagnosticStorage::CSection *pStorageSection, LPCTSTR lpszDataString);
	void ShowSubItemSection(const DiagnosticStorage::CItem *pStorageItem, BOOL bShow = TRUE) const;
	// void ShowSubItemSection(const DiagnosticService::SectionVector &vpSection, BOOL bShow = TRUE) const;
	void AddProperties(const DiagnosticService::CItem *pItem, DiagnosticStorage::CItem *pStorageItem, BOOL bShow);

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnToolsTest();
	afx_msg void OnConfigPhysicalLayer();
	afx_msg void OnConfigNetworkLayer();
	afx_msg void OnConfigApplicationLayer();
	afx_msg void OnDiagnosticBeginDiagnostic();
	afx_msg void OnUpdateDiagnosticBeginDiagnostic(CCmdUI *pCmdUI);
	afx_msg void OnDiagnosticStopDiagnostic();
	afx_msg void OnUpdateDiagnosticStopDiagnostic(CCmdUI *pCmdUI);
	afx_msg void OnDiagnosticExecute();
	afx_msg void OnUpdateDiagnosticExecute(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // UDSonCANView.cpp �еĵ��԰汾
inline CUDSonCANDoc *CUDSonCANView::GetDocument() const
   { return reinterpret_cast<CUDSonCANDoc*>(m_pDocument); }
#endif

