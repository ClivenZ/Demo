#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//string intToString(int number) {
//	std::stringstream ss;
//	ss << number;
//	return ss.str();
//}

int main() {
	//打开摄像头
	VideoCapture capture(0);
	//判断是否打开成功
	if (!capture.isOpened()) {
		cout << "Faluse" << endl;
		return -1;
	}
	Mat frame;
	Mat salt_image;
	while (1)
	{
		capture.read(frame);
		//判断是否读取数据成功
		if (frame.empty()) {
			break;
		}
		else {
			//显示摄像头的实时图像
			cv::imshow("video", frame);
			//OpenCV自带高斯滤波
			cv::GaussianBlur(frame, salt_image, Size(9, 9), 0,0);
			//对滤波完的图像进行显示
			cv::imshow("GaussianBlur", salt_image);

		//颜色提取
			//色相角
			int iLowH = 160;int iHighH = 179;
			//饱和度
			int iLowS = 90;int iHighS = 255;
			//亮度
			int iLowV = 90;int iHighV = 255;
				
			Mat imgHSV;
			vector<Mat> hsvSplit;
			cvtColor(salt_image, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		//	//直方图均衡化需要在HSV空间做
		//	split(imgHSV, hsvSplit);
		//	equalizeHist(hsvSplit[2], hsvSplit[2]);
		//	merge(hsvSplit, imgHSV);
			Mat imgThresholded;

			inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

				////开操作 (去除一些噪点)
				//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
				//morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

				////闭操作 (连接一些连通域)
				//morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

			imshow("Thresholded Image", imgThresholded); //show the thresholded image

			//按下任意键退出摄像头
			if (waitKey(20) > 0) {
				break;
			}
		}

	}
	//释放数据
	capture.release();
	//关闭所有窗口
	destroyAllWindows();
	return 0;
}