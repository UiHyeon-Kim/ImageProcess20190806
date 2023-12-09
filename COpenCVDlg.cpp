// COpenCVDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProc20190806.h"
#include "afxdialogex.h"
#include "COpenCVDlg.h"

#include "opencv2/opencv.hpp"

// COpenCVDlg 대화 상자

IMPLEMENT_DYNAMIC(COpenCVDlg, CDialogEx)

COpenCVDlg::COpenCVDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPENCV, pParent)
{

}

COpenCVDlg::~COpenCVDlg()
{
}

void COpenCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COpenCVDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SELECT, &COpenCVDlg::OnBnClickedButtonFileSelect)
	ON_BN_CLICKED(IDC_BUTTON__VIDEO, &COpenCVDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA, &COpenCVDlg::OnBnClickedButtonCamera)
END_MESSAGE_MAP()


// COpenCVDlg 메시지 처리기


void COpenCVDlg::OnBnClickedButtonFileSelect()
{
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "JPG화일(*.jpg)|*.jpg|모든화일(*.*)|*.*|");
	if (dlg.DoModal() == IDOK) {	// 파일 선택창 열어줌
		cv::Mat frame;	
		frame = cv::imread((LPSTR)(LPCTSTR)dlg.GetPathName(), 1);

		cv::imshow("Image", frame);
		cv::waitKey(0);	// 바로 꺼지는걸 정지 시켜둠 / 0 = 무한, 1 이상은 숫자만큼 정지
	}
}


void COpenCVDlg::OnBnClickedButton()
{
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "AVI화일(*.avi)|*.avi|모든화일(*.*)|*.*|");
	if (dlg.DoModal() == IDOK) {	// 파일 선택창 열어줌
		cv::VideoCapture cap((LPSTR)(LPCTSTR)dlg.GetPathName());	// 동영상 가져옴

		cv::Mat frame;

		while (1) {
			cap.read(frame);	// 한 프레임 읽기
			if (frame.empty()) break;

			cv::imshow("Video", frame);	// 한 프레임 출력
			if (cv::waitKey(30) == 'q') break;
		}
		cv::destroyWindow("Video");	// 창 끄기
	}
}


void COpenCVDlg::OnBnClickedButtonCamera()
{
	cv::VideoCapture cap(0);

	cv::Mat frame;

	while (1) {
		cap.read(frame);	// 한 프레임 읽기
		if (frame.empty()) break;

		cv::imshow("Camera", frame);	// 한 프레임 출력
		if (cv::waitKey(30) == 'q') break;
	}
	cv::destroyWindow("Camera");	// 창 끄기
}
