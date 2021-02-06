
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
	float liczbaobszarow = 0; //obszary z pliku
	float liczbaobszarow2 = 0; //obszary po zageszczeniu
	float liczbawezlow; //liczba wezlow z pliku
	float liczbawezlow2; //liczba wezlow po zageszczeniu
	float zrodlo; //czy material jest zrodlem?
	float moc; //jesli tak to moc zrodla
	float przenikalnosc; //przenikalnosc cieplna materialu
	float zageszczenie = 2; //zageszczenie siatki
	float dlugosc; //dlugosc preta
	float x1; //wspolrzedna z pliku
	float x2; //wspolrzedna z pliku
	int nr; //numer obszaru
	// pomocnicze
	float y0 = 20;
	float x0 = 50;
	bool obszary = false;
	bool zagesc = false;
	float skl;
	float wsp;
	float wsp2;
	float t = 0;

	std::vector<float> wynikRozw;



	// Operations
public:
	std::vector<float> tablica;
	std::vector<float> tablica2;
	std::vector<float> obliczenia;

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
	afx_msg void OnMenuZagescsiatke();
	afx_msg void OnMenuRysujrozkladtemp();
	afx_msg void OnMenuZapiszplik();
	void czytaj(std::fstream& plik);
	void RysujObszary(CDC* pDC);
	void RysujSiatke(CDC* pDC);
	void Zageszczanie();
};

#ifndef _DEBUG  // debug version in MES_ProjektView.cpp
inline CMESProjektDoc* CMESProjektView::GetDocument() const
   { return reinterpret_cast<CMESProjektDoc*>(m_pDocument); }
#endif

