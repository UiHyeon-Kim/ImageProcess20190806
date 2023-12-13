
// ImageProc20190806Doc.cpp: CImageProc20190806Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageProc20190806.h"
#endif

#include "ImageProc20190806Doc.h"

#include <propkey.h>

#include <string>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageProc20190806Doc

IMPLEMENT_DYNCREATE(CImageProc20190806Doc, CDocument)

BEGIN_MESSAGE_MAP(CImageProc20190806Doc, CDocument)
END_MESSAGE_MAP()


// CImageProc20190806Doc 생성/소멸

CImageProc20190806Doc::CImageProc20190806Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	inputImg = NULL;
	inputImg2 = NULL;
	resultImg = NULL;	// 사용중인지 확인하기 위해
	gResultImg = NULL;
	for (int i = 0; i < 10; i++) morphedImg[i] = NULL;
}

CImageProc20190806Doc::~CImageProc20190806Doc()
{
	if (inputImg != NULL) {
		for (int i = 0; i < imageHeight; i++)
			free(inputImg[i]);	// 메모리 할당 삭제
		free(inputImg);
	}
	if (inputImg2 != NULL) {
		for (int i = 0; i < imageHeight; i++)
			free(inputImg2[i]);
		free(inputImg2);
	}
	if (resultImg != NULL) {
		for (int i = 0; i < imageHeight; i++)
			free(resultImg[i]);		
		free(resultImg);
	}
	if (gResultImg != NULL) {
		for (int i = 0; i < gImageHeight; i++)
			free(gResultImg[i]);
		free(gResultImg);
	}
	for (int i = 0; i < 10; i++) {
		if (morphedImg[i] != NULL) {
			int  j;
			for (j = 0; j < imageHeight; j++)
				free(morphedImg[i][j]);
			free(morphedImg[i]);
		}
	}
}

BOOL CImageProc20190806Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImageProc20190806Doc serialization
// 파일 입출력을 위한 함수
void CImageProc20190806Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())		// 저장하기 선택했는지
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		//ar.Write(resultImg, 256 * 256);
	}
	else	// 로딩을 선택했는지
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		//CFile* fp = ar.GetFile();										// ar는 받아온 정보 / 현재 사용한 파일 포인터를 이용해 불러옴
		//if (fp->GetLength() == 256 * 256) ar.Read(inputImg, 256 * 256); // 지금 선택한 파일의 크기를 불러옴 // ar읽어서 저장할 메모리의 포인터
		//else AfxMessageBox("256X256 크기의 파일만 사용가능합니다.");	// 에러메시지 출력 함수

		LoadImageFile(ar);
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageProc20190806Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageProc20190806Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageProc20190806Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageProc20190806Doc 진단

#ifdef _DEBUG
void CImageProc20190806Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProc20190806Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProc20190806Doc 명령


void CImageProc20190806Doc::LoadImageFile(CArchive& ar)	//Serialize를 통해 열림
{
	// TODO: 여기에 구현 코드 추가.
	int maxValue;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();	// CStrung - 문자열을 다루는 여러 기능 포함// fp-> 함수 이름 넘김
	bool isbmp = false;

	// strcmp : 문자열 비교 / strrchr : 
	
	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0) 
	{
		ar.ReadString(type, 15);	// type 변수 15글자 읽음 마지막 하나는 null값
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');	// #으로 시작하면 버림
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);	// sscanf 문자열(버퍼)에 들어간걸 처리 (저장된 변수, 숫자로 바꿈, 숫자
		
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');	// #으로 시작하면 버림
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) depth = 1;	// 흑백
		else depth = 3;							// 컬러
	}
	else if (strcmp(strrchr(fname, '.'), ".bmp") == 0 || strcmp(strrchr(fname, '.'), ".BMP") == 0) {
		// bitmap file header 읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		// BMP 파일임을 나타내는 "BM" 마커가 있는지 확인
		if (bmfh.bfType != (WORD)('B' | ('M' << 8))) return;
		// bitmap info 읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));

		imageHeight = bih.biHeight;
		imageWidth = bih.biWidth;
		depth = bih.biBitCount / 8;

		if (depth == 1) {
			// 팔레트 존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strrchr(fname, '.'), ".raw") == 0 || strcmp(strrchr(fname, '.'), ".RAW") == 0)
	{
		if (fp ->GetLength()!= 256*256) {
			AfxMessageBox("256x256 크기의 파일만 가능합니다.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}
	// 메모리 할당
	//메모리 얼로케이션?
	//inputImg : unsigned char 포인터의 포인터
	inputImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	resultImg= (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < imageHeight; i++) {
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		inputImg[i] = (unsigned char*)malloc(imageWidth * depth);
		resultImg[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	// 영상데이터 읽기
	if (!isbmp) {
		for (int i = 0; i < imageHeight; i++) {
			ar.Read(inputImg[i], imageWidth * depth);	//ar.Read는 아스키 코드 문자열을 읽음
		}
	}
	else {
		// 파일에서 읽어서 저장
		BYTE nu[4];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (int i = 0; i < imageHeight; i++) {
			if (depth == 1)
				ar.Read(inputImg[imageHeight - 1 - i], imageWidth * depth);
			else {
				for (size_t j = 0; j < imageWidth; j++) {
					BYTE r, g, b;
					ar.Read(&b, 1); ar.Read(&g, 1); ar.Read(&r, 1);
					inputImg[imageHeight - 1 - i][3 * j + 0] = r;
					inputImg[imageHeight - 1 - i][3 * j + 1] = g;
					inputImg[imageHeight - 1 - i][3 * j + 2] = b;
				}
			}
			if (widthfile - imageWidth != 0) 
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}
	}
}



void CImageProc20190806Doc::LoadSecondImageFile(CArchive& ar)
{
	int i, maxValue;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();	// CStrung - 문자열을 다루는 여러 기능 포함// fp-> 함수 이름 넘김
	bool isbmp = false;

	int imgw, imgh, imgd;

	// strcmp : 문자열 비교 / strrchr : 
	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0)
	{
		ar.ReadString(type, 15);	// type 변수 15글자 읽음 마지막 하나는 null값
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');	// #으로 시작하면 버림
		sscanf_s(buf, "%d %d", &imgw, &imgh);	// sscanf 문자열(버퍼)에 들어간걸 처리 (저장된 변수, 숫자로 바꿈, 숫자

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');	// #으로 시작하면 버림
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) imgd = 1;	// 흑백
		else imgd = 3;							// 컬러
	}
	else if (strcmp(strrchr(fname, '.'), ".bmp") == 0 || strcmp(strrchr(fname, '.'), ".BMP") == 0) {
		// bitmap file header 읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		// BMP 파일임을 나타내는 "BM" 마커가 있는지 확인
		if (bmfh.bfType != (WORD)('B' | ('M' << 8))) return;
		// bitmap info 읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));

		imgh = bih.biHeight;
		imgw = bih.biWidth;
		imgd = bih.biBitCount / 8;

		if (imgd == 1) {
			// 팔레트 존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strrchr(fname, '.'), ".raw") == 0 || strcmp(strrchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox("256x256 크기의 파일만 가능합니다.");
			return;
		}

		imgw = 256;
		imgh = 256;
		imgd = 1;
	}

	if (imgw != imageWidth || imgh != imageHeight || imgd != depth) {
		AfxMessageBox("동일한 크기의 화일만 읽어들일 수 있습니다.");
		return;
	}

	// 메모리 할당
	//메모리 얼로케이션?
	//inputImg : unsigned char 포인터의 포인터
	inputImg2 = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (int i = 0; i < imageHeight; i++) {
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		inputImg2[i] = (unsigned char*)malloc(imageWidth * depth);
	}
	 
	// 영상데이터 읽기
	if (!isbmp) {
		for (int i = 0; i < imageHeight; i++) 
			ar.Read(inputImg2[i], imageWidth * depth);	//ar.Read는 아스키 코드 문자열을 읽음
	}
	else {
		// 파일에서 읽어서 저장
		BYTE nu[4];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (int i = 0; i < imageHeight; i++) {
			if (depth == 1)
				ar.Read(inputImg2[imageHeight - 1 - i], imageWidth * depth);
			else {
				for (size_t j = 0; j < imageWidth; j++) {
					BYTE r, g, b;
					ar.Read(&b, 1); ar.Read(&g, 1); ar.Read(&r, 1);
					inputImg2[imageHeight - 1 - i][3 * j + 0] = r;
					inputImg2[imageHeight - 1 - i][3 * j + 1] = g;
					inputImg2[imageHeight - 1 - i][3 * j + 2] = b;
				}
			}
			if (widthfile - imageWidth != 0)
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}
	}
}