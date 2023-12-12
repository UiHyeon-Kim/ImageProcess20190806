
// ImageProc20190806View.cpp: CImageProc20190806View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageProc20190806.h"
#endif

#include "ImageProc20190806Doc.h"
#include "ImageProc20190806View.h"
#include "COpenCvDlg.h"

#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TWO_IMAGES 1
#define THREE_IMAGES 2
#define TWO_IMAGES_SCALED 4
#define MORPHING 8

// CImageProc20190806View

IMPLEMENT_DYNCREATE(CImageProc20190806View, CScrollView)

BEGIN_MESSAGE_MAP(CImageProc20190806View, CScrollView)
	ON_COMMAND(ID_PIXEL_ADD, &CImageProc20190806View::OnPixelAdd)
	ON_COMMAND(ID_PIXEL_SUB, &CImageProc20190806View::OnPixelSub)
	ON_COMMAND(ID_PIXEL_MUL, &CImageProc20190806View::OnPixelMul)
	ON_COMMAND(ID_PIXEL_DIV, &CImageProc20190806View::OnPixelDiv)
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &CImageProc20190806View::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_CONTRAST_STRETCHING, &CImageProc20190806View::OnPixelContrastStretching)
	ON_COMMAND(ID_PIXEL_BINARIZATION, &CImageProc20190806View::OnPixelBinarization)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &CImageProc20190806View::OnPixelTwoImageAdd)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_SUB, &CImageProc20190806View::OnPixelTwoImageSub)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_MUL, &CImageProc20190806View::OnPixelTwoImageMul)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_DIV, &CImageProc20190806View::OnPixelTwoImageDiv)
//	ON_COMMAND(ID_TEST, &CImageProc20190806View::OnTest)
ON_COMMAND(ID_REGION_SMOOTHING, &CImageProc20190806View::OnRegionSmoothing)
ON_COMMAND(ID_REGION_SHRAPENING, &CImageProc20190806View::OnRegionShrapening)
ON_COMMAND(ID_REGION_EMBOSING, &CImageProc20190806View::OnRegionEmbosing)
ON_COMMAND(ID_REGION_PREWITT, &CImageProc20190806View::OnRegionPrewitt)
ON_COMMAND(ID_REGION_ROBERTS, &CImageProc20190806View::OnRegionRoberts)
ON_COMMAND(ID_REGION_SOBEL, &CImageProc20190806View::OnRegionSobel)
ON_COMMAND(ID_REGION_AVERAGE_FILTERING, &CImageProc20190806View::OnRegionAverageFiltering)
ON_COMMAND(ID_REGION_MEDIAN_FILTERING, &CImageProc20190806View::OnRegionMedianFiltering)
ON_COMMAND(ID_MOPOLOGY_COLOR_GRAY, &CImageProc20190806View::OnMopologyColorGray)
ON_COMMAND(ID_MOPOLOGY_BINARIZATION, &CImageProc20190806View::OnMopologyBinarization)
ON_COMMAND(ID_MOPOLOGY_EROSION, &CImageProc20190806View::OnMopologyErosion)
ON_COMMAND(ID_MOPOLOGY_DILATION, &CImageProc20190806View::OnMopologyDilation)
ON_COMMAND(ID_MOPOLOGY_OPENING, &CImageProc20190806View::OnMopologyOpening)
ON_COMMAND(ID_MOPOLOGY_CLOSING, &CImageProc20190806View::OnMopologyClosing)
ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXELCOPY, &CImageProc20190806View::OnGeometryZoominPixelcopy)
ON_COMMAND(ID_GEOMETRY_ZOOMIN_BILINAR_INTERPOLATION, &CImageProc20190806View::OnGeometryZoominBilinarInterpolation)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CImageProc20190806View::OnGeometryZoomoutSubsampling)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_MEAN_SUB, &CImageProc20190806View::OnGeometryZoomoutMeanSub)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_AVG_FILTERING, &CImageProc20190806View::OnGeometryZoomoutAvgFiltering)
ON_COMMAND(ID_GEOMETRY_ROTATION, &CImageProc20190806View::OnGeometryRotation)
ON_COMMAND(ID_GEOMETRY_VERTICAL_FLIP, &CImageProc20190806View::OnGeometryVerticalFlip)
ON_COMMAND(ID_GEOMETRY_HOLIZANTAL_FLIP, &CImageProc20190806View::OnGeometryHolizantalFlip)
ON_COMMAND(ID_GEOMETRY_WARPING, &CImageProc20190806View::OnGeometryWarping)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_COMMAND(ID_AVI_VIEW, &CImageProc20190806View::OnAviView)
ON_COMMAND(ID_GEOMETRY_MORPHING, &CImageProc20190806View::OnGeometryMorphing)
ON_COMMAND(ID_OPENCV, &CImageProc20190806View::OnOpencv)
END_MESSAGE_MAP()

// CImageProc20190806View 생성/소멸

CImageProc20190806View::CImageProc20190806View() noexcept
{
	bAviMode = false;
	//AviFileName = "";
}

CImageProc20190806View::~CImageProc20190806View()
{
}

BOOL CImageProc20190806View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImageProc20190806View 그리기

void CImageProc20190806View::OnDraw(CDC* pDC)
{
	// pDC->TextOutA(100, 100, _T("테스트출력"));
	CImageProc20190806Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (bAviMode) {
		// 재생하는 부분
		LoadAviFIle(pDC);
		bAviMode = false;
		return;
	}

	int x, y;
	
	if (pDoc->depth == 1) {		// 반복문 바깥에 컬러 조건을 넣음으로써 줄은 길어져도 속도면에서 좋음
		for (y = 0; y < pDoc->imageHeight; y++) {
			for (x = 0; x < pDoc->imageWidth; x++) {
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],	// 삽입 이미지 공간 생성
										pDoc->inputImg[y][x],
										pDoc->inputImg[y][x]));
			}
		}
	}
	else {	// 컬러일 때
		for (y = 0; y < pDoc->imageHeight; y++) {
			for (x = 0; x < pDoc->imageWidth; x++) {
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][3 * x + 0],	// red 성분   3*x+0	
										pDoc->inputImg[y][3 * x + 1],	// green 성분 3*x+1
										pDoc->inputImg[y][3 * x + 2]));	// blue 성분  3*x+2
			}
		}
	}
	
	
	if (viewMode == THREE_IMAGES){
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x + 2 * (pDoc->imageWidth + 20), y, RGB(pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x]));
				}
			}
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x + 2 * (pDoc->imageWidth + 20), y, RGB(pDoc->inputImg2[y][3 * x + 0],	// red 성분   3*x+0	
						pDoc->inputImg2[y][3 * x + 1],						// green 성분 3*x+1
						pDoc->inputImg2[y][3 * x + 2]));						// blue 성분  3*x+2
				}
			}
		}
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x + pDoc->imageWidth + 20, y, RGB(pDoc->resultImg[y][x],
						pDoc->resultImg[y][x],
						pDoc->resultImg[y][x]));
				}
			}
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x + pDoc->imageWidth + 20, y, RGB(pDoc->resultImg[y][3 * x + 0],
						pDoc->resultImg[y][3 * x + 1],
						pDoc->resultImg[y][3 * x + 2]));
				}
			}
		}
	}

	if (viewMode == TWO_IMAGES_SCALED) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->gImageHeight; y++) {
				for (x = 0; x < pDoc->gImageWidth; x++) {
					pDC->SetPixel(x, pDoc->imageHeight + 20 + y, RGB(pDoc->gResultImg[y][x],
						pDoc->gResultImg[y][x],
						pDoc->gResultImg[y][x]));
				}
			}
		}
		else {
			for (y = 0; y < pDoc->gImageHeight; y++) {
				for (x = 0; x < pDoc->gImageWidth; x++) {
					pDC->SetPixel(x, pDoc->imageHeight + 20 + y, RGB(pDoc->gResultImg[y][3 * x + 0],
						pDoc->gResultImg[y][3 * x + 1],
						pDoc->gResultImg[y][3 * x + 2]));
				}
			}
		}
	}
	else if (viewMode == MORPHING) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x + 2 * (pDoc->imageWidth + 20), y, RGB(pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x]));
				}
			}
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x + 2 * (pDoc->imageWidth + 20), y, RGB(pDoc->inputImg2[y][3 * x + 0],	// red 성분   3*x+0	
						pDoc->inputImg2[y][3 * x + 1],						// green 성분 3*x+1
						pDoc->inputImg2[y][3 * x + 2]));						// blue 성분  3*x+2
				}
			}
		}
		if (pDoc->depth == 1) {
			for (int i = 0; i < 10; i++) {
				for (y = 0; y < pDoc->imageHeight; y++) {
					for (x = 0; x < pDoc->imageWidth; x++) {
						pDC->SetPixel(x + pDoc->imageWidth + 20, y,
							RGB(pDoc->morphedImg[i][y][x],
								pDoc->morphedImg[i][y][x],
								pDoc->morphedImg[i][y][x]));
					}
				}
			}
		}
		/*else {
			for (int i = 0; i < 10; i++)
				for (y = 0; y < pDoc->imageHeight; y++)
					for (x = 0; x < pDoc->imageWidth; x++)
						pDC->SetPixel(x + pDoc->imageWidth + 20, y, RGB(pDoc->morphedImg[i][y][3 * x + 0],
							pDoc->morphedImg[i][y][3 * x + 1],
							pDoc->morphedImg[i][y][3 * x + 2]));
		}*/
	}
	else {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x + pDoc->imageWidth + 20, y, RGB(pDoc->resultImg[y][x],
						pDoc->resultImg[y][x],
						pDoc->resultImg[y][x]));
				}
			}
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x + pDoc->imageWidth + 20, y, RGB(pDoc->resultImg[y][3 * x + 0],
						pDoc->resultImg[y][3 * x + 1],
						pDoc->resultImg[y][3 * x + 2]));
				}
			}
		}
			
	}
}

void CImageProc20190806View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;

	sizeTotal.cx = sizeTotal.cy = 2048; // 내부 화면 사이즈 조절
	SetScrollSizes(MM_TEXT, sizeTotal);
}

// CImageProc20190806View 진단

#ifdef _DEBUG
void CImageProc20190806View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProc20190806View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProc20190806Doc* CImageProc20190806View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProc20190806Doc)));
	return (CImageProc20190806Doc*)m_pDocument;
}
#endif //_DEBUG

// CImageProc20190806View 메시지 처리기

void CImageProc20190806View::OnPixelAdd()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	if (pDoc->inputImg == NULL)return;

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] + 50;
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}
	viewMode = TWO_IMAGES;
	Invalidate(); // 데이터 수정한걸 다시 그리면서 화면에 나타남
}

void CImageProc20190806View::OnPixelSub()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] - 100;
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnPixelMul()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] * 2;
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnPixelDiv()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] / 2;
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnPixelHistoEq()
{
	CImageProc20190806Doc* pDoc = GetDocument(); // 뷰라서 Document 포인트? 따로 안만들고 여기다 포함시켜서 필요?

	int x, y, i, k;
	int acc_Hist;							// 누적된 히스토그램 값
	int N = pDoc->imageHeight * pDoc->imageWidth;	// N 전체 화소 수 
	int hist[256], sum[256];				// hist 히스토그램 밝기 sum 누적분포

	for (i = 0; i < 256; i++) hist[i] = 0;  // 히스토그램 초기화

	// 히스토그램 구하기
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			k = pDoc->inputImg[y][x];
			hist[k]++;                     // 히스토그램 처리
		}

	// 누적분포
	acc_Hist = 0;
	for (i = 0; i < 256; i++) {            // 히스토그램 각 밝기 값을 sum에 저장
		acc_Hist += hist[i];
		sum[i] = acc_Hist;
	}

	// 평활화 화소값 저장
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			k = pDoc->inputImg[y][x];
			pDoc->resultImg[y][x] = (float)sum[k] / N * 255; // float 캐스트 연산자로 소수점을 구해야함 안그럼 0과 1 흰 검으로 나타남
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnPixelContrastStretching()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y;
	int minVal = 255;
	int maxVal = 0;

	// 입력 이미지에서 최소값과 최대값을 찾습니다.
	for (y = 0; y < pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			int pixelValue = pDoc->inputImg[y][x];
			if (pixelValue < minVal) {
				minVal = pixelValue;
			}
			if (pixelValue > maxVal) {
				maxVal = pixelValue;
			}
		}
	}

	// 명암 대비 스트레칭을 적용하여 결과 이미지를 생성합니다.
	for (y = 0; y <	pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			int pixelValue = pDoc->inputImg[y][x];
			int stretchedValue = 255 * (pixelValue - minVal) / (maxVal - minVal);

			// 범위를 벗어난 값은 조정합니다.
			if (stretchedValue > 255) {
				stretchedValue = 255;
			}
			else if (stretchedValue < 0) {
				stretchedValue = 0;
			}
			pDoc->resultImg[y][x] = stretchedValue;
		}
	}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnPixelBinarization()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, value;
	int threshold = 128;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x];							// 인풋 이미지의 픽셀을 받아옴
			if (value > threshold)	pDoc->resultImg[y][x] = 255;	// 받은 각 픽셀의 값이 128보다 크면 흰
			else					pDoc->resultImg[y][x] = 0;		// 픽셀의 값이 128보다 같거나 작으면 검
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::LoadTwoImage()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);
	CFile file;

	if (dlg.DoModal() == IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		file.Close();
	}
	// dlg.GetPathName();	// 경로명 파일이름 확장자 등 다 넘김
	// dlg.GetFileName();	// 파일 이름만 넘김
	// dlg.GetFileExt();	// 파일 경로만 넘김
	// dlg.GetFileTitle();	// 파일 확장자만 넘김
}

void CImageProc20190806View::OnPixelTwoImageAdd()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK) // DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
	CArchive ar(&file, CArchive::load);
	//LoadTwoImage();		// 새로운 이미지 불러오기
	pDoc->LoadSecondImageFile(ar);
	file.Close();

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = 0.5 * pDoc->inputImg[y][x] + 0.5 * pDoc->inputImg2[y][x];	// 0.* 수정해서 해당 이미지 선명도를 높임
			if (value > 255) value = 255;		// 픽셀 값이 255가 넘어가면 255로 고정
			else if (value < 0) value = 0;		//  " 0보다 작아지면 0으로 고정
			pDoc->resultImg[y][x] = value;
		}
	viewMode = THREE_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnPixelTwoImageSub()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK) // DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
	CArchive ar(&file, CArchive::load);
	//LoadTwoImage();		// 새로운 이미지 불러오기
	pDoc->LoadSecondImageFile(ar);
	file.Close();

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = abs(pDoc->inputImg[y][x] -pDoc->inputImg2[y][x]);	// abs로 절댓값을 구함

			//if (value > 64) value = 255;		// 이진화시킴 (아래 코드 필요없어짐)
			//else value = 0;

			if (value > 255) value = 255;		// 픽셀 값이 255가 넘어가면 255로 고정
			else if (value < 0) value = 0;		//  " 0보다 작아지면 0으로 고정
			pDoc->resultImg[y][x] = value;
		}
	viewMode = THREE_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnPixelTwoImageMul()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK) // DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
	CArchive ar(&file, CArchive::load);
	//LoadTwoImage();		// 새로운 이미지 불러오기
	pDoc->LoadSecondImageFile(ar);
	file.Close();

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] * pDoc->inputImg2[y][x];	// abs로 절댓값을 구함
			
			if (value > 255) value = 255;		// 픽셀 값이 255가 넘어가면 255로 고정
			else if (value < 0) value = 0;		//  " 0보다 작아지면 0으로 고정

			pDoc->resultImg[y][x] = value;
		}
	viewMode = THREE_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnPixelTwoImageDiv()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK) // DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
	CArchive ar(&file, CArchive::load);
	//LoadTwoImage();		// 새로운 이미지 불러오기
	pDoc->LoadSecondImageFile(ar);
	file.Close();

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = (float)pDoc->inputImg[y][x] / pDoc->inputImg2[y][x];	// abs로 절댓값을 구함

			if (value > 255) value = 255;		// 픽셀 값이 255가 넘어가면 255로 고정
			else if (value < 0) value = 0;		//  " 0보다 작아지면 0으로 고정

			pDoc->resultImg[y][x] = value;
		}
	viewMode = THREE_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnRegionSmoothing()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float kernel[3][3] = { {1/9.0,1/9.0,1/9.0},
						   {1/9.0,1/9.0,1/9.0},
						   {1/9.0,1/9.0,1/9.0} };
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::Convolve(unsigned char** Inimg, unsigned char** Outimg, int cols, int rows, float mask[][3], int bias, int depth)
{
	int x, y, value, i, j;
	float sum, redsum, greensum, bluesum;
	for (y = 1; y < rows-1; y++) {		// 테두리 부분 계산 의미 없으므로
		for (x = 1; x < cols-1; x++) {
			if (depth==1) {				// 흑백일 때
				sum = 0;
				for (j = 0; j < 3; j++) 
					for (i = 0; i < 3; i++)
						sum += Inimg[y - 1 + j][x - 1 + i] * mask[j][i];
				sum += bias;
				if (sum > 255) sum = 255;
				else if (sum < 0)sum = 0;
				Outimg[y][x] = sum;
			}
			else {						// 컬러일 때
				redsum = greensum = bluesum = 0;
				for (j = 0; j < 3; j++) 
					for (i = 0; i < 3; i++) {
						redsum +=	Inimg[y - 1 + j][3 * (x - 1 + i) + 0] * mask[j][i];
						greensum += Inimg[y - 1 + j][3 * (x - 1 + i) + 1] * mask[j][i];
						bluesum +=	Inimg[y - 1 + j][3 * (x - 1 + i) + 2] * mask[j][i];
					}
				redsum += bias;
				greensum += bias;
				bluesum += bias;

				if (redsum > 255) redsum = 255;
				else if (redsum < 0)redsum = 0;
				if (greensum > 255) greensum = 255;
				else if (greensum < 0)greensum = 0;
				if (bluesum > 255) bluesum = 255;
				else if (bluesum < 0)bluesum = 0;

				Outimg[y][3 * x + 0] = redsum;
				Outimg[y][3 * x + 1] = greensum;
				Outimg[y][3 * x + 2] = bluesum;
			}
		}
	}
}

void CImageProc20190806View::OnRegionShrapening()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float kernel[3][3] = { {0,-1,0},
						   {-1,5,-1},
						   {0,-1,0} };
	/*float kernel[3][3] = { {-1,-1,-1},
						   {-1,9,-1},
						   {-1,-1,-1} };*/
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnRegionEmbosing()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float kernel[3][3] = { {-1,0, 0},
					       {0, 0, 0},
						   {0, 0, 1} };
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnRegionPrewitt()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float Hmask[3][3] = { {-1,-1,-1},
						  {0,  0, 0},
						  {1,  1, 1} };
	float Vmask[3][3] = { {1, 0, -1},
						  {1, 0, -1},
					      {1, 0, -1} };

	unsigned char** Er, ** Ec;
	int value;

	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < pDoc->imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}
	Convolve(pDoc->inputImg, Er, pDoc->imageWidth, pDoc->imageHeight, Hmask, 0, pDoc->depth);
	Convolve(pDoc->inputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, Vmask, 0, pDoc->depth);

	for (int y = 0; y < pDoc->imageHeight; y++)
		for (int x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]);
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}

	// 메모리 해제
	for (int i = 0; i < pDoc->imageHeight; i++){
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnRegionRoberts()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float Hmask[3][3] = { {-1, 0, 0},
						  {0,  1, 0},
						  {0,  0, 0} };
	float Vmask[3][3] = { {0, 0, -1},
						  {0, 1,  0},
						  {0, 0,  0} };

	unsigned char** Er, ** Ec;
	int value;

	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < pDoc->imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}
	Convolve(pDoc->inputImg, Er, pDoc->imageWidth, pDoc->imageHeight, Hmask, 0, pDoc->depth);
	Convolve(pDoc->inputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, Vmask, 0, pDoc->depth);

	for (int y = 0; y < pDoc->imageHeight; y++)
		for (int x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]);
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}

	// 메모리 해제
	for (int i = 0; i < pDoc->imageHeight; i++) {
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnRegionSobel()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float Hmask[3][3] = { {-1,-2,-1},
						  { 0, 0, 0},
						  { 1, 2, 1} };
	float Vmask[3][3] = { { 1, 0,-1},
						  { 2, 0,-2},
						  { 1, 0,-1} };

	unsigned char** Er, ** Ec;
	int value;

	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < pDoc->imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}
	Convolve(pDoc->inputImg, Er, pDoc->imageWidth, pDoc->imageHeight, Hmask, 0, pDoc->depth);
	Convolve(pDoc->inputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, Vmask, 0, pDoc->depth);

	for (int y = 0; y < pDoc->imageHeight; y++)
		for (int x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]);
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}

	// 메모리 해제
	for (int i = 0; i < pDoc->imageHeight; i++) {
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnRegionAverageFiltering()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, i, j, k, xcor, ycor, sum, count, rsum, gsum, bsum;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			if (pDoc->depth == 1) {
				sum = count = 0;
				for (j = -2; j <= 2; j++)
					for (i = -2; i <= 2; i++) {
						ycor = y + j;
						xcor = x + i;

						if (0 <= ycor && ycor <= pDoc->imageHeight - 1 && 0 <= xcor && xcor <= pDoc->imageWidth - 1) {
							sum += pDoc->inputImg[ycor][xcor];
							count++;
						}
					}
				pDoc->resultImg[y][x] = sum / count;
			}
			else if (pDoc->depth == 3) {
				rsum = gsum = bsum = count = 0;
				for (j = -2; j <= 2; j++)
					for (i = -2; i <= 2; i++) {
						ycor = y + j;
						xcor = x + i;

						if (0 <= ycor && ycor <= pDoc->imageHeight - 1 && 0 <= xcor && xcor <= pDoc->imageWidth - 1) {
							rsum += pDoc->inputImg[ycor][3 * xcor + 0];
							gsum += pDoc->inputImg[ycor][3 * xcor + 1];
							bsum += pDoc->inputImg[ycor][3 * xcor + 2];
							count++;
						}
					}
				pDoc->resultImg[y][3 * x + 0] = rsum / count;
				pDoc->resultImg[y][3 * x + 1] = gsum / count;
				pDoc->resultImg[y][3 * x + 2] = bsum / count;
			}
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnRegionMedianFiltering()
{
	CImageProc20190806Doc* pDoc = GetDocument();
	int x, y, i, j, temp;
	int n[9];

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++) {
			if (pDoc->depth == 1) {
				n[0] = pDoc->inputImg[y - 1][x - 1];
				n[1] = pDoc->inputImg[y - 1][x - 0];
				n[2] = pDoc->inputImg[y - 1][x + 1];
				n[3] = pDoc->inputImg[y - 0][x - 1];
				n[4] = pDoc->inputImg[y - 0][x - 0];
				n[5] = pDoc->inputImg[y - 0][x + 1];
				n[6] = pDoc->inputImg[y + 1][x - 1];
				n[7] = pDoc->inputImg[y + 1][x - 0];
				n[8] = pDoc->inputImg[y + 1][x + 1];

				// buble sorting
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
				pDoc->resultImg[y][x] = n[4];
			}
			else if (pDoc->depth == 3) {
				n[0] = pDoc->inputImg[y - 1][3 * (x - 1) + 0];
				n[1] = pDoc->inputImg[y - 1][3 * (x - 0) + 0];
				n[2] = pDoc->inputImg[y - 1][3 * (x + 1) + 0];
				n[3] = pDoc->inputImg[y - 0][3 * (x - 1) + 0];
				n[4] = pDoc->inputImg[y - 0][3 * (x - 0) + 0];
				n[5] = pDoc->inputImg[y - 0][3 * (x + 1) + 0];
				n[6] = pDoc->inputImg[y + 1][3 * (x - 1) + 0];
				n[7] = pDoc->inputImg[y + 1][3 * (x - 0) + 0];
				n[8] = pDoc->inputImg[y + 1][3 * (x + 1) + 0];

				// buble sorting
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
				pDoc->resultImg[y][3 * x + 0] = n[4];

				n[0] = pDoc->inputImg[y - 1][3 * (x - 1) + 1];
				n[1] = pDoc->inputImg[y - 1][3 * (x - 0) + 1];
				n[2] = pDoc->inputImg[y - 1][3 * (x + 1) + 1];
				n[3] = pDoc->inputImg[y - 0][3 * (x - 1) + 1];
				n[4] = pDoc->inputImg[y - 0][3 * (x - 0) + 1];
				n[5] = pDoc->inputImg[y - 0][3 * (x + 1) + 1];
				n[6] = pDoc->inputImg[y + 1][3 * (x - 1) + 1];
				n[7] = pDoc->inputImg[y + 1][3 * (x - 0) + 1];
				n[8] = pDoc->inputImg[y + 1][3 * (x + 1) + 1];

				// buble sorting
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
				pDoc->resultImg[y][3 * x + 1] = n[4];

				n[0] = pDoc->inputImg[y - 1][3 * (x - 1) + 2];
				n[1] = pDoc->inputImg[y - 1][3 * (x - 0) + 2];
				n[2] = pDoc->inputImg[y - 1][3 * (x + 1) + 2];
				n[3] = pDoc->inputImg[y - 0][3 * (x - 1) + 2];
				n[4] = pDoc->inputImg[y - 0][3 * (x - 0) + 2];
				n[5] = pDoc->inputImg[y - 0][3 * (x + 1) + 2];
				n[6] = pDoc->inputImg[y + 1][3 * (x - 1) + 2];
				n[7] = pDoc->inputImg[y + 1][3 * (x - 0) + 2];
				n[8] = pDoc->inputImg[y + 1][3 * (x + 1) + 2];

				// buble sorting
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
				pDoc->resultImg[y][3 * x + 2] = n[4];
			}
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnMopologyColorGray()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	if (pDoc->depth == 1)return;

	int x, y, gray;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			gray = (pDoc->inputImg[y][3 * x + 0] + pDoc->inputImg[y][3 * x + 1] + pDoc->inputImg[y][3 * x + 2]) / 3;
			pDoc->inputImg[y][3 * x + 0] = gray;
			pDoc->inputImg[y][3 * x + 1] = gray;
			pDoc->inputImg[y][3 * x + 2] = gray;
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnMopologyBinarization()
{
	CImageProc20190806Doc* pDoc = GetDocument();
	int x, y;
	int thresh = 128;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			if (pDoc->depth == 1) {
				if (pDoc->inputImg[y][x] > thresh)
					pDoc->inputImg[y][x] = 255;
				else
					pDoc->inputImg[y][x] = 0;
			}
			else {
				if (pDoc->inputImg[y][3 * x + 0] > thresh) {
					pDoc->inputImg[y][3 * x + 0] = 255;
					pDoc->inputImg[y][3 * x + 1] = 255;
					pDoc->inputImg[y][3 * x + 2] = 255;
				}
				else {
					pDoc->inputImg[y][3 * x + 0] = 0;
					pDoc->inputImg[y][3 * x + 1] = 0;
					pDoc->inputImg[y][3 * x + 2] = 0;
				}
			}
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnMopologyErosion()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, i, j;
	int min, rmin, gmin, bmin;

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++) {
			if (pDoc->depth == 1) {
				min = 255;
				for (j = -1; j <= 1; j++)
					for (i = -1; i <= 1; i++) {
						if (pDoc->inputImg[y + j][x + i] < min)
							min = pDoc->inputImg[y + j][x + i];
					}
				pDoc->resultImg[y][x] = min;
			}
			else {
				rmin = gmin = bmin = 255;
				for (j = -1; j <= 1; j++)
					for (i = -1; i <= 1; i++) {
						if (pDoc->inputImg[y + j][3 * (x + i) + 0] < rmin)
							rmin = pDoc->inputImg[y + j][3 * (x + i) + 0];
						if (pDoc->inputImg[y + j][3 * (x + i) + 1] < gmin)
							gmin = pDoc->inputImg[y + j][3 * (x + i) + 1];
						if (pDoc->inputImg[y + j][3 * (x + i) + 2] < bmin)
							bmin = pDoc->inputImg[y + j][3 * (x + i) + 2];
					}
				pDoc->resultImg[y][3 * x + 0] = rmin;
				pDoc->resultImg[y][3 * x + 1] = gmin;
				pDoc->resultImg[y][3 * x + 2] = bmin;
			}
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnMopologyDilation()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, i, j;
	int max, rmax, gmax, bmax;

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++) {
			if (pDoc->depth == 1) {
				max = 0;
				for (j = -1; j <= 1; j++)
					for (i = -1; i <= 1; i++) {
						if (pDoc->inputImg[y + j][x + i] > max)
							max = pDoc->inputImg[y + j][x + i];
					}
				pDoc->resultImg[y][x] = max;
			}
			else {
				rmax = gmax = bmax = 0;
				for (j = -1; j <= 1; j++)
					for (i = -1; i <= 1; i++) {
						if (pDoc->inputImg[y + j][3 * (x + i) + 0] > rmax)
							rmax = pDoc->inputImg[y + j][3 * (x + i) + 0];
						if (pDoc->inputImg[y + j][3 * (x + i) + 1] > gmax)
							gmax = pDoc->inputImg[y + j][3 * (x + i) + 1];
						if (pDoc->inputImg[y + j][3 * (x + i) + 2] > bmax)
							bmax = pDoc->inputImg[y + j][3 * (x + i) + 2];
					}
				pDoc->resultImg[y][3 * x + 0] = rmax;
				pDoc->resultImg[y][3 * x + 1] = gmax;
				pDoc->resultImg[y][3 * x + 2] = bmax;
			}
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnMopologyOpening()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();

	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
}

void CImageProc20190806View::OnMopologyClosing()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();

	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
}

void CImageProc20190806View::CopyResultToInput()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			pDoc->inputImg[y][x] = pDoc->resultImg[y][x];
		}
}

void CImageProc20190806View::OnGeometryZoominPixelcopy()
{
	CImageProc20190806Doc* pDoc = GetDocument();
	
	int x, y;
	int xscale = 3;
	int yscale = 2;

	if (pDoc->gResultImg != NULL) {
		for (int i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]);
		free(pDoc->gResultImg);
	}

	pDoc->gImageWidth = pDoc->imageWidth * xscale;
	pDoc->gImageHeight = pDoc->imageHeight * yscale;

	//메모리 할당
	pDoc->gResultImg= (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gImageHeight; i++) 
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	//
	for (y = 0; y < pDoc->gImageHeight; y++)
		for (x = 0; x < pDoc->gImageWidth; x++) {
			if(pDoc->depth == 1)
				pDoc->gResultImg[y][x] = pDoc->inputImg[y / yscale][x / xscale];
			else {
				pDoc->gResultImg[y][3 * x + 0] = pDoc->inputImg[y / yscale][3 * (x / xscale) + 0];
				pDoc->gResultImg[y][3 * x + 1] = pDoc->inputImg[y / yscale][3 * (x / xscale) + 1];
				pDoc->gResultImg[y][3 * x + 2] = pDoc->inputImg[y / yscale][3 * (x / xscale) + 2];
			}
		}
	// 정방향사상
	/*for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
			pDoc->gResultImg[y * yscale][x * xscale] = pDoc->inputImg[y][x];
	*/
	viewMode = TWO_IMAGES_SCALED;
	Invalidate();
}

void CImageProc20190806View::OnGeometryZoominBilinarInterpolation()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, Ax, Ay, Bx, By, Cx, Cy, Dx, Dy, E, F;
	float xscale = 2.3;
	float yscale = 1.5;
	float src_x, src_y, alpha, beta;

	if (pDoc->gResultImg != NULL) {
		for (int i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]);
		free(pDoc->gResultImg);
	}

	pDoc->gImageWidth = pDoc->imageWidth * xscale + 0.5;
	pDoc->gImageHeight = pDoc->imageHeight * yscale + 0.5;
	
	//메모리 할당
	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gImageHeight; i++)
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);

	// 역방향 사상
	for (y = 0; y < pDoc->gImageHeight; y++)
		for (x = 0; x < pDoc->gImageWidth; x++) {
			src_x = x / xscale;
			src_y = y / yscale;
			alpha = src_x - (int)src_x;
			beta = src_y - (int)src_y;

			Ax = (int)src_x;
			Ay = (int)src_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;

			if (Bx > pDoc->imageWidth - 1) Bx = pDoc->imageWidth - 1;
			if (Dx > pDoc->imageWidth - 1) Dx = pDoc->imageWidth - 1;
			if (Cy > pDoc->imageHeight - 1) Cy = pDoc->imageHeight - 1;
			if (Dy > pDoc->imageHeight - 1) Dy = pDoc->imageHeight - 1;

			if (pDoc->depth == 1) {
				E = (1 - alpha) * pDoc->inputImg[Ay][Ax] + alpha * pDoc->inputImg[By][Bx];
				F = (1 - alpha) * pDoc->inputImg[Cy][Cx] + alpha * pDoc->inputImg[Dy][Dx];
			
				pDoc->gResultImg[y][x] = (1 - beta) * E + beta * F;
			}
			else {
				F = (1 - alpha) * pDoc->inputImg[Cy][3 * Cx + 0] + alpha * pDoc->inputImg[Dy][3 * Dx + 0];
				E = (1 - alpha) * pDoc->inputImg[Ay][3 * Ax + 0] + alpha * pDoc->inputImg[By][3 * Bx + 0];
				pDoc->gResultImg[y][3 * x + 0] = (1 - beta) * E + beta * F;
				
				F = (1 - alpha) * pDoc->inputImg[Cy][3 * Cx + 1] + alpha * pDoc->inputImg[Dy][3 * Dx + 1];
				E = (1 - alpha) * pDoc->inputImg[Ay][3 * Ax + 1] + alpha * pDoc->inputImg[By][3 * Bx + 1];
				pDoc->gResultImg[y][3 * x + 1] = (1 - beta) * E + beta * F;
				
				F = (1 - alpha) * pDoc->inputImg[Cy][3 * Cx + 2] + alpha * pDoc->inputImg[Dy][3 * Dx + 2];
				E = (1 - alpha) * pDoc->inputImg[Ay][3 * Ax + 2] + alpha * pDoc->inputImg[By][3 * Bx + 2];
				pDoc->gResultImg[y][3 * x + 2] = (1 - beta) * E + beta * F;

			}
		}	
	viewMode = TWO_IMAGES_SCALED;
	Invalidate();
}

void CImageProc20190806View::OnGeometryZoomoutSubsampling()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int xscale = 3;		// 1/3
	int yscale = 2;		// 1/2
	int x, y;

	if (pDoc->gResultImg != NULL) {
		for (int i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]);
		free(pDoc->gResultImg);
	}

	pDoc->gImageWidth = pDoc->imageWidth / xscale;		// 줄어들었을 때 이미지 크기
	pDoc->gImageHeight = pDoc->imageHeight / yscale;

	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gImageHeight; i++)
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);

	// 역방향 사상
	for (y = 0; y < pDoc->gImageHeight; y++)
		for (x = 0; x < pDoc->gImageWidth; x++) {
			if (pDoc->depth == 1)
				pDoc->gResultImg[y][x] = pDoc->inputImg[y * yscale][x * xscale];
			else {
				pDoc->gResultImg[y][3 * x + 0] = pDoc->inputImg[y * yscale][3 * (x * xscale) + 0];
				pDoc->gResultImg[y][3 * x + 1] = pDoc->inputImg[y * yscale][3 * (x * xscale) + 1];
				pDoc->gResultImg[y][3 * x + 2] = pDoc->inputImg[y * yscale][3 * (x * xscale) + 2];
			}
		}

	viewMode = TWO_IMAGES_SCALED;
	Invalidate();
}

void CImageProc20190806View::OnGeometryZoomoutMeanSub()
{
	OnRegionSmoothing();
	CopyResultToInput();
	OnGeometryZoomoutSubsampling();
}

void CImageProc20190806View::OnGeometryZoomoutAvgFiltering()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int xscale = 3;		// 1/3
	int yscale = 2;		// 1/2
	int x, y, i, j, sum, rsum, bsum, gsum, src_x, src_y;

	if (pDoc->gResultImg != NULL) {
		for (int i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]);
		free(pDoc->gResultImg);
	}

	pDoc->gImageWidth = pDoc->imageWidth / xscale;		// 줄어들었을 때 이미지 크기
	pDoc->gImageHeight = pDoc->imageHeight / yscale;

	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gImageHeight; i++)
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);

	// 전방향 사상
	for (y = 0; y < pDoc->imageHeight; y += yscale)
		for (x = 0; x < pDoc->imageWidth; x += xscale) {
			if (pDoc->depth == 1) {
				sum = 0;
				for (j = 0; j < yscale; j++) 
					for (i = 0; i < xscale; i++) {
						src_x = x + i;
						src_y = y + j;
						sum += pDoc->inputImg[src_y][src_x];
					}
				pDoc->gResultImg[y / yscale][x / xscale] = sum / (xscale * yscale);
			}
			else {
				rsum = gsum = bsum = 0;
				for (j = 0; j < yscale; j++)
					for (i = 0; i < xscale; i++) {
						src_x = x + i;
						src_y = y + j;
						rsum += pDoc->inputImg[src_y][3 * src_x + 0];
						gsum += pDoc->inputImg[src_y][3 * src_x + 1];
						bsum += pDoc->inputImg[src_y][3 * src_x + 2];
					}
				pDoc->gResultImg[y / yscale][3 * (x / xscale) + 0] = rsum / (xscale * yscale);
				pDoc->gResultImg[y / yscale][3 * (x / xscale) + 1] = gsum / (xscale * yscale);
				pDoc->gResultImg[y / yscale][3 * (x / xscale) + 2] = bsum / (xscale * yscale);
			}
		}
	viewMode = TWO_IMAGES_SCALED;
	Invalidate();
}

#define PI 3.1415926521
#include "CAngleDialog.h"
void CImageProc20190806View::OnGeometryRotation()
{
	CImageProc20190806Doc* pDoc = GetDocument();
	CAngleDialog dlg;

	int angle = 30; // degree
	int x, y, Hy, Cx, Cy, xdiff, ydiff, x_source, y_source;
	float radian;	// 회전 각도 계산

	dlg.m_iAngle = angle; // Dialog에 뜨는 기본값이 30이 됨
	if (dlg.DoModal() == IDCANCEL) return;
	angle = dlg.m_iAngle;

	radian = PI / 180 * angle;	
	// y의 마지막
	Hy = pDoc->imageHeight - 1;
	// 중심점
	Cx = pDoc->imageWidth / 2;
	Cy = pDoc->imageHeight / 2;

	if (pDoc->gResultImg != NULL) {
		for (int i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]);
		free(pDoc->gResultImg);
	}

	pDoc->gImageWidth = pDoc->imageHeight * fabs(cos(PI / 2 - radian)) + pDoc->imageWidth * fabs(cos(radian));	// 회전했을 때 이미지 크기
	pDoc->gImageHeight = pDoc->imageHeight * fabs(cos(radian)) + pDoc->imageWidth * fabs(cos(PI / 2 - radian));	// radian = 세타 // fabs 양수로 만듬
 
	// 메모리 할당
	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gImageHeight; i++)
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);

	xdiff = (pDoc->gImageWidth - pDoc->imageWidth) / 2;
	ydiff = (pDoc->gImageHeight - pDoc->imageHeight) / 2;

	for (y = -ydiff; y < pDoc->gImageHeight - ydiff; y++)
		for (x = -xdiff; x < pDoc->gImageWidth - xdiff; x++) {
			x_source = (Hy - y - Cx) * sin(radian) + (x - Cx) * cos(radian) + Cx;
			y_source = Hy - ((Hy - y - Cy) * cos(radian) - (x - Cx) * sin(radian) + Cy);
			if (pDoc->depth == 1) {
				if (x_source<0 || x_source>pDoc->imageWidth - 1 || y_source<0 || y_source>pDoc->imageHeight - 1)
					pDoc->gResultImg[y + ydiff][x + xdiff] = 255;
				else

					pDoc->gResultImg[y + ydiff][x + xdiff] = pDoc->inputImg[y_source][x_source];
			}
			else {
				if (x_source<0 || x_source>pDoc->imageWidth - 1 || y_source<0 || y_source>pDoc->imageHeight - 1) {
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 0] = 255;
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 1] = 255;
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 2] = 255;
				}
				else {
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 0] = pDoc->inputImg[y_source][3 * x_source + 0];
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 1] = pDoc->inputImg[y_source][3 * x_source + 1];
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 2] = pDoc->inputImg[y_source][3 * x_source + 2];
				}
			}
		}
	viewMode = TWO_IMAGES_SCALED;
	Invalidate();
}
void CImageProc20190806View::OnGeometryHolizantalFlip()
{
	CImageProc20190806Doc* pDoc = GetDocument();
	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->resultImg[y][x] = pDoc->inputImg[y][pDoc->imageWidth - 1 - x];
			else {
				pDoc->resultImg[y][3 * x + 0] = pDoc->inputImg[y][3 * (pDoc->imageWidth - 1 - x) + 0];
				pDoc->resultImg[y][3 * x + 1] = pDoc->inputImg[y][3 * (pDoc->imageWidth - 1 - x) + 1];
				pDoc->resultImg[y][3 * x + 2] = pDoc->inputImg[y][3 * (pDoc->imageWidth - 1 - x) + 2];
			}
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

void CImageProc20190806View::OnGeometryVerticalFlip()
{
	CImageProc20190806Doc* pDoc = GetDocument();
	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->resultImg[pDoc->imageHeight - 1 - y][x] = pDoc->inputImg[y][x];
			else {
				pDoc->resultImg[pDoc->imageHeight - 1 - y][3 * x + 0] = pDoc->inputImg[y][3 * x + 0];
				pDoc->resultImg[pDoc->imageHeight - 1 - y][3 * x + 1] = pDoc->inputImg[y][3 * x + 1];
				pDoc->resultImg[pDoc->imageHeight - 1 - y][3 * x + 2] = pDoc->inputImg[y][3 * x + 2];
			}
		}
	viewMode = TWO_IMAGES;
	Invalidate();
}

typedef struct // 새로 만든 자료형 typedef를 붙여주면 됨
{
	int Px;
	int Py;
	int Qx;
	int Qy;
}control_line;

control_line mctrl_source = { 100,100,150,150 };
control_line mctrl_dest = { 100,100,200,200 };

void CImageProc20190806View::OnGeometryWarping()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	control_line source_lines[5] = { {100,100,150,150}, {0,0,pDoc->imageWidth - 1,0},{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1},
									 {pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1},{0,pDoc->imageHeight - 1,0,0} };
	control_line dest_lines[5] = { {100,100,200,200}, {0,0,pDoc->imageWidth - 1,0},{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1},
									 {pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1},{0,pDoc->imageHeight - 1,0,0} };

	source_lines[0] = mctrl_source;
	dest_lines[0] = mctrl_dest;

	int x, y;
	double u;	// 수직 교차점의 위치
	double h;	// 제어선으로부터 픽셀의 수직 변위
	double d;	// 제어선과 픽셀 사이의 거리
	double tx, ty;	// 결과영상 픽셀에 대응되는 입력 영상 픽셀 사이의 변위의 합 
	double xp, yp;	// 각 제어선에 대해 계산된 입력 영상의 대응되는 픽셀 위치
	double weight;	// 각 제어선의 가중치
	double totalweight;	// 가중치의 합
	double a = 0.001;	// 0을 나누지 않기 위한 변수
	double b = 2.0;		// 어느정도 끌려올지, 찌그러질지
	double p = 0.75;

	int x1, y1, x2, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;

	int num_lines = 5;	// 제어선의 개수
	int line;
	int source_x, source_y;
	int last_row, last_col;

	last_col = pDoc->imageWidth - 1;
	last_row = pDoc->imageHeight - 1;
	// 출력 영상의 각 픽셀에 대해
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			tx = 0.0;
			ty = 0.0;
			totalweight = 0.0;
			// 각 제어선에 대해
			for (line = 0; line < num_lines; line++) {
				x1 = dest_lines[line].Px;
				y1 = dest_lines[line].Py;
				x2 = dest_lines[line].Qx;
				y2 = dest_lines[line].Qy;

				dest_line_length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				// 수직 교차점의 위치 및 픽셀의 수직 변위 계산
				u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) / (double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				h = ((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;
				// 제어선과 픽셀 사이의 거리 계산
				if (u < 0) d = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
				else if (u > 1) d = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
				else d = fabs(h);	// 절댓값

				src_x1 = source_lines[line].Px;
				src_y1 = source_lines[line].Py;
				src_x2 = source_lines[line].Qx;
				src_y2 = source_lines[line].Qy;
				src_line_length = sqrt((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1));
				// 입력 영상에서의 대응 픽셀 위치 계산
				xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
				yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;
				// 제어선에 대한 가중치 계산
				weight = pow(pow(dest_line_length, p) / (a + d), b);
				// 대응 픽셀과의 변위 계산
				tx += (xp - x) * weight;
				ty += (yp - y) * weight;
				totalweight += weight;
			}
			source_x = x + (tx / totalweight/* + 0.5*/);
			source_y = y + (ty / totalweight/* + 0.5*/);
			// 영상의 경계를 벗어나는지 검사
			if (source_x < 0)source_x = 0;
			if (source_x > last_col)source_x = last_col;
			if (source_y < 0)source_y = 0;
			if (source_y > last_row)source_y = last_row;

			if (pDoc->depth == 1)
				pDoc->resultImg[y][x] = pDoc->inputImg[source_y][source_x];
			else
			{
				pDoc->resultImg[y][3 * x + 0] = pDoc->inputImg[source_y][3 * source_x + 0];
				pDoc->resultImg[y][3 * x + 1] = pDoc->inputImg[source_y][3 * source_x + 1];
				pDoc->resultImg[y][3 * x + 2] = pDoc->inputImg[source_y][3 * source_x + 2];
			}
		}
	viewMode = TWO_IMAGES;
	Invalidate();
};

CPoint mpos_st, mpos_end;
void CImageProc20190806View::OnLButtonDown(UINT nFlags, CPoint point)
{
	mpos_st = point;
	CScrollView::OnLButtonDown(nFlags, point);
}

void CImageProc20190806View::OnLButtonUp(UINT nFlags, CPoint point)
{
	mpos_end = point;

	CDC* pDC = GetDC();
	CPen rpen;

	rpen.CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	pDC->SelectObject(&rpen);

	pDC->MoveTo(mpos_st);
	pDC->LineTo(mpos_end);
	ReleaseDC(pDC);

	int Ax, Ay, Bx, By;
	Ax = mpos_st.x;
	Ay = mpos_st.y;
	Bx = mpos_end.x;
	By = mpos_end.y;

	if (Ax < Bx) mctrl_source.Px = Ax - (Bx - Ax) / 2;
	else       mctrl_source.Px = Ax + (Bx - Ax) / 2;
	if (Ay < By) mctrl_source.Py = Ay - (By - Ay) / 2;
	else       mctrl_source.Py = Ay + (By - Ay) / 2;

	mctrl_dest.Px = mctrl_source.Px;
	mctrl_dest.Py = mctrl_source.Py;

	mctrl_source.Qx = mpos_st.x;
	mctrl_source.Qy = mpos_st.y;
	mctrl_dest.Qx = mpos_end.x;
	mctrl_dest.Qy = mpos_end.y;

	CScrollView::OnLButtonUp(nFlags, point);
}

void CImageProc20190806View::OnAviView()
{
	/*CImageProc20190806Doc* pDoc = GetDocument();*/
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Avi화일(*.avi)|*.avi|모든화일|*.*|");	// 옵션 선택(이름) | 실제 파일 필터링(실제파일의 종류

	if (dlg.DoModal() == IDOK) {
		AviFileName = dlg.GetPathName();
		bAviMode = true;
		Invalidate();
	}
}


void CImageProc20190806View::LoadAviFIle(CDC* pDC)
{
	PAVIFILE pavi;	// avi 파일 열어서 처리
	AVIFILEINFO	fi;	// 파일에 대한 정보
	int stm;
	PAVISTREAM pstm = NULL;	// 스트림을 처리하기 위한 변수
	AVISTREAMINFO si;			// 스트림 정보
	PGETFRAME pfrm = NULL;		
	int frame;
	LPBITMAPINFOHEADER pbmpih;	
	unsigned char* image;
	int x, y;

	AVIFileInit();
	AVIFileOpen(&pavi, AviFileName, OF_READ|OF_SHARE_DENY_NONE, NULL);
	AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));

	for (stm = 0; stm < fi.dwStreams; stm++) {
		AVIFileGetStream(pavi, &pstm, 0, stm);
		AVIStreamInfo(pstm, &si, sizeof(si));
		if (si.fccType == streamtypeVIDEO) {
			pfrm = AVIStreamGetFrameOpen(pstm, NULL);	// 한 프레임을 잘라서 가져옴
			for (frame = 0; frame < si.dwLength; frame++) {
				pbmpih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
				if (!pbmpih) continue;
				image = (unsigned char*)((LPSTR)pbmpih + (WORD)pbmpih->biSize);
				/*
				for (y = 0; y < fi.dwHeight; y++)
					for (x = 0; x < fi.dwWidth; x++) {
						pDC->SetPixel(x, fi.dwHeight - 1 - y,
							RGB(image[3 * (y * fi.dwWidth + x) + 2],
								image[3 * (y * fi.dwWidth + x) + 1],
								image[3 * (y * fi.dwWidth + x) + 0]));
					}
				*/
				::SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, fi.dwWidth, fi.dwHeight, 0, 0, 0, fi.dwWidth, image, (BITMAPINFO*)pbmpih, DIB_RGB_COLORS);
				Sleep(33);
			}
		}
	}
	AVIStreamGetFrameClose(pfrm);
	AVIStreamRelease(pstm);
	AVIFileRelease(pavi);
	AVIFileExit();
}

#define NUM_FRAMES 10
void CImageProc20190806View::OnGeometryMorphing()
{
	CImageProc20190806Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	control_line source_line[23] =
	{ {116,7,207,5},{34,109,90,21},{55,249,30,128},{118,320,65,261},
	 {123,321,171,321},{179,319,240,264},{247,251,282,135},{281,114,228,8},
	 {78,106,123,109},{187,115,235,114},{72,142,99,128},{74,150,122,154},
	 {108,127,123,146},{182,152,213,132},{183,159,229,157},{219,131,240,154},
	 {80,246,117,212},{127,222,146,223},{154,227,174,221},{228,252,183,213},
	 {114,255,186,257},{109,258,143,277},{152,278,190,262} };
	control_line dest_line[23] =
	{ {120,8,200,6},{12,93,96,16},{74,271,16,110},{126,336,96,290},
	 {142,337,181,335},{192,335,232,280},{244,259,288,108},{285,92,212,13},
	 {96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
	 {131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
	 {91,268,122,202},{149,206,159,209},{170,209,181,204},{235,265,208,199},
	 {121,280,205,284},{112,286,160,301},{166,301,214,287} };

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;
	double weight;
	double totalWeight;
	double a = 0.001, b = 2.0, p = 0.75;
	unsigned char** warpedImg;
	unsigned char** warpedImg2;
	int frame;
	double fweight;
	control_line warp_lines[23];
	double tx2, ty2, xp2, yp2;
	int dest_x1, dest_y1, dest_x2, dest_y2, source_x2, source_y2;
	int x1, y1, x2, y2, src_x1, src_x2, src_y1, src_y2;
	double src_line_length, dest_line_length;
	int i, j;
	int num_lines = 23;
	int line, x, y, source_x, source_y, last_row, last_col;

	//두 입력 영상을 읽어 들임
	LoadTwoImage();

	// 중간 프레임의 위핑 결과를 저장
	warpedImg = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	warpedImg2 = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->imageHeight; i++) {
		warpedImg[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		warpedImg2[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}
	
	for (i = 0; i < NUM_FRAMES; i++) {
		pDoc->morphedImg[i] = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
		for (j = 0; j < pDoc->imageHeight; j++) 
			pDoc->morphedImg[i][j] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}

	last_col = pDoc->imageWidth - 1;
	last_row = pDoc->imageHeight - 1;

	for (frame = 1; frame <= NUM_FRAMES; frame++) {
		fweight = (double)(frame) / NUM_FRAMES;
		for (line = 0; line < num_lines; line++) {
			warp_lines[line].Px = (int)(source_line[line].Px + (dest_line[line].Px - source_line[line].Px) * fweight);
			warp_lines[line].Py = (int)(source_line[line].Py + (dest_line[line].Py - source_line[line].Py) * fweight);
			warp_lines[line].Qx = (int)(source_line[line].Qx + (dest_line[line].Qx - source_line[line].Qx) * fweight);
			warp_lines[line].Qy = (int)(source_line[line].Qy + (dest_line[line].Qy - source_line[line].Qy) * fweight);
		}
		for (y = 0; y < pDoc->imageHeight; y++) {
			for (x = 0; x < pDoc->imageHeight; x++) {
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;
				tx2 = 0.0;
				ty2 = 0.0;
				for (line = 0; line < num_lines; line++)
				{
					x1 = warp_lines[line].Px;
					y1 = warp_lines[line].Py;
					x2 = warp_lines[line].Qx;
					y2 = warp_lines[line].Qy;
					dest_line_length = sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) / (double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					if (u < 0) d = sqrt((double)(x - x1) * (x - x1) + (y - y1) * (y - y1));
					else if (u > 1) d = sqrt((double)(x - x2) * (x - x2) + (y - y2) * (y - y2));
					else d = fabs(h);

					src_x1 = source_line[line].Px;
					src_y1 = source_line[line].Py;
					src_x2 = source_line[line].Qx;
					src_y2 = source_line[line].Qy;
					src_line_length = sqrt((double)(src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1));

					dest_x1 = dest_line[line].Px;
					dest_y1 = dest_line[line].Py;
					dest_x2 = dest_line[line].Qx;
					dest_y2 = dest_line[line].Qy;
					dest_line_length = sqrt((double)(dest_x2 - dest_x1) * (dest_x2 - dest_x1) + (dest_y2 - dest_y1) * (dest_y2 - dest_y1));

					xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

					xp2 = dest_x1 + u * (dest_x2 - dest_x1) - h * (dest_y2 - dest_y1) / dest_line_length;
					yp2 = dest_y1 + u * (dest_y2 - dest_y1) + h * (dest_x2 - dest_x1) / dest_line_length;
					weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

					tx += (xp - x) * weight;
					ty += (yp - y) * weight;

					tx2 += (xp2 - x) * weight;
					ty2 += (yp2 - y) * weight;

					totalWeight += weight;
				}
				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);

				source_x2 = x + (int)(tx2 / totalWeight + 0.5);
				source_y2 = y + (int)(ty2 / totalWeight + 0.5);

				if (source_x < 0) source_x = 0;
				if (source_x > last_col) source_x = last_col;
				if (source_y < 0) source_y = 0;
				if (source_y > last_row) source_y = last_row;

				if (source_x2 < 0) source_x2 = 0;
				if (source_x2 > last_col) source_x2 = last_col;
				if (source_y2 < 0) source_y2 = 0;
				if (source_y2 > last_row) source_y2 = last_row;
				warpedImg[y][x] = pDoc->inputImg[source_y][source_x];
				warpedImg2[y][x] = pDoc->inputImg2[source_y2][source_x2];
			}
		}

		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++) {
				int val = (int)((1.0 - fweight) * warpedImg[y][x] + fweight * warpedImg2[y][x]);
				if (val < 0) val = 0;
				if (val > 255) val = 255;
				pDoc->morphedImg[frame - 1][y][x] = val;
			}
	}
	viewMode = MORPHING;
	Invalidate(FALSE);
}


void CImageProc20190806View::OnOpencv()
{
	COpenCVDlg dlg;

	dlg.DoModal();	// 다이얼로그 띄우기
}
