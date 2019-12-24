
// demo1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "demo1.h"
#include "demo1Dlg.h"
#include "afxdialogex.h"
#include "types_c.h"
#include<fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
//#include<vector>
//#include<fstream>
//#include<opencv2/ml/ml.hpp>
#include <math.h>
#define _MATH_DEFINES_DEFINED
#define M_PI       3.14159265358979323846
using namespace std;
using namespace cv;
using namespace ml;
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


// Cdemo1Dlg 对话框



Cdemo1Dlg::Cdemo1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEMO1_DIALOG, pParent)
	, m_bRun(FALSE)
	, m_hCam(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cdemo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cdemo1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenCam, &Cdemo1Dlg::OnBnClickedOpencam)
	ON_BN_CLICKED(IDC_StartGrab, &Cdemo1Dlg::OnBnClickedStartgrab)
	ON_BN_CLICKED(IDC_CloseCam, &Cdemo1Dlg::OnBnClickedClosecam)
	ON_BN_CLICKED(IDOK, &Cdemo1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Cdemo1Dlg::OnBnClickedCancel)//需要在这里加上ON BN CLICKED函数
	ON_BN_CLICKED(IDC_Save, &Cdemo1Dlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOpenimage, &Cdemo1Dlg::OnBnClickedOpenimage)
	ON_EN_CHANGE(IDC_SHOW, &Cdemo1Dlg::OnEnChangeShow)
	ON_BN_CLICKED(IDC_fenge, &Cdemo1Dlg::OnBnClickedfenge)
	ON_BN_CLICKED(IDfenimage, &Cdemo1Dlg::OnBnClickedfenimage)
END_MESSAGE_MAP()


// Cdemo1Dlg 消息处理程序

BOOL Cdemo1Dlg::OnInitDialog()
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
	MVSTATUS_CODES r;
	r = MVInitLib();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("函数库初始化失败！"), _T("提示"), MB_ICONWARNING);
		return TRUE;
	}
	r = MVUpdateCameraList();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("查找连接计算机的相机失败！"),_T("提示"), MB_ICONWARNING);
		return TRUE;
	}
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Save)->EnableWindow(false);
	GetDlgItem(IDOpenimage)->EnableWindow(true);
	GetDlgItem(IDC_fenge)->EnableWindow(true);
	GetDlgItem(IDfenimage)->EnableWindow(true);
	//cvNamedWindow("src", CV_WINDOW_AUTOSIZE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cdemo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cdemo1Dlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cdemo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void Cdemo1Dlg::OnBnClickedOpencam()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox(_T(" 没有找到相机 , 请确认连接和相机 IP 设置"), _T("提示"),
			MB_ICONWARNING); 
			return;
	}

	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox(_T("无法打开相机,可能正被别的软件控制"), _T("提示"),
				MB_ICONWARNING);
		else
			MessageBox(_T("无法打开相机"), _T("提示"), MB_ICONWARNING);
		return;
	}
	int w, h;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &m_PixelFormat);
	m_image.CreateByPixelFormat(w, h, m_PixelFormat);
	//cvNamedWindow("src", CV_WINDOW_AUTOSIZE);

	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Save)->EnableWindow(false);
	GetDlgItem(IDOpenimage)->EnableWindow(true);
	GetDlgItem(IDC_fenge)->EnableWindow(true);
	GetDlgItem(IDfenimage)->EnableWindow(true);
}

void Cdemo1Dlg::DrawImage()
{
	CRect rct;
	GetDlgItem(pic)->GetClientRect(&rct);
	int dstW = rct.Width();
	int dstH = rct.Height();

	CDC* pDC = GetDlgItem(pic)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		m_image.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
	}
	ReleaseDC(pDC);
}

int Cdemo1Dlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{
	MVInfo2Image(m_hCam, pInfo, &m_image);
	DrawImage();
	return 0;
}

int __stdcall StreamCB(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	Cdemo1Dlg* pDlg = (Cdemo1Dlg*)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}



void Cdemo1Dlg::OnBnClickedStartgrab()
{
	// TODO: 在此添加控件通知处理程序代码
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}

	MVStartGrab(m_hCam, StreamCB, (ULONG_PTR)this);
	m_bRun = true;

	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);
	GetDlgItem(IDC_Save)->EnableWindow(true);
	GetDlgItem(IDOpenimage)->EnableWindow(true);
	GetDlgItem(IDC_fenge)->EnableWindow(true);
	GetDlgItem(IDfenimage)->EnableWindow(true);
	//获取原始图像数据，转成图像格式进行显示：
		
}



void Cdemo1Dlg::OnClose()
{
	if (m_bRun != false)
	{
		MVStopGrab(m_hCam);
	}
	MVTerminateLib();

	CDialog::OnClose();
}

void Cdemo1Dlg::OnBnClickedClosecam()
{
	// TODO: 在此添加控件通知处理程序代码
	MVStopGrab(m_hCam);
	MVCloseCam(m_hCam);
	m_bRun = false;
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Save)->EnableWindow(false);
	GetDlgItem(IDOpenimage)->EnableWindow(true);
	GetDlgItem(IDC_fenge)->EnableWindow(true);
	GetDlgItem(IDfenimage)->EnableWindow(true);
}


void Cdemo1Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void Cdemo1Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void Cdemo1Dlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	DrawImage();
	m_image.Save("I:/TEST/BMP");
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);
	GetDlgItem(IDC_Save)->EnableWindow(true);
	GetDlgItem(IDOpenimage)->EnableWindow(true);
	GetDlgItem(IDC_fenge)->EnableWindow(true);
	GetDlgItem(IDfenimage)->EnableWindow(true);
}




void countpt(const Mat& image, Mat& pt,int a,int b)
{
	int nr = image.rows;
	int nl = image.cols;
	int countx = nr / 2 - a;
	int county = nl / 2 - b;
	for (int i = 0; i < nr ; i++)
	{
		const float* im = image.ptr<float>(i);
		for (int j = 0; j < nl ; j++)
		{
			int x = i + countx;
			int y = j + county;
			if (im[j] > 50)
			{
				if (x >= 0 && x < nr && y >= 0 && y < nl)
				{
					float* pt_im = pt.ptr<float>(x);
					pt_im[y] = 255;
				}
			}

		}
	}
}

void change(Mat& p)
{
	int nr = p.rows;
	int nl = p.cols;
	for (int i = 0; i < nr; i++)
	{
		float* im = p.ptr<float>(i);
		for (int j = 0; j < nl; j++)
		{
			if (im[j] > 100)
			{
				im[j] = 0;
			}
			else
			{
				im[j] = 255;
			}
		}
	}
}
template <class T>
int getArrayLen(T& array)
{
	return sizeof(array) / sizeof(array[0]);
}
int findmax(double X[])
{
	int length = 11;
	int min=0;
	for (int i = 0; i < length; i++)
	{
		if (X[i] <= X[min])
			min = i;
	}
	return min;
 }
int findmin(double X[],int length)
{
	int min = 0;
	for (int i = 0; i < length; )
	{
		if (X[i] <= X[min])
		{
			min = i;
		}
		i = i + 1;
	}
	return min;
}
char *getname(int number,double area)
{
	if (abs(area - 0.386518) < 0.05)
		return "luomu";
	else {
		switch (number)
		{
		case 0:
			return "duanjian";
			//return "短尖螺丝";
			break;
		case 1:
			return "changjian";
			//return "长尖螺丝";
			break;
		case 2:
			return "changjian";
			//return "长尖螺丝";
			break;
		case 3:
			return "luomu";
			//return "螺母";
			break;
		case 4:
			return "wanxing";
			//return "弯型";
			break;
		case 5:
			return "yingbi";
			//return "硬币";
			break;
		case 6:
			return "duanyuan";
			//return "短圆螺丝";
			break;
		case 7:
			return "duanyuan";
			//return "短圆螺丝";
			break;
		case 8:
			return "changyuan";
			//return "长圆螺丝";
			break;
		case 9:
			return "changyuan";
			//return "长圆螺丝";
			break;
		case 10:
			return "duanchangjian";
			//return "短长尖螺丝";
			break;
		}
	}
}
double countarea(Mat x)
{
	int nr = x.rows;
	int nl = x.cols;
	int count = 0;
	for (int i = 0; i < nr; i++)
	{
		float *p=x.ptr<float>(i);
		for (int j = 0; j < nl; j++)
		{
			if (p[j] > 50)
				count++;
		}
	}
	return count;
}
void theresult(Mat& image, Mat &src,double yuanmianji,double HU[][7],double area[])//输入参数为iamge（CU_8U单通道），src（三通道），圆面积的像素值，样本的area
{
	Mat imchange;
	image.convertTo(imchange, CV_8U);
	imchange.convertTo(imchange, CV_32FC1);
	double areat;
	areat = countarea(imchange)/yuanmianji*4.90625;
	Moments moments1 = cv::moments(imchange, false);
	double huMoments1[7];
	HuMoments(moments1, huMoments1);
	Point p(moments1.m10 / moments1.m00, moments1.m01 / moments1.m00);
	for (int j = 0; j < 7; j++)
	{
		huMoments1[j] = -1 * copysign(1.0, huMoments1[j]) * log10(abs(huMoments1[j]));
	}
	double SUM[11];
	for (int i = 0; i < 11; i++)
	{
		if (i != 3)
		{
			SUM[i] = (6 * (HU[i][0] - huMoments1[0]) * (HU[i][0] - huMoments1[0]) + 10 * (HU[i][1] - huMoments1[1]) * (HU[i][1] - huMoments1[1]) + 0.01 * (HU[i][2] - huMoments1[2]) * (HU[i][2] - huMoments1[2]) + 0.01 * (HU[i][3] - huMoments1[3]) * (HU[i][3] - huMoments1[3]) + 100 * (areat - area[i]) * (areat - area[i]) ) / 5;
		}
		else
			SUM[i] = 1000000;
	}
	int local = findmax(SUM);
	const char* name = getname(local, areat);

	putText(src, name, p, FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 23, 0), 4, 8);
}

void Cdemo1Dlg::OnBnClickedOpenimage()
{
	m_image.Load("I:/7.jpg");
	DrawImage();
	//imread("I:/111.jpg");
	ofstream ofile;
	ofile.open("I:/Test.txt");
	double HU[11][7];
	double area[11];
	for (int i = 1; i <=11; i++)
	{
		string path = "I:/" + to_string(i)+ to_string(i) + ".jpg";
		Mat im1 = imread(path, IMREAD_GRAYSCALE);
		
		threshold(im1, im1, 0, 255, CV_THRESH_OTSU);
		//imshow(path, im1);
		im1.convertTo(im1, CV_32FC1);
		change(im1);
		area[i - 1] = countarea(im1);
		im1.convertTo(im1, CV_8U);
		Moments moments1 = cv::moments(im1, false);
		double huMoments1[7];
		HuMoments(moments1, huMoments1);
		for (int j = 0; j < 7; j++)
		{
			huMoments1[j] = -1 * copysign(1.0, huMoments1[j]) * log10(abs(huMoments1[j]));
			HU[i-1][j] = huMoments1[j];
		}
	}
	for (int i = 0; i < 11; i++)
	{
		ofile << "第" << i+1 << "个图片";
		ofile << "面积为" << area[i]/area[5]*4.90625<<"\r\n";
		for (int j = 0; j < 7; j++)
		{
			ofile << HU[i][j] << "\r\n";
		}
	}
	ofstream file1;
	file1.open("I:/angle.txt");
	for (int i = 1; i <= 8; i++)
	{
		for (int m = 1; m <= 3; m++)
		{
			string path1 = "I:/angle/" + to_string(i) + to_string(m) + ".BMP";
			Mat im1 = imread(path1, IMREAD_GRAYSCALE);
			Mat im2 = imread(path1);
			threshold(im1, im1, 0, 255, CV_THRESH_OTSU);
			//imshow(path, im1);
			im1.convertTo(im1, CV_32FC1);
			change(im1);
			//imshow(path, im1);
			Mat H = Mat::zeros(300, 300, CV_32FC1);
			im1.convertTo(im1, CV_8U);
			Moments moments1 = cv::moments(im1, false);
			double huMoments1[7];
			HuMoments(moments1, huMoments1);
			file1 << path1<<"\r\n";
			for (int j = 0; j < 7; j++)
			{
				huMoments1[j] = -1 * copysign(1.0, huMoments1[j]) * log10(abs(huMoments1[j]));
				file1 << huMoments1[j] << "\r\n";
			}
		}
	}
	file1.close();
	//ofile.close();
	Mat imagemain = imread("I:/angle/71.bmp", IMREAD_GRAYSCALE);
	threshold(imagemain, imagemain, 0, 255, CV_THRESH_OTSU);
	imagemain.convertTo(imagemain, CV_32FC1);
	change(imagemain);
	double biaozhun;
	biaozhun = countarea(imagemain);
	for (int m = 1; m <= 8; m++)
	{
		string path = "I:/angle/" + to_string(m) + "1.bmp";
		Mat image = imread(path, IMREAD_GRAYSCALE);
		Mat image1 = imread(path);
		threshold(image, image, 0, 255, CV_THRESH_OTSU);
		//imshow("", image);
		//imshow(path, im1);
		image.convertTo(image, CV_32FC1);
		change(image);
		double areat;
		areat = countarea(image)*area[4]/biaozhun;
		double area1 = areat / area[5] * 4.90625;
		image.convertTo(image, CV_8U);
		//测试
		//vector<vector<Point>> contours;
		//vector<Vec4i> hierarchy;
		//findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
		//	Mat imageContours = Mat::zeros(image.size(), CV_8UC1);	//最小外接矩形画布
		//for (int i = 0; i < contours.size(); i++)
		//{
		//	//绘制轮廓
		//	//drawContours(imageContours, contours, i, Scalar(255), 1, 8, hierarchy);



		//	//绘制轮廓的最小外结矩形
		//	RotatedRect rect = minAreaRect(contours[i]);
		//	Point2f P[4];
		//	rect.points(P);
		//	for (int j = 0; j <= 3; j++)
		//	{
		//		line(imageContours, P[j], P[(j + 1) % 4], Scalar(255), 2);
		//	}
		//}
		//imshow("MinAreaRect", imageContours);
		//
		//imshow(path, im1);
		image.convertTo(image, CV_8U);
		Moments moments1 = cv::moments(image, false);
		double huMoments1[7];
		HuMoments(moments1, huMoments1);
		Point p(moments1.m10 / moments1.m00, moments1.m01 / moments1.m00);
		for (int j = 0; j < 7; j++)
		{
			huMoments1[j] = -1 * copysign(1.0, huMoments1[j]) * log10(abs(huMoments1[j]));
		}
		double SUM[11];
		for (int i = 0; i < 11; i++)
		{
			if (i != 3)
			{
				SUM[i] = (6 * (HU[i][0] - huMoments1[0]) * (HU[i][0] - huMoments1[0]) + 10 * (HU[i][1] - huMoments1[1]) * (HU[i][1] - huMoments1[1]) + 0.01 * (HU[i][2] - huMoments1[2]) * (HU[i][2] - huMoments1[2]) + 0.01 * (HU[i][3] - huMoments1[3]) * (HU[i][3] - huMoments1[3]) + 100 * (areat - area[i]) * (areat - area[i]) / area[5] * 4.90625 / area[5] * 4.90625) / 5;
			}
			else
				SUM[i] = 1000000;
		}
		int local = findmax(SUM);
		const char* name = getname(local,area1);

		putText(image1, name, p, FONT_HERSHEY_SIMPLEX, 1, Scalar(255,23,0), 4, 8);
		//imshow("", image1);
		//waitKey(0);
		for (int n = 0; n < 7; n++)
		{
			//ofile << SUM[n] << "  ";
			ofile << huMoments1[n]<<"  ";
		}
		ofile << "\n";
		ofile << name<<"面积："<<area1<<"\r\n";
	}
	ofile.close();
	////double d1 = matchShapes(im1, im2, CONTOURS_MATCH_I1, 0);//相似度
	////double d2 = matchShapes(im1, im2, CONTOURS_MATCH_I2, 0);
	////double d3 = matchShapes(im1, im2, CONTOURS_MATCH_I3, 0);
}



void Cdemo1Dlg::OnEnChangeShow()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//void transform(Mat& src, Mat& dst,vector<Point>v)
//{
//	int row = src.rows;
//	int col = src.cols;
//	for (int i = 0; i < row; i++)
//	{
//		float* im = src.ptr<float>(i);
//		float* im1 = dst.ptr<float>(i);
//		for (int j = 0; j < col; j++)
//		{
//
//		}
//	}
//}
void Cdemo1Dlg::OnBnClickedfenge()
{
	UINT dwTimerId;
	dwTimerId = SetTimer(1, 20000, NULL);
	MSG msg;
	unsigned int q = 0;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (q > 9)
		{
			KillTimer(1);
			dwTimerId = SetTimer(1, 10000, NULL);
			break;
		}
		else if (msg.message == WM_TIMER)
		{
			double area[11];//样本集的面积
			double HU[11][7];
			ofstream file1;
			file1.open("I:/1.txt");
			for (int i = 1; i <= 11; i++)
			{
				string path = "I:/" + to_string(i) + to_string(i) + ".jpg";
				Mat im1 = imread(path, IMREAD_GRAYSCALE);

				threshold(im1, im1, 0, 255, CV_THRESH_OTSU);
				//imshow(path, im1);
				im1.convertTo(im1, CV_32FC1);
				change(im1);
				area[i - 1] = countarea(im1);
				Moments moments1 = cv::moments(im1, false);
				double huMoments1[7];
				HuMoments(moments1, huMoments1);
				for (int j = 0; j < 7; j++)
				{
					huMoments1[j] = -1 * copysign(1.0, huMoments1[j]) * log10(abs(huMoments1[j]));
					HU[i - 1][j] = huMoments1[j];
				}
			}
			double yuanmianji = area[5];
			for (int i = 0; i < 11; i++)
			{
				area[i] = area[i] / yuanmianji * 4.90625;
			}
			string path = "H:/相机/" + to_string(q) + ".bmp";
			Mat imagemain1 = imread(path);
			Mat imagemain = imread(path, IMREAD_GRAYSCALE);
			GaussianBlur(imagemain, imagemain, cv::Size(5, 5), 3, 3);
			threshold(imagemain, imagemain, 0, 255, CV_THRESH_OTSU);
			imagemain.convertTo(imagemain, CV_32FC1);
			change(imagemain);
			imagemain.convertTo(imagemain, CV_8U);
			vector<vector<Point>>contours;
			vector<Vec4i>hierarchy;
			findContours(imagemain, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());//查找轮廓
			double point[20][2];
			int tt = 0;
			int flag[40] = { 0 };//标记
			double newarea[40];
			for (int i = 0; i < contours.size(); i++)
			{
				Mat im = Mat::zeros(imagemain.rows, imagemain.cols, CV_8U);
				Scalar color(255);
				drawContours(im, contours, i, color, CV_FILLED, 8, hierarchy);//用来画轮廓呀呀呀
				Moments moment = moments(im, false);
				if (moment.m00 != 0)//除数不能为0
				{
					point[i][0] = cvRound(moment.m10 / moment.m00);//计算重心横坐标
					point[i][1] = cvRound(moment.m01 / moment.m00);//计算重心纵坐标
				}
				im.convertTo(im, CV_32FC1);
				newarea[i] = countarea(im);
				if (newarea[i] < 2400)
				{
					newarea[i] = 0;
					flag[i] = 5;//噪点或者轮廓与轮廓加的地方
				}
			}
			Point circle_center;              //定义圆心坐标
			double areacircle;
			for (auto t = 0; t < contours.size(); ++t)
			{
				// 面积过滤
				double area1 = contourArea(contours[t]);     //计算点集所围区域的面积
				if (area1 < 70000)            //晒选出轮廓面积大于100的轮廓
					continue;
				// 横纵比过滤
				Rect rect = boundingRect(contours[t]);            // 求点集的最小直立外包矩形
				float ratio = float(rect.width) / float(rect.height);        //求出宽高比

				if (ratio < 1.1 && ratio > 0.9)       //因为圆的外接直立矩形肯定近似于一个正方形，因此宽高比接近1.0
				{
					flag[t] = 2;
					areacircle = newarea[t];
				}
			}
			for (int i = 0; i < contours.size(); i++)
			{
				newarea[i] = newarea[i] / areacircle * 4.90625;
			}
			for (int i = 0; i < contours.size(); i++)
			{
				for (int j = 0; j < contours.size(); j++)
				{
					if (i != j)
					{
						double dis = sqrtf((point[i][0] - point[j][0]) * (point[i][0] - point[j][0]) + (point[i][1] - point[j][1]) * (point[i][1] - point[j][1]));
						if (dis < 40)
						{
							double area1 = contourArea(contours[i]);
							double area2 = contourArea(contours[j]);
							flag[i] = (area1 >= area2) ? 1 : -1;
							flag[j] = (area1 < area2) ? 1 : -1;//1表示的是真正的，-1表示的是内部的
						}
					}
				}
			}
			for (int i = 0; i < contours.size(); i++)
			{
				if ((flag[i] != 2) && (flag[i] != 1) && (flag[i] != -1) && (flag[i] != 5))
				{
					double reference1[50];
					double reference2[50];
					Scalar color(255);
					Mat im = Mat::zeros(imagemain.rows, imagemain.cols, CV_8U);
					drawContours(im, contours, i, color, CV_FILLED, 8, hierarchy);
					for (int j = 0; j < 11; j++)
					{
						reference1[j] = sqrt((area[j] - newarea[i]) * (area[j] - newarea[i]));
					}
					if ((area[findmin(reference1, 11)] - newarea[i]) < 0.2)
					{
						Moments moments1 = cv::moments(im, false);
						double huMoments1[7];
						HuMoments(moments1, huMoments1);
						for (int j = 0; j < 7; j++)
						{
							huMoments1[j] = -1 * copysign(1.0, huMoments1[j]) * log10(abs(huMoments1[j]));
						}
						for (int j = 0; j < 11; j++)
						{
							reference2[j] = sqrt((huMoments1[0] - HU[j][0]) * (huMoments1[0] - HU[j][0]) + (huMoments1[1] - HU[j][1]) * (huMoments1[1] - HU[j][1]));
						}
						if (reference2[findmin(reference2, 11)] < 0.01)
						{
							flag[i] = 3;
						}
					}
				}
			}
			Mat im = Mat::zeros(imagemain.rows, imagemain.cols, CV_8U);
			Scalar color(255);
			for (int i = 0; i < contours.size(); i++)
			{
				if (flag[i] != 3 && flag[i] != 2 && flag[i] != 1 && flag[i] != -1 && (flag[i] != 5))
					drawContours(im, contours, i, color, CV_FILLED, 8, hierarchy);
			}
			//imshow("", im);
			//waitKey(0);
			Mat three_channel = Mat::zeros(im.rows, im.cols, CV_8UC3);
			vector<Mat> channels1;
			for (int i = 0; i < 3; i++)
			{
				channels1.push_back(im);
			}
			merge(&channels1[0], channels1.size(), three_channel);
			imwrite("I:/ttttt.jpg", three_channel);
			Mat src = imread("I:/ttttt.jpg");
			Mat gray, binary, shiffted;
			// 保留边缘的平滑滤波

			// 二值化

			//imshow("binary", binary);

			// 距离变换
			Mat dist;
			distanceTransform(im, dist, DistanceTypes::DIST_L2, 3, CV_32F);
			// 归一化
			normalize(dist, dist, 0, 1, NORM_MINMAX);
			//imshow("dist", dist);
			// binary 得到山头,寻找种子
			threshold(dist, dist, 0.3, 1, THRESH_BINARY);
			//imshow("dist1", dist);

			// markers标记山头

			dist.convertTo(dist, CV_8UC1);

			vector<vector<Point>> contours1;
			findContours(dist, contours1, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
			// create markers
			Mat markers = Mat::zeros(src.size(), CV_32SC1);
			for (size_t i = 0; i < contours1.size(); i++)
			{
				drawContours(markers, contours1, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), -1); // 填充
				//imshow("", markers);
				//waitKey();
			}
			circle(markers, Point(5, 5), 3, Scalar(255), -1);
			// 完成分水岭变换
			watershed(src, markers);
			//imshow("watershed", markers);

			Mat mark = Mat::zeros(markers.size(), CV_8UC1);
			markers.convertTo(mark, CV_8UC1);
			bitwise_not(mark, mark, Mat());

			//imshow("完成分水岭变换", mark);

			// 生成随机颜色
			vector<Vec3b> colors;
			for (size_t i = 0; i < contours1.size(); i++)
			{
				int r = theRNG().uniform(0, 255);
				int g = theRNG().uniform(0, 255);
				int b = theRNG().uniform(0, 255);
				colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
			}

			// 颜色填充与最终显示
			Mat dst = Mat::zeros(markers.size(), CV_8UC3);
			int index = 0;
			for (int row = 0; row < markers.rows; row++)
			{
				for (int col = 0; col < markers.cols; col++)
				{
					index = markers.at<int>(row, col);

					if (index > 0 && index <= contours1.size())
						dst.at<Vec3b>(row, col) = colors[index - 1];
					else
						dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
				}
			}
			for (int i = 0; i < contours1.size(); i++)
			{
				Mat im = Mat::zeros(markers.rows, markers.cols, CV_32F);
				for (int row = 0; row < markers.rows; row++)
				{
					float* p = im.ptr<float>(row);
					for (int col = 0; col < markers.cols; col++)
					{
						if (markers.at<int>(row, col) == i + 1)
						{
							p[col] = 255;
						}
					}
				}
				if (i >= 1)
				{
					im.convertTo(im, CV_8U);
					theresult(im, imagemain1, areacircle, HU, area);
				}
			}
			for (int i = 0; i < contours.size(); i++)
			{
				if (flag[i] == 1 || flag[i] == 3 || flag[i] == 2)
				{
					Mat im = Mat::zeros(imagemain.rows, imagemain.cols, CV_8U);
					Scalar color(255);
					drawContours(im, contours, i, color, CV_FILLED, 8, hierarchy);//用来画轮廓呀呀呀
					theresult(im, imagemain1, areacircle, HU, area);
				}
			}
			//printf("number of objects: %d\n", contours.size());
			//imshow("Final Result", imagemain1);

			//waitKey(0);
			imwrite("H:/PENGPENG.JPG",imagemain1);
			m_image.Load("H:/PENGPENG.JPG");
			DrawImage();
		}
		++q;
	}

	

}


void Cdemo1Dlg::OnBnClickedfenimage()
{
	Mat src = imread("G:/QQ下载/1125649235/FileRecv/test11.bmp");
	Mat gray, binary, shiffted;
	// 保留边缘的平滑滤波
	pyrMeanShiftFiltering(src, shiffted, 21, 51);
	imshow("shiffted", shiffted);

	// 二值化
	cvtColor(shiffted, binary, COLOR_BGR2GRAY);
	threshold(binary, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	binary.convertTo(binary, CV_32FC1);
	change(binary);
	binary.convertTo(binary, CV_8U);
	imshow("binary", binary);

	// 距离变换
	Mat dist;
	distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);
	// 归一化
	normalize(dist, dist, 0, 1, NORM_MINMAX);
	imshow("dist", dist);
	// binary 得到山头,寻找种子
	threshold(dist, dist,0.3,1, THRESH_BINARY);
	imshow("dist", dist);

	// markers标记山头
	Mat dist_m;
	dist.convertTo(dist_m, CV_8U);
	// 寻找轮廓
	vector<vector<Point>> contours;
	findContours(dist_m, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// create markers
	Mat markers = Mat::zeros(src.size(), CV_32SC1);
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), -1); // 填充
		//imshow("", markers);
		waitKey();
	}
	//imshow("markers", markers * 10000);

	// 形态学腐蚀操作 - 彩色图像，目的是去掉干扰，让结果更好
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(src, src, MORPH_ERODE, k);
	circle(markers, Point(5, 5), 3, Scalar(255), -1);
	// 完成分水岭变换
	watershed(src, markers);
	//imshow("watershed", markers * 10000);

	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark, Mat());

	imshow("完成分水岭变换", mark);

	// 生成随机颜色
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int b = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// 颜色填充与最终显示
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);
	int index = 0;
	for (int row = 0; row < markers.rows; row++)
	{
		for (int col = 0; col < markers.cols; col++)
		{
			index = markers.at<int>(row, col);

			if (index > 0 && index <= contours.size())
				dst.at<Vec3b>(row, col) = colors[index - 1];
			else
				dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
		}
	}

	//printf("number of objects: %d\n", contours.size());
	imshow("Final Result", dst);

	waitKey(0);

}



	/*Mat element;
	int s = 10;
	element = getStructuringElement(MORPH_RECT, Size(s, s));
	Mat img1;
	morphologyEx(image, img1, MORPH_OPEN, element);
	Mat img2;
	morphologyEx(image, img2, MORPH_CLOSE, element);
	imshow("开运算", img1);
	imshow("闭运算", img2);
	Mat img3;
	morphologyEx(image, img3, MORPH_GRADIENT, element);
	imshow("形态学梯度", img3);
	Mat img4;
	erode(image,img4,element);
	imshow("腐蚀", img4);*/

