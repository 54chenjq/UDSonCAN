
// UDSonCANDoc.h : CUDSonCANDoc ��Ľӿ�
//

#pragma once

#include "DiagnosticStorage.h"

class CDiagnosticControl;

class CUDSonCANDoc : public CDocument
{
protected: // �������л�����
	CUDSonCANDoc();
	DECLARE_DYNCREATE(CUDSonCANDoc)

// ����
public:
	DiagnosticStorage::CSection *GetDiagnosticStorage();
	CDiagnosticControl &GetDiagnosticControl();

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	//virtual void Serialize(CArchive &ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC &dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CUDSonCANDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

protected:
	CDiagnosticControl *m_pDiagnosticControl;
	DiagnosticStorage::CSection m_diagnosticStorage;	// ���ڵ�

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString &value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
};
