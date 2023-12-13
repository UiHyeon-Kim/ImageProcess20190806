
// ImageProc20190806View.h: CImageProc20190806View 클래스의 인터페이스
//

#pragma once


class CImageProc20190806View : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CImageProc20190806View() noexcept;
	DECLARE_DYNCREATE(CImageProc20190806View)

// 특성입니다.
public:
	CImageProc20190806Doc* GetDocument() const;
	int viewMode;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CImageProc20190806View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPixelAdd();
	afx_msg void OnPixelSub();
	afx_msg void OnPixelMul();
	afx_msg void OnPixelDiv();
	afx_msg void OnPixelHistoEq();
	afx_msg void OnPixelContrastStretching();
	afx_msg void OnPixelBinarization();
	afx_msg void OnPixelTwoImageAdd();
	void LoadTwoImage();
	afx_msg void OnPixelTwoImageSub();
	afx_msg void OnPixelTwoImageMul();
	afx_msg void OnPixelTwoImageDiv();
//	afx_msg void OnTest();
	afx_msg void OnRegionSmoothing();
	void Convolve(unsigned char** Inimg, unsigned char** Outimg, int cols, int rows, float mask[][3], int bias, int depth);
	afx_msg void OnRegionShrapening();
	afx_msg void OnRegionEmbosing();
	afx_msg void OnRegionPrewitt();
	afx_msg void OnRegionRoberts();
	afx_msg void OnRegionSobel();
	afx_msg void OnRegionAverageFiltering();
	afx_msg void OnRegionMedianFiltering();
	afx_msg void OnMopologyColorGray();
	afx_msg void OnMopologyBinarization();
	afx_msg void OnMopologyErosion();
	afx_msg void OnMopologyDilation();
	afx_msg void OnMopologyOpening();
	afx_msg void OnMopologyClosing();
	void CopyResultToInput();
	afx_msg void OnGeometryZoominPixelcopy();
	afx_msg void OnGeometryZoominBilinarInterpolation();
	afx_msg void OnGeometryZoomoutSubsampling();
	afx_msg void OnGeometryZoomoutMeanSub();
	afx_msg void OnGeometryZoomoutAvgFiltering();
	afx_msg void OnGeometryRotation();
	afx_msg void OnGeometryVerticalFlip();
	afx_msg void OnGeometryHolizantalFlip();
	afx_msg void OnGeometryWarping();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAviView();
	bool bAviMode;
	CString AviFileName;
	void LoadAviFIle(CDC* pDC);
	afx_msg void OnGeometryMorphing();
};

#ifndef _DEBUG  // ImageProc20190806View.cpp의 디버그 버전
inline CImageProc20190806Doc* CImageProc20190806View::GetDocument() const
   { return reinterpret_cast<CImageProc20190806Doc*>(m_pDocument); }
#endif

