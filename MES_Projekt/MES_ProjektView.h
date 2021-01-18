
// MES_ProjektView.h : interface of the CMESProjektView class
//

#pragma once
#include "MES_Projekt.h"

class CMESProjektView : public CView
{
protected: // create from serialization only
	CMESProjektView() noexcept;
	DECLARE_DYNCREATE(CMESProjektView)

// Attributes
public:

	CMESProjektDoc* GetDocument() const;
	bool Rysuj = false;
	CString FilePathName;
	float liczbaobszarow;
	float liczbawezlow = 0;
	float dlugosc;
	float zageszczenie;
	float skala = 1;
	float yos0 = 590;
	float xos0 = 15;
	float yos00 = 590;
	float xos00 = 15;
	bool czy_pokrywa = false;
	bool RysSiatka = false;
	float x_max = 0;
	float y_max = 0;
	float xPos = 0;
	float yPos = 0;
	float xxpos;
	float yypos;
	float skalaTrue;
	double tempmin;
	double tempmax;
	bool TempRysuj = false;
	bool RozRysuj = false;
	bool RysSiatkaU = false;
	bool RozRysujU = false;
	bool TempRysujU = false;
	bool err = false;
	bool ZapiszU = true;

	//std::vector<float> wynikRozw;
	float** tablica;
	std::vector<float> wynikRozw;


	// Operations
public:
	//std::vector <Input> wektor_obszarow;
	//std::vector <WarunkiBrzegowe> wektor_warunkow_brzegowych;
	std::vector<float>wsp_x;
	std::vector<float>wsp_y;


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
	void RysujObszary(CDC* pDC);
};

#ifndef _DEBUG  // debug version in MES_ProjektView.cpp
inline CMESProjektDoc* CMESProjektView::GetDocument() const
   { return reinterpret_cast<CMESProjektDoc*>(m_pDocument); }
#endif

