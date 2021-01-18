
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

	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* oldpen1 = pDC->SelectObject(&pen1);
	pDC->Rectangle(50, 20, 950, 220); //rysowanie prostokątengo pola
	pDC->Rectangle(300, 240, 800, 270); //to bedzie skala temperatury z oznaczeniami kolorow
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
	floatString = "Tutaj bedzie dlugosc max";
	pDC->TextOutW(820, 230, floatString);
	floatString = "Zageszczenie siatki:";
	pDC->TextOutW(800, 340, floatString);
	floatString = "0";
	pDC->TextOutW(290, 273, floatString);
	floatString = "Tu bedzie temp_max";
	pDC->TextOutW(800, 273, floatString);

	//--------To powinno byc w jakims if zeby cyfry wyswietlaly sie gdy zadziala funkcja rysujaca siatke------------
	/*if (Rysuj) {
		RysujObszary(pDC);
		floatString.Format(_T("%.0f"), liczbaobszarow);
		pDC->TextOutW(1170, 100, floatString);
		floatString.Format(_T("%.0f"), liczbawezlow);
		pDC->TextOutW(1170, 100, floatString);
		floatString.Format(_T("%.0f"), tempmax);
		pDC->TextOutW(1170, 100, floatString);
		floatString.Format(_T("%.0f"), dlugosc);
		pDC->TextOutW(1170, 70, floatString);
		floatString.Format(_T("%.0f"), zageszczenie);
		pDC->TextOutW(1170, 70, floatString);
		for (float j = 0; j < liczbaobszarow; j++)
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
		}

	};

	if (RysSiatka)
	{

		CPen pen1(PS_SOLID, 1, RGB(50, 205, 50));
		CPen* oldpen = pDC->SelectObject(&pen1);

		Siatka siatka(wektor_obszarow);
		siatka.utworz_siatke(wektor_obszarow);
		siatka.zageszczenie_prostokatow(zag_y, siatka.kord_y);
		siatka.zageszczenie_prostokatow(zag_x, siatka.kord_x);
		wsp_x.clear();
		wsp_y.clear();
		for (int i = 0; i < siatka.kord_x.size(); i++)
		{
			if (fabs(siatka.kord_x[i]) > 0.0000000001)wsp_x.push_back(siatka.kord_x[i]);
			else wsp_x.push_back(0);
		}
		for (int i = 0; i < siatka.kord_y.size(); i++)
		{
			if (fabs(siatka.kord_y[i]) > 0.0000000001)wsp_y.push_back(siatka.kord_y[i]);
			else wsp_y.push_back(0);
		}
		RozRysujU = true;

		for (int j = 0; j < liczbaobszarow; j++)
		{



			for (int k = 0; k < siatka.kord_y.size(); k++)
			{
				if ((siatka.kord_y[k]) >= (wektor_obszarow[j].y1) && (siatka.kord_y[k]) <= (wektor_obszarow[j].y4))
				{
					if ((yos0 - skala * siatka.kord_y[k]) > 8 && (yos0 - skala * siatka.kord_y[k]) < 590)
					{
						if ((xos0 + skala * wektor_obszarow[j].x1) < 15)
						{
							pDC->MoveTo(15, yos0 - skala * siatka.kord_y[k]);
							if ((xos0 + skala * wektor_obszarow[j].x4) < 800 && (xos0 + skala * wektor_obszarow[j].x4) > 15)
							{
								pDC->LineTo(xos0 + skala * wektor_obszarow[j].x4, yos0 - skala * siatka.kord_y[k]);
							}
							else if ((xos0 + skala * wektor_obszarow[j].x4) > 800)
							{
								pDC->LineTo(800, yos0 - skala * siatka.kord_y[k]);
							}
						}
						else if ((xos0 + skala * wektor_obszarow[j].x1) > 8 && (xos0 + skala * wektor_obszarow[j].x1) < 800)
						{
							pDC->MoveTo(xos0 + skala * wektor_obszarow[j].x1, yos0 - skala * siatka.kord_y[k]);
							if ((xos0 + skala * wektor_obszarow[j].x4) < 800)
							{
								pDC->LineTo(xos0 + skala * wektor_obszarow[j].x4, yos0 - skala * siatka.kord_y[k]);
							}
							else if ((xos0 + skala * wektor_obszarow[j].x4) > 800)
							{
								pDC->LineTo(800, yos0 - skala * siatka.kord_y[k]);
							}
						}
					}
				}
			}
			for (int k = 0; k < siatka.kord_x.size(); k++)
			{
				if ((siatka.kord_x[k]) >= (wektor_obszarow[j].x1) && (siatka.kord_x[k]) <= (wektor_obszarow[j].x4))
				{
					if ((xos0 + skala * siatka.kord_x[k]) > 10 && (xos0 + skala * siatka.kord_x[k]) < 800)
					{
						if ((yos0 - skala * wektor_obszarow[j].y4) < 10)
						{
							pDC->MoveTo(xos0 + skala * siatka.kord_x[k], 10);
							if ((yos0 - skala * wektor_obszarow[j].y1) < 600 && (yos0 - skala * wektor_obszarow[j].y1) > 10)
							{
								pDC->LineTo(xos0 + skala * siatka.kord_x[k], yos0 - skala * wektor_obszarow[j].y1);
							}
							else if ((yos0 - skala * wektor_obszarow[j].y1) > 600)
							{
								pDC->LineTo(xos0 + skala * siatka.kord_x[k], 590);
							}
						}
						else if ((yos0 - skala * wektor_obszarow[j].y4) > 10 && (yos0 - skala * wektor_obszarow[j].y4) < 600)
						{
							pDC->MoveTo(xos0 + skala * siatka.kord_x[k], yos0 - skala * wektor_obszarow[j].y4);
							if ((yos0 - skala * wektor_obszarow[j].y1) < 600)
							{
								pDC->LineTo(xos0 + skala * siatka.kord_x[k], yos0 - skala * wektor_obszarow[j].y1);
							}
							else if ((yos0 - skala * wektor_obszarow[j].y1) > 600)
							{
								pDC->LineTo(xos0 + skala * siatka.kord_x[k], 590);
							}
						}
					}
				}
			}

			liczbaWezlow = wsp_x.size() * wsp_y.size();
		}

		if (RozRysuj)
		{
			TempRysujU = true;
			std::ofstream plik;
			plik.open("Testy.txt");
			plik << "Zaczynamy\n";
			licz licz;
			plik.close();
			licz.rozw(wynikRozw, siatka, wektor_obszarow, wektor_warunkow_brzegowych);
			int nr = 0;
			tempmax = wynikRozw[0];
			tempmin = wynikRozw[0];

			tablica = new float* [x_max];
			for (int i = 0; i < x_max; i++)
			{
				tablica[i] = new float[y_max];
			}


			for (int x = 0; x < x_max; x++)
			{
				for (int y = 0; y < y_max; y++)
				{
					tablica[x][y] = licz.temp(x, y, nr, wynikRozw, siatka);

					if (tablica[x][y] > tempmax)
						tempmax = tablica[x][y];
					if (tablica[x][y] < tempmin && tablica[x][y] > -300)
						tempmin = tablica[x][y];
				}
			}
			plik.open("Testy.txt", std::ofstream::app);
			plik << "\nTmax = " << tempmax << "  ;  Tmin = " << tempmin << "\n";
			RozRysuj = false;
		}
		if (TempRysuj)
		{
			//skala
			double jedn = (tempmax - tempmin) / 500.0;
			int tmp;
			floatString.Format(_T("%.2f"), tempmax);
			pDC->TextOutW(910, 50, floatString);
			floatString.Format(_T("%.2f"), tempmin);
			pDC->TextOutW(910, 550, floatString);
			for (int xx = 850; xx < 900; xx++)
			{
				double temper = tempmax;

				for (int yy = 50; yy < 550; yy++)
				{

					tmp = 255 * (temper - tempmin) / (tempmax - tempmin);
					int tt = tmp / 25;
					pDC->SetPixel(xx, yy, RGB(25 * tt, 20, 255 - 25 * tt));
					temper -= jedn;

				}
			}

			//rysowanie
			int nr = 0;
			float tempa;
			licz licz;
			int pomt = 0;


			for (int i = 0; i < liczbaobszarow; i++)
			{

				for (int x = skala * wektor_obszarow[i].x1; x < skala * wektor_obszarow[i].x4; x++)
				{


					for (int y = skala * wektor_obszarow[i].y1; y < skala * wektor_obszarow[i].y4; y++)
					{
						pomt = licz.temp(x / skala, y / skala, nr, wynikRozw, siatka);

						if (pomt > -300)
						{

							pomt = 255 * (pomt - tempmin) / (tempmax - tempmin);
							int tt = pomt / 25;
							if ((xos0 + x) < 800 && (xos0 + x) > 15)
							{
								if ((yos0 - y) < 590 && (yos0 - y) > 8)
									pDC->SetPixel(xos0 + x, yos0 - y, RGB(25 * tt, 20, 255 - 25 * tt));

							}
						}

					}
				}
			}


			RysujObszary(pDC);
		}

	}*/

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

//---------------wczytywanie pliku-----------------
/*void CMESProjektView::OnMenuWczytajplik()
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
		CArchive loadArchive(&oldFile, CArchive::load | CArchive::bNoFlushOnDelete); // Create the archive to load data, the archive must be closed manually after the loading process      
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

			plik >> skala;

			getline(plik, ignorowanalinia);
			getline(plik, ignorowanalinia);

			for (int i = 0; i < liczba_obszarow; i++)
			{
				Input obszar; //utworzenie obiektu obszar
				obszar.czytaj(plik); //uzupelnienie wlasciwosci obszaru
				obszar.liczba_obszarow = liczba_obszarow;
				obszar.skala = skala;
				if (obszar.x4 > x_max) //znalezienie max i min wspolrzednych obszarow
					x_max = obszar.x4;
				if (obszar.y4 > y_max)
					y_max = obszar.y4;
				obszar.moc_zrodla *= pow(10, -6);
				obszar.przewodnosc_x *= pow(10, -3);
				obszar.przewodnosc_y *= pow(10, -3);
				wektor_obszarow.push_back(obszar); //dodanie obszaru do wektora obszarow

			}
			getline(plik, ignorowanalinia);
			getline(plik, ignorowanalinia);

			int ile_warunkow_brzegowych;

			plik >> ile_warunkow_brzegowych;

			getline(plik, ignorowanalinia);
			getline(plik, ignorowanalinia);

			for (int i = 0; i < ile_warunkow_brzegowych; i++)
			{
				WarunkiBrzegowe warunek; //utworzenie obiektu - warunek brzegowy
				warunek.czytaj(plik);
				wektor_warunkow_brzegowych.push_back(warunek);
			}

		}
		while ((skala * x_max) > 800 || (skala * y_max) > 600) {
			skala -= 0.1;
		}
		skalaTrue = skala;

		//obszar.test(liczba_obszarow,wektor_obszarow,czy_pokrywa);
		Rysuj = true;
		RysSiatka = false;
		RysSiatkaU = true;
		zag_x = 1;
		zag_y = 1;
		//sprawdzanie opszarów
		for (int i = 0; i < liczbaobszarow; i++)
		{
			for (int j = 0; j < liczbaobszarow; j++)
			{
				if (wektor_obszarow[i].x1 < wektor_obszarow[j].x1 && wektor_obszarow[i].x4 > wektor_obszarow[j].x1)
				{
					if (wektor_obszarow[i].y1 < wektor_obszarow[j].y1 && wektor_obszarow[i].y4 > wektor_obszarow[j].y1) {
						err = true;
						Rysuj = false;
					}

				}
				else if (wektor_obszarow[i].x1 < wektor_obszarow[j].x4 && wektor_obszarow[i].x4 > wektor_obszarow[j].x4)
				{
					if (wektor_obszarow[i].y1 < wektor_obszarow[j].y1 && wektor_obszarow[i].y4 > wektor_obszarow[j].y1) {
						err = true;
						Rysuj = false;
					}
				}
				else if (wektor_obszarow[i].x1 < wektor_obszarow[j].x1 && wektor_obszarow[i].x4 > wektor_obszarow[j].x1)
				{
					if (wektor_obszarow[i].y4 < wektor_obszarow[j].y1 && wektor_obszarow[i].y4 > wektor_obszarow[j].y4) {
						err = true;
						Rysuj = false;
					}
				}
				else if (wektor_obszarow[i].x1 < wektor_obszarow[j].x4 && wektor_obszarow[i].x4 > wektor_obszarow[j].x4)
				{
					if (wektor_obszarow[i].y4 < wektor_obszarow[j].y1 && wektor_obszarow[i].y4 > wektor_obszarow[j].y4) {
						err = true;
						Rysuj = false;
					}
				}

			}


		}
		Invalidate(TRUE);
		UpdateWindow();

	}
}*/

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

void CMESProjektView::RysujObszary(CDC* pDC)
{
	CPen pen3(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldpen3 = pDC->SelectObject(&pen3);
	for (int i = 0; i < liczbaobszarow; i++)
	{
		if (wektor_obszarow[i].czy_prostokat) // dla prostokatow
		{
			//pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y1));
			//pDC->LineTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y1));
			if (600 > (yos0 - skala * wektor_obszarow[i].y1) && 10 < (yos0 - skala * wektor_obszarow[i].y1))
			{
				if (8 > (xos0 + skala * wektor_obszarow[i].x1) && 800 > (xos0 + skala * wektor_obszarow[i].x4) && (xos0 + skala * wektor_obszarow[i].x4) > 8)
				{
					pDC->MoveTo(15, (yos0 - skala * wektor_obszarow[i].y1));
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y1));
				}
				else if (8 > (xos0 + skala * wektor_obszarow[i].x1) && 800 < (xos0 + skala * wektor_obszarow[i].x4))
				{
					pDC->MoveTo(15, (yos0 - skala * wektor_obszarow[i].y1));
					pDC->LineTo(800, (yos0 - skala * wektor_obszarow[i].y1));
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y1));

				}
				else if (8 < (xos0 + skala * wektor_obszarow[i].x1) && (xos0 + skala * wektor_obszarow[i].x1) < 800 && 800 > (xos0 + skala * wektor_obszarow[i].x4))
				{
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y1));
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y1));
				}
				else if (8 < (xos0 + skala * wektor_obszarow[i].x1) && (xos0 + skala * wektor_obszarow[i].x1) < 800 && 800 < (xos0 + skala * wektor_obszarow[i].x4))
				{
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y1));
					pDC->LineTo(800, (yos0 - skala * wektor_obszarow[i].y1));
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y1));

				}
			}
			else
			{
				pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y1));
			}

			//pDC->LineTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y4));

			if (8 < (xos0 + skala * wektor_obszarow[i].x4) && 800 > (xos0 + skala * wektor_obszarow[i].x4))
			{
				if (600 < (yos0 - skala * wektor_obszarow[i].y1) && 10 < (yos0 - skala * wektor_obszarow[i].y4) && (yos0 - skala * wektor_obszarow[i].y4) < 600)
				{
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x4), 590);
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y4));
				}
				else if (600 < (yos0 - skala * wektor_obszarow[i].y1) && 10 > (yos0 - skala * wektor_obszarow[i].y4))
				{
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x4), 590);
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x4), 10);
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y4));

				}
				else if (600 > (yos0 - skala * wektor_obszarow[i].y1) && (yos0 - skala * wektor_obszarow[i].y1) > 10 && 10 > (yos0 - skala * wektor_obszarow[i].y4))
				{
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x4), 10);
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y4));
				}
				else if (600 > (yos0 - skala * wektor_obszarow[i].y1) && (yos0 - skala * wektor_obszarow[i].y1) > 10 && 10 < (yos0 - skala * wektor_obszarow[i].y4))
				{
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y4));

				}
			}
			else
			{
				pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x4), (yos0 - skala * wektor_obszarow[i].y4));
			}


			//pDC->LineTo((xos0 + skala * wektor_obszarow[i].x1),  (yos0 - skala * wektor_obszarow[i].y4));
			if (600 > (yos0 - skala * wektor_obszarow[i].y4) && 10 < (yos0 - skala * wektor_obszarow[i].y4))
			{
				if (8 < (xos0 + skala * wektor_obszarow[i].x1) && (xos0 + skala * wektor_obszarow[i].x1) < 800 && 800 < (xos0 + skala * wektor_obszarow[i].x4))
				{
					pDC->MoveTo(800, (yos0 - skala * wektor_obszarow[i].y4));
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y4));
				}
				else if (8 > (xos0 + skala * wektor_obszarow[i].x1) && 800 < (xos0 + skala * wektor_obszarow[i].x4))
				{
					pDC->MoveTo(800, (yos0 - skala * wektor_obszarow[i].y4));
					pDC->LineTo(15, (yos0 - skala * wektor_obszarow[i].y4));
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y4));

				}
				else if (8 < (xos0 + skala * wektor_obszarow[i].x1) && 800 > (xos0 + skala * wektor_obszarow[i].x4))
				{
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y4));
				}
				else if (8 > (xos0 + skala * wektor_obszarow[i].x1) && 800 > (xos0 + skala * wektor_obszarow[i].x4) && (xos0 + skala * wektor_obszarow[i].x4) > 8)
				{

					pDC->LineTo(15, (yos0 - skala * wektor_obszarow[i].y4));
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y4));

				}
			}
			else
			{
				pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y4));
			}

			//pDC->LineTo((xos0 + skala * wektor_obszarow[i].x1),  (yos0 - skala * wektor_obszarow[i].y1));
			if (8 < (xos0 + skala * wektor_obszarow[i].x4) && 800 > (xos0 + skala * wektor_obszarow[i].x4))
			{
				if (600 > (xos0 + skala * wektor_obszarow[i].x1) && (xos0 + skala * wektor_obszarow[i].x1) > 10 && 10 > (yos0 - skala * wektor_obszarow[i].y4))
				{
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), 10);
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y1));
				}
				else if (600 < (yos0 - skala * wektor_obszarow[i].y1) && 10 > (yos0 - skala * wektor_obszarow[i].y4))
				{
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), 8);
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x1), 590);
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y1));

				}
				else if (600 > (yos0 - skala * wektor_obszarow[i].y1) && 10 < (yos0 - skala * wektor_obszarow[i].y4))
				{
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y1));
				}
				else if (600 < (yos0 - skala * wektor_obszarow[i].y1) && 10 < (yos0 - skala * wektor_obszarow[i].y4) && (yos0 - skala * wektor_obszarow[i].y4) < 600)
				{
					pDC->LineTo((xos0 + skala * wektor_obszarow[i].x1), 590);
					pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y1));

				}
			}
			else
			{
				pDC->MoveTo((xos0 + skala * wektor_obszarow[i].x1), (yos0 - skala * wektor_obszarow[i].y1));
			}
		}


	}
}
