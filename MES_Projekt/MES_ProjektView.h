
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
	float zageszczenie = 2; //zageszczenie siatki
	float dlugosc; //dlugosc preta
	float x1; //wspolrzedna z pliku
	float x2; //wspolrzedna z pliku
	int nr; //numer obszaru
	float wb1; //pierwszy warunek brzegowy
	float wb2; //drugi warunek brzegowy
	float tempmin;
	float tempmax;
	float y0 = 20;
	float x0 = 50;
	bool obszary = false;
	bool zagesc = false;
	float skl;

	std::vector<float> wspolrz; //wspolrzedne przed zageszczeniem
	std::vector<float> wspolrz2; //wspolrzedne po zageszczeniu
	std::vector<float> lambda; //przenikalnosci cieplne
	std::vector<float> Q; //wydajnosc zrodel
	std::vector<float> L; //dlugosci obszarow
	std::vector<float> L_w_metrach; //długości po zagęszczeniu wyrażone w metrach
	std::vector<float> wynikRozw;
	std::vector<float> obliczenia;
	std::vector<float> lambda_new;
	std::vector<float> Q_new;
	std::vector<float> Temp;
	std::vector<float> L1;
	//std::vector<float> warunkiBrzegowe;



	// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
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
	void Rysujtemp(CDC* pDC);
	void Zageszczanie();
};

#ifndef _DEBUG  // debug version in MES_ProjektView.cpp
inline CMESProjektDoc* CMESProjektView::GetDocument() const
   { return reinterpret_cast<CMESProjektDoc*>(m_pDocument); }
#endif

