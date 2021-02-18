
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
#include <algorithm>

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

//-------------------początkowe elementy wizualne w menu-----------------
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

	floatString = "Ilosc obszarow:";
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
	
	//-----------zmiany w menu po wczytaniu pliku-------------

	if (obszary)
	{
		RysujObszary(pDC);
		floatString.Format(_T("%.0f"), dlugosc);
		pDC->TextOutW(935, 220, floatString);
		floatString.Format(_T("%.0f"), liczbaobszarow);
		pDC->TextOutW(160, 280, floatString);
		floatString.Format(_T("%.0f"), dlugosc);
		pDC->TextOutW(150, 340, floatString);
		//floatString.Format(_T("%.0f"), tempmax);
		//pDC->TextOutW(1170, 100, floatString);
	}

	//----------------proces zagęszczania siatki---------------

	if (zagesc)
	{
		float wsp = 0; // zerowanie zmiennych i wektorów przed nowym zagęszczaniem
		float wsp2 = 0;
		float t = 0;
		float wsp_w_m = 0;
		float Ltym = 0;
		wspolrz2.clear();
		L.clear();
		L1.clear();
		L_w_metrach.clear();
		obliczenia.clear();
		lambda_new.clear();
		Q_new.clear();
		liczbaobszarow2 = 0;
		for (int i = 0; i < wspolrz.size(); i++)
		{
			wspolrz2.push_back(wspolrz[i]); //zapełnianie wektora starymi wartościami
		}
		sort(wspolrz2.begin(), wspolrz2.end());
		for (int i = 0; i < wspolrz2.size() - 1; i++)
		{
			wsp = (wspolrz2[i + 1] - wspolrz2[i]) / zageszczenie; //zagęszczanie i licznie współrzędnych nowych obszarów
			for (int j = 0; j < zageszczenie; j++)
			{
				wsp2 = wspolrz2[i] + t;
				obliczenia.push_back(wsp2);
				t = t + wsp;
			}
			t = 0;
			wsp = 0;

		}
		wspolrz2.clear();
		for (int i = 0; i < obliczenia.size(); i++) //zapełnianie wektora nowymi współrzędnymi wynikającymi z zagęszczeń
		{
			wspolrz2.push_back(obliczenia[i]);
		}
		sort(wspolrz2.begin(), wspolrz2.end());
		wspolrz2.push_back(wspolrz.back());
		liczbaobszarow2 = wspolrz2.size() - 1;

		for (int i = 0; i < wspolrz2.size() - 1; i++) //tworzenie wektora odcinków przeliczonych na metry
		{
			Ltym = (wspolrz2[i + 1] - wspolrz2[i]) * 0.001; 
			L.push_back(Ltym);
		}

		for (int i = 0; i < lambda.size(); i++) //przeliczanie przenikalności zgodnie z zagęszczeniem
		{
			for (int j = 0; j < zageszczenie; j++)
			{
				lambda_new.push_back(lambda[i]);
			}
		}

		for (int i = 0; i < Q.size(); i++) //przeliczanie wydajności zgodnie z zagęszczeniem
		{
			for (int j = 0; j < zageszczenie; j++)
			{
				Q_new.push_back(Q[i]);
			}
		}

		//----------------zmiany w menu po zagęszczeniu siatki--------------

		RysujSiatke(pDC);
		floatString.Format(_T("%.0f"), dlugosc);
		pDC->TextOutW(935, 220, floatString);
		floatString.Format(_T("%.0f"), liczbaobszarow2);
		pDC->TextOutW(160, 280, floatString);
		floatString.Format(_T("%.0f"), dlugosc);
		pDC->TextOutW(150, 340, floatString);
		floatString.Format(_T("%.0f"), zageszczenie);
		pDC->TextOutW(195, 370, floatString);

	}
}


// CMESProjektView printing

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


// CMESProjektView diagnostics

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


//-----------Rysowanie obszarow---------------

void CMESProjektView::RysujObszary(CDC* pDC)
{
	skl = 900 / dlugosc;  //do przeskalowania
	CPen pen2(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldpen2 = pDC->SelectObject(&pen2);
	for (int i = 0; i < liczbaobszarow; i++)
	{
		x0 == x0 + (skl * wspolrz[i]);

		pDC->MoveTo((x0 + (skl * wspolrz[i])), y0);
		pDC->LineTo((x0 + (skl * wspolrz[i])), (y0 + 200));
	}
}

//---------------Rysowanie zagęszczonej siatki----------------

void CMESProjektView::RysujSiatke(CDC* pDC)
{
	x0 = 50;
	y0 = 20;
	CPen pen2(PS_SOLID, 1, RGB(0, 255, 0));
	CPen* oldpen2 = pDC->SelectObject(&pen2);
	for (int i = 0; i <= liczbaobszarow2; i++)
	{
		x0 == x0 + (skl * wspolrz2[i]);

		pDC->MoveTo((x0 + (skl * wspolrz2[i])), y0);
		pDC->LineTo((x0 + (skl * wspolrz2[i])), (y0 + 200));
	}
}

//-----------------Rysowanie rozkładu temperatury---------------

void CMESProjektView::Rysujtemp(CDC* pDC)
{

}

//----------------Główna funkcja do obliczeń z MES--------------------

std::vector<float> licz(std::vector<float> L, std::vector<float> lambda_new, std::vector<float> Q_new, float wb1, float wb2)
{
	std::vector<float> a;
	std::vector<float> b;
	std::vector<float> c;
	std::vector<float> d;

	//---------------wypełnianie zerami wartości z diagonali (wektory a, b, c, d) oraz wektora rozwiązań d---------------

	for (int i = 0; i < L.size(); i++)
	{
		a.push_back(0);
		b.push_back(0);
		c.push_back(0);
		d.push_back(0);
	}

	//------------liczenie wartości wektorów a, b, c, d-------------

	b[0] += lambda_new[0] / L[0];
	d[0] += Q_new[0] * L[0] / 2;

	for (int i = 0; i < L.size() - 1; i++)
	{
		a[i + 1] += -(lambda_new[i] / L[i]);
	}

	for (int i = 0; i < L.size() - 1; i++)
	{
		c[i] += -(lambda_new[i] / L[i]);
	}

	for (int i = 1; i < L.size(); i++)
	{
		b[i] += (lambda_new[i] / L[i]) + (lambda_new[i - 1] / L[i - 1]);
	}

	for (int i = 1; i < L.size(); i++)
	{
		d[i] += (Q_new[i] * L[i] / 2) + (Q_new[i - 1] * L[i - 1] / 2);
	}

	//--------Podstawienie warunków brzegowych--------------

	d[0] = b[0] * wb1 * 10e8;
	d[L.size() - 1] = b[L.size() - 1] * wb2 * 10e8;

	b[0] *= 10e8;
	b[L.size() - 1] *= 10e8;
	
	//-----------Rozwiązanie układu równań wynikającego z macierzy (w celu wyliczenia wektora temperatur)------------

	c[0] /= b[0];
	d[0] /= b[0];
	
	for (int i = 1; i < L.size(); i++)
	{
		c[i] /= b[i] - a[i] * c[i - 1];
		d[i] = (d[i] - a[i] * d[i - 1]) / (b[i] - a[i] * c[i - 1]);
	}

	for (int i = L.size() - 1; i-- > 0;) 
	{
		d[i] -= c[i] * d[i + 1];
	}
	
	//--------------Zwrócenie wektora z temperaturami------------------

	return d;
	
}


//----------Przycisk- Wczytywanie pliku-------------

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


		std::fstream plik;
		plik.open(FilePathName, std::ios::in);

		//------------czytanie danych z pliku-----------------

		if (plik.good())
		{
			float lambtym;
			char nazwa;
			float Qtym;
			wspolrz.push_back(0);
			std::string ignorowanalinia;
			getline(plik, ignorowanalinia);

			plik >> liczbaobszarow;

			getline(plik, ignorowanalinia);
			getline(plik, ignorowanalinia);
			for (int i = 0; i < liczbaobszarow; i++)
			{
				plik >> nr >> x1 >> x2 >> lambtym >> Qtym >> nazwa;
				wspolrz.push_back(x2);
				lambda.push_back(lambtym);
				Q.push_back(Qtym);

				getline(plik, ignorowanalinia);
			}
			getline(plik, ignorowanalinia);
			plik >> wb1 >> wb2;

			dlugosc = wspolrz.back();
		}
		obszary = true;
		zagesc = false;

		Invalidate(TRUE);
		UpdateWindow();
	}
}

//----------------Przycisk- Zageszczenie siatki---------------
void CMESProjektView::OnMenuZagescsiatke()
{
	obszary = true;
	zagesc = true;
	Invalidate(TRUE);
	UpdateWindow();
	zageszczenie = zageszczenie + 1; //zwiększanie zagęszczenia w razie kolejnego użycia przycisku
}

//---------------Przycisk- Liczenie rozkladu temperatur------------
void CMESProjektView::OnMenuRysujrozkladtemp()
{
	Temp = licz(L, lambda_new, Q_new, wb1, wb2); //liczenie wektora temperatur

	tempmin = Temp[0];
	for (int i = 1; i < Temp.size(); i++) //szukanie mainimalnej temperatury
	{
		if (tempmin > Temp[i])
			tempmin = Temp[i];
	}

	tempmax = Temp[0];
	for (int i = 1; i < Temp.size(); i++) //szukanie maksymalnej temperatury
	{
		if (tempmax < Temp[i])
			tempmax = Temp[i];
	}
}

//---------------Przycisk- Zapisywanie pliku-----------------------
void CMESProjektView::OnMenuZapiszplik()
{
	CFile newfile;
	TCHAR szFilters[] = _T("txt Type Files (*.txt)|*.txt||");
	CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, szFilters);
	if (fileDlg.DoModal() == IDOK)
	{
		FilePathName = fileDlg.GetPathName();
		std::ofstream plik;

		//--------------edycja pliku wynikowego---------------

		plik.open(FilePathName);
		plik << "Wyniki dla " << liczbaobszarow << " obszarów materiałowych, podzielonych na siatkę o \n";
		plik << "Szerokosci odcinkow:\n";
		for (int i = 0; i < L.size(); i++)
		{
			plik << L[i] << "\n";
		}
		plik << "Temperatury odcinkow:\n";
		for (int i = 0; i < Temp.size(); i++)
		{
			plik << Temp[i] << "\n";
		}
		if (liczbaobszarow2 == 0)
			plik << "Liczba obszarow po zageszczeniu: " << liczbaobszarow << "\n";
		else
			plik << "Liczba obszarow po zageszczeniu: " << liczbaobszarow2 << "\n";
		//plik << " węzłach ( " << wsp_x.size() << " w osi X i " << wsp_y.size() << " w osi Y):\n";
		//tempmin = wynikRozw[0];
		//tempmax = wynikRozw[0];
		/*for (int i = 1; i < wynikRozw.size(); i++)
		{
			if (wynikRozw[i] < tempmin)tempmin = wynikRozw[i];
			if (wynikRozw[i] > tempmax)tempmax = wynikRozw[i];
		}*/
		//plik << std::setw(8) << "\nNr węzła" << "|" << std::setw(10) << "X[mm]" << "|" << std::setw(10) << "Y[mm]" << "|";
		//plik << std::setw(14) << "Temperatura[K]" << "|" << std::setw(10) << "Nr obszaru" << "|" << std::setw(22) << "Przewodność X[W/(m*K)]" << "|" << std::setw(22) << "Przewodność Y[W/(m*K)]" << "|" << std::setw(17) << "Moc źródła[W/m^2]\n";
		//for (int i = 0; i < wynikRozw.size(); i++)
		/*{
			pom1.ktory_obszar(wsp_x[i % wsp_x.size()], wsp_y[i / wsp_x.size()], ob, b, wektor_obszarow);
			plik << std::setw(8) << i << "|" << std::setw(10) << wsp_x[i % wsp_x.size()] << "|" << std::setw(10) << wsp_y[i / wsp_x.size()] << "|";
			plik << std::setw(14) << wynikRozw[i] << "|" << std::setw(10) << ob << "|" << std::setw(22) << wektor_obszarow[ob].przewodnosc_x * 1000 << "|" << std::setw(22) << wektor_obszarow[ob].przewodnosc_y * 1000 << "|" << std::setw(17) << wektor_obszarow[ob].moc_zrodla * pow(10, 6) << "\n";
			if (fabs(wynikRozw[i] - tempmax) < 0.0000001)max.push_back(i);
			if (fabs(wynikRozw[i] - tempmin) < 0.0000001)min.push_back(i);
		}*/
		//plik << "\nTemperatura maksymalna [K]: " << tempmax << ". Wystąpiła dla węzłów nr: ";
		/*if (max.size() > 0)
		{
			for (int i = 0; i < max.size() - 1; i++)
			{
				plik << max[i] << ", ";
			}
			plik << max[max.size() - 1] << ".";
		}*/
		//plik << "\nTemperatura minimalna [K]: " << tempmin << ". Wystąpiła dla węzłów nr: ";
		/*if (min.size() > 0)
		{
			for (int i = 0; i < min.size() - 1; i++)
			{
				plik << min[i] << ", ";
			}
			plik << min[min.size() - 1] << ".";
		}*/
		plik.close();
	}
}
