#pragma once
#include "afxdialogex.h"


// CAngleDialog 대화 상자

class CAngleDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAngleDialog)

public:
	CAngleDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAngleDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANGLEINPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iAngle;
};
