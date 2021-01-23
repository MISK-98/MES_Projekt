
// MES_ProjektView.h : interface of the CMESProjektView class
//

#pragma once
#include "MES_ProjektDoc.h"
#include "pch.h"

class CMESProjektView : public CView
{
protected: // create from serialization only
	CMESProjektView() noexcept;
	DECLARE_DYNCREATE(CMESProjektView)

// Attributes
public:

	CMESProjektDoc* GetDocument() const;
	CString FilePathName;
	float liczbaobszarow;
	float dlugosc;
	float x1;
	float x2;
	int nr;
	float y0 = 20;
	float x0 = 50;
	bool obszary = false;
	float skl;

	std::vector<float> wynikRozw;



	// Operations
public:
	std::vector<float> tablica;

// Overrides----tego nie ruszam
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation---- tego nie ruszam ---
public:
	virtual ~CMESProjektView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuWczytajplik();
	afx_msg void OnMenuGenerujsiatke();
	afx_msg void OnMenuZagescsiatke();
	afx_msg void OnMenuRysujrozkladtemp();
	afx_msg void OnMenuZapiszplik();
	void czytaj(std::fstream& plik);
	void RysujObszary(CDC* pDC);
};

#ifndef _DEBUG  // debug version in MES_ProjektView.cpp
inline CMESProjektDoc* CMESProjektView::GetDocument() const
   { return reinterpret_cast<CMESProjektDoc*>(m_pDocument); }
#endif

