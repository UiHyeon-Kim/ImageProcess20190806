#pragma once
#include "afxdialogex.h"


// COpenCVDlg 대화 상자

class COpenCVDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenCVDlg)

public:
	COpenCVDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~COpenCVDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPENCV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFileSelect();
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedButtonCamera();
};
