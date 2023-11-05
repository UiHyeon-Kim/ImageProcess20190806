﻿
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
END_MESSAGE_MAP()

// CImageProc20190806View 생성/소멸

CImageProc20190806View::CImageProc20190806View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

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
	CImageProc20190806Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// pDC->TextOutA(100, 100, _T("테스트출력"));

	int x, y;
	if (pDoc->inputImg != NULL) {
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
	}
	if (pDoc->resultImg != NULL) {
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
	if (pDoc->inputImg2 != NULL){
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
	}

	if (pDoc->gResultImg != NULL) {
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
	Invalidate();
}

void CImageProc20190806View::LoadTwoImage()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);
	CFile file;

	if (dlg.DoModal() == IDOK) {	// DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소

		file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
		CArchive ar(&file, CArchive::load);
		ar.Read(pDoc->inputImg2, 256 * 256);
		//file.Read(pDoc->inputImg2, 256 * 256);			// 파일 읽어서 inputImage2에 삽입
		file.Close();									// 파일 닫기

		int x, y;
		for (y = 0; y < 256; y++) {
			for (x = 0; x < 256; x++) {
				pDoc->resultImg[y][x] = pDoc->inputImg[y][x];	// 결과 이미지를 초기화
			}
		}
		Invalidate();
		// dlg.GetPathName();	// 경로명 파일이름 확장자 등 다 넘김
		// dlg.GetFileName();	// 파일 이름만 넘김
		// dlg.GetFileExt();	// 파일 경로만 넘김
		// dlg.GetFileTitle();	// 파일 확장자만 넘김
	}
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
	Invalidate();
}


void CImageProc20190806View::OnRegionSmoothing()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float kernel[3][3] = { {1/9.0,1/9.0,1/9.0},
						   {1/9.0,1/9.0,1/9.0},
						   {1/9.0,1/9.0,1/9.0} };
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);
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

	Invalidate();
}


void CImageProc20190806View::OnRegionEmbosing()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float kernel[3][3] = { {-1,0, 0},
					       {0, 0, 0},
						   {0, 0, 1} };
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

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
	Invalidate();
}