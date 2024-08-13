// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()

// CMFCApplication1Dlg 对话框

CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_conver1, btn1);
	DDX_Control(pDX, IDC_EDT1, EDT1);
	DDX_Control(pDX, IDC_EDT2, EDT2);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_conver1, &CMFCApplication1Dlg::OnBnClickedconver1)
	ON_BN_CLICKED(IDC_conver2, &CMFCApplication1Dlg::OnBnClickedconver2)
END_MESSAGE_MAP()

// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CMFCApplication1Dlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 1) // CTRL+A
	{
		CWnd* pFocus = GetFocus();
		if (pFocus != NULL)
		{
			if (pFocus == &EDT1 || pFocus == &EDT2)
			{
				((CEdit*)pFocus)->SetSel(0, -1);
				return;
			}
		}
	}

	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

std::string GetEditText(CEdit* pEditCtrl) {
	CString cstr;
	pEditCtrl->GetWindowText(cstr);
	return std::string(cstr.GetString());
}

std::vector<std::string> SplitString(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter)) {
		if (!token.empty()) {
			tokens.push_back(token);
		}
	}
	return tokens;
}

std::string MaskAndReplace(const std::string& hexStr, const std::string& mask) {
	std::vector<std::string> hexTokens = SplitString(hexStr, '\\');
	std::stringstream result;
	size_t maskIndex = 0;

	for (const auto& token : hexTokens) {
		if (token.length() == 3 && token[0] == 'x') {
			if (maskIndex >= mask.length()) {
				throw std::runtime_error("Mask length does not match hex string length");
			}

			int hexValue = std::stoi(token.substr(1), nullptr, 16);

			while (maskIndex < mask.length() && std::isspace(mask[maskIndex])) {
				maskIndex++; // 跳过掩码中的空格
			}

			if (maskIndex >= mask.length()) {
				throw std::runtime_error("Mask length does not match hex string length after skipping spaces");
			}

			if (mask[maskIndex] == 'x') {
				result << std::setw(2) << std::setfill('0') << std::hex << hexValue << " ";
			}
			else if (mask[maskIndex] == '?') {
				result << "?? ";
			}
			else {
				throw std::runtime_error("Invalid mask character: " + std::string(1, mask[maskIndex]));
			}

			maskIndex++;
		}
	}

	// 移除末尾的空格
	std::string finalResult = result.str();
	if (!finalResult.empty() && finalResult.back() == ' ') {
		finalResult.pop_back();
	}

	// 确保结果全部是小写
	std::transform(finalResult.begin(), finalResult.end(), finalResult.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return finalResult;
}
std::string ReverseConversion(const std::string& idaPattern) {
	std::vector<std::string> tokens = SplitString(idaPattern, ' ');
	std::stringstream result;
	std::stringstream maskResult;

	for (const auto& token : tokens) {
		if (token == "??") {
			result << "\\x00";
			maskResult << "?";
		}
		else {
			result << "\\x" << token;
			maskResult << "x";
		}
	}

	return "\"" + result.str() + "\",\n\"" + maskResult.str() + "\"";
}

void CMFCApplication1Dlg::OnBnClickedconver1()
{
	try {
		const auto input = GetEditText(&EDT1);
		size_t commaPos = input.find(',');
		if (commaPos == std::string::npos) {
			throw std::runtime_error("Invalid input format. Expected 'hex_string,mask'");
		}

		std::string hexStr = input.substr(0, commaPos);
		std::string mask = input.substr(commaPos + 1);

		// 移除引号和多余的空白字符
		hexStr.erase(remove(hexStr.begin(), hexStr.end(), '"'), hexStr.end());
		mask.erase(remove(mask.begin(), mask.end(), '"'), mask.end());
		mask.erase(remove(mask.begin(), mask.end(), ' '), mask.end());

		const auto strr = MaskAndReplace(hexStr, mask);
		EDT2.SetWindowText(CString(strr.c_str()));
	}
	catch (const std::exception& e) {
		AfxMessageBox(CString(e.what()), MB_OK | MB_ICONERROR);
	}
}

void CMFCApplication1Dlg::OnBnClickedconver2()
{
	try {
		const auto idaPattern = GetEditText(&EDT2);
		const auto reversedStr = ReverseConversion(idaPattern);
		EDT1.SetWindowText(CString(reversedStr.c_str()));
	}
	catch (const std::exception& e) {
		AfxMessageBox(CString(e.what()), MB_OK | MB_ICONERROR);
	}
}