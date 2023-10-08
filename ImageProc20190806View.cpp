
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
						pDoc->inputImg[y][3 * x + 1],						// green 성분 3*x+1
						pDoc->inputImg[y][3 * x + 2]));						// blue 성분  3*x+2
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
}

void CImageProc20190806View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
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

			//pDoc->resultImg[y][x] = pDoc->inputImg[y][x] + 50;

			Invalidate(); // 데이터 수정한걸 다시 그리면서 화면에 나타남
		}
}

void CImageProc20190806View::OnPixelSub()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	int x, y, value;
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			value = pDoc->inputImg[y][x] - 50;
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
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			value = pDoc->inputImg[y][x] * 1.5;
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
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			value = pDoc->inputImg[y][x] / 1.5;
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
	int N = 256 * 256;						// N 전체 화소 수 
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
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
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
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
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
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
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

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
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
		for (x = 0; x < pDoc->imageWidth; x++) {
			value = 0.7 * pDoc->inputImg[y][x] + 0.3 * pDoc->inputImg2[y][x];	// 0.* 수정해서 해당 이미지 선명도를 높임
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
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
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
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
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
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
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
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();
}


void CImageProc20190806View::OnRegionEmbosing()
{
	CImageProc20190806Doc* pDoc = GetDocument();

	float kernel[3][3] = { {-1, 0, 0},
					       {0,  0, 0},
						   {0,  0, 1} };
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();
}
