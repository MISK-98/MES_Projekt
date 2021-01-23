
// MES_ProjektView.cpp : implementation of the CMESProjektView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MES_Projekt.h"
#endif

#include "MES_ProjektDoc.h"
#include "MES_ProjektView.h"
#include "sstream"
#include "limits"
#include <vector>
#include <iostream>

//#define NOMINMAX
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMESProjektView

IMPLEMENT_DYNCREATE(CMESProjektView, CView)

BEGIN_MESSAGE_MAP(CMESProjektView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_MENU_WCZYTAJPLIK, &CMESProjektView::OnMenuWczytajplik)
	ON_COMMAND(ID_MENU_GENERUJSIATK32772, &CMESProjektView::OnMenuGenerujsiatke)
	ON_COMMAND(ID_MENU_ZAG32773, &CMESProjektView::OnMenuZagescsiatke)
	ON_COMMAND(ID_MENU_RYSUJROZK32775, &CMESProjektView::OnMenuRysujrozkladtemp)
	ON_COMMAND(ID_MENU_ZAPISZPLIK, &CMESProjektView::OnMenuZapiszplik)
END_MESSAGE_MAP()

// CMESProjektView construction/destruction

CMESProjektView::CMESProjektView() noexcept
{
	// TODO: add construction code here

}

CMESProjektView::~CMESProjektView()
{
}

BOOL CMESProjektView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMESProjektView drawing

//-------------------elementy wizualne w menu-----------------
void CMESProjektView::OnDraw(CDC *pDC)
{
	CMESProjektDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CPen pen1(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldpen1 = pDC->SelectObject(&pen1);
	pDC->Rectangle(50, 20, 950, 220); //rysowanie prostokątengo pola
	pDC->Rectangle(450, 240, 950, 270); //to bedzie skala temperatury z oznaczeniami kolorow
	pDC->MoveTo(50, 15); //rysowanie strzałki osi y
	pDC->LineTo(47, 15);
	pDC->LineTo(50, 5);
	pDC->LineTo(53, 15);
	pDC->LineTo(50, 15);
	pDC->MoveTo(50, 15); //rysowanie obu osi
	pDC->LineTo(50, 219);
	pDC->LineTo(955, 219);
	pDC->MoveTo(955, 219); //rysowanie strzałki osi x
	pDC->LineTo(955, 216);
	pDC->LineTo(965, 219);
	pDC->LineTo(955, 222);
	pDC->LineTo(955, 219);

	CString floatString;

	//--------------Zapisanie tekstu z menu------------

	floatString = "Ilosc obszarow:";
	pDC->TextOutW(50, 250, floatString);
	floatString = "Liczba wezlow siatki:";
	pDC->TextOutW(50, 280, floatString);
	floatString = "Temperatura maksymalna [K]:";
	pDC->TextOutW(50, 310, floatString);
	floatString = "Dlugosc [mm]:";
	pDC->TextOutW(50, 340, floatString);
	floatString = "0";
	pDC->TextOutW(40, 216, floatString);
	floatString = "1";
	pDC->TextOutW(40, 17, floatString);
	floatString = "Zageszczenie siatki:";
	pDC->TextOutW(50, 370, floatString);
	floatString = "0";
	pDC->TextOutW(440, 273, floatString);
	floatString = "Tu bedzie temp_max";
	pDC->TextOutW(950, 273, floatString);

	//--------To powinno byc w jakims if zeby cyfry wyswietlaly sie gdy zadziala funkcja rysujaca siatke------------
	if (obszary)
	{
		RysujObszary(pDC);
		floatString.Format(_T("%.0f"), dlugosc);
		pDC->TextOutW(935, 220, floatString);
		floatString.Format(_T("%.0f"), liczbaobszarow);
		pDC->TextOutW(160, 250, floatString);
		floatString.Format(_T("%.0f"), dlugosc);
		pDC->TextOutW(150, 340, floatString);
		//floatString.Format(_T("%.0f"), liczbawezlow);
		//pDC->TextOutW(1170, 100, floatString);
		//floatString.Format(_T("%.0f"), tempmax);
		//pDC->TextOutW(1170, 100, floatString);
		//floatString.Format(_T("%.0f"), zageszczenie);
		//pDC->TextOutW(1170, 70, floatString);
		/*for (float j = 0; j < liczbaobszarow; j++)
		{
			if ((xxpos - xos0) < (skala * wektor_obszarow[j].x4) && (skala * wektor_obszarow[j].x1) < (xxpos - xos0))
			{
				if ((yos0 - yypos) < (skala * wektor_obszarow[j].y4) && (skala * wektor_obszarow[j].y1) < (yos0 - yypos))
				{

					floatString = "numer obszaru:";
					pDC->TextOutW(980, 160, floatString);
					floatString = "gestosc mocy [W/m^2]:";
					pDC->TextOutW(980, 180, floatString);
					floatString = "przewodnosc x [W/(m * K)]:";
					pDC->TextOutW(980, 200, floatString);
					floatString = "przewodnosc y [W/(m* K):";
					pDC->TextOutW(980, 220, floatString);
					floatString = "material:";
					pDC->TextOutW(980, 240, floatString);

					floatString.Format(_T("%.0f"), wektor_obszarow[j].nr);
					pDC->TextOutW(1170, 160, floatString);
					floatString.Format(_T("%.0f"), wektor_obszarow[j].moc_zrodla * pow(10, 6));
					pDC->TextOutW(1170, 180, floatString);
					floatString.Format(_T("%.0f"), wektor_obszarow[j].przewodnosc_x * 1000);
					pDC->TextOutW(1170, 200, floatString);
					floatString.Format(_T("%.0f"), wektor_obszarow[j].przewodnosc_y * 1000);
					pDC->TextOutW(1170, 220, floatString);

					pDC->TextOutW(1170, 240, CString(wektor_obszarow[j].material));
				}
			}
		}*/

	}

}


// CMESProjektView printing- tego nie ruszam

BOOL CMESProjektView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMESProjektView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMESProjektView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMESProjektView diagnostics- tego nie ruszam

#ifdef _DEBUG
void CMESProjektView::AssertValid() const
{
	CView::AssertValid();
}

void CMESProjektView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMESProjektDoc* CMESProjektView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMESProjektDoc)));
	return (CMESProjektDoc*)m_pDocument;
}
#endif //_DEBUG


// CMESProjektView message handlers


//----------rysowanie obszarow---------------

void CMESProjektView::RysujObszary(CDC* pDC)
{
	skl = 900 / dlugosc;  //do przeskalowania
	CPen pen2(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* oldpen2 = pDC->SelectObject(&pen2);
	for (int i = 0; i < liczbaobszarow; i++)
	{
		x0 == x0 + skl * tablica[i];

		pDC->MoveTo((x0 + (skl * tablica[i])), y0);
		pDC->LineTo((x0 + (skl * tablica[i])), (y0 + 200));
	}
}

//----------wczytywanie pliku-------------

void CMESProjektView::OnMenuWczytajplik()
{
	CFile newfile;
	TCHAR szFilters[] = _T("txt Type Files (*.txt)|*.txt||");
	CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilters);
	if (fileDlg.DoModal() == IDOK)
	{
		CFile oldFile;
		ASSERT(oldFile != NULL);
		oldFile.Open(fileDlg.GetPathName(), CFile::modeRead | CFile::shareExclusive);
		FilePathName = fileDlg.GetPathName();
		CArchive loadArchive(&oldFile, CArchive::load | CArchive::bNoFlushOnDelete);
		Serialize(loadArchive);
		loadArchive.Close();
		oldFile.Close();


		std::fstream plik; //otworzenie pliku o danej nazwie
		plik.open(FilePathName, std::ios::in);

		if (plik.good())
		{
			std::string ignorowanalinia; //linia - smietnik
			getline(plik, ignorowanalinia); //ignorowanie linii z oznaczeniami. Dalej tez tak samo

			plik >> liczbaobszarow;

			getline(plik, ignorowanalinia);
			getline(plik, ignorowanalinia);
			for (int i = 0; i < liczbaobszarow; i++)
			{
				plik >> nr >> x1 >> x2;
				tablica.push_back(x2);
				
				dlugosc = tablica.back();
				getline(plik, ignorowanalinia);
			}
		}
		obszary = true;

		Invalidate(TRUE);
		UpdateWindow();
	}
}

//---------------generowanie siatki----------------
void CMESProjektView::OnMenuGenerujsiatke()
{
	// TODO: Add your command handler code here
}

//----------------zgeszczenie siatki---------------
void CMESProjektView::OnMenuZagescsiatke()
{
	// TODO: Add your command handler code here
}

//---------------rysowanie rozkladu temperatur------------
void CMESProjektView::OnMenuRysujrozkladtemp()
{
	// TODO: Add your command handler code here
}

//---------------zapisywanie pliku-----------------------
void CMESProjektView::OnMenuZapiszplik()
{
	// TODO: Add your command handler code here
}
