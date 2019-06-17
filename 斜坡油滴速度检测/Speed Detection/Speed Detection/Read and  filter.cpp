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
	//������ͷ
	VideoCapture capture(0);
	//�ж��Ƿ�򿪳ɹ�
	if (!capture.isOpened()) {
		cout << "Faluse" << endl;
		return -1;
	}
	Mat frame;
	Mat salt_image;
	while (1)
	{
		capture.read(frame);
		//�ж��Ƿ��ȡ���ݳɹ�
		if (frame.empty()) {
			break;
		}
		else {
			//��ʾ����ͷ��ʵʱͼ��
			cv::imshow("video", frame);
			//OpenCV�Դ���˹�˲�
			cv::GaussianBlur(frame, salt_image, Size(9, 9), 0,0);
			//���˲����ͼ�������ʾ
			cv::imshow("GaussianBlur", salt_image);

		//��ɫ��ȡ
			//ɫ���
			int iLowH = 160;int iHighH = 179;
			//���Ͷ�
			int iLowS = 90;int iHighS = 255;
			//����
			int iLowV = 90;int iHighV = 255;
				
			Mat imgHSV;
			vector<Mat> hsvSplit;
			cvtColor(salt_image, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		//	//ֱ��ͼ���⻯��Ҫ��HSV�ռ���
		//	split(imgHSV, hsvSplit);
		//	equalizeHist(hsvSplit[2], hsvSplit[2]);
		//	merge(hsvSplit, imgHSV);
			Mat imgThresholded;

			inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

				////������ (ȥ��һЩ���)
				//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
				//morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

				////�ղ��� (����һЩ��ͨ��)
				//morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

			imshow("Thresholded Image", imgThresholded); //show the thresholded image

			//����������˳�����ͷ
			if (waitKey(20) > 0) {
				break;
			}
		}

	}
	//�ͷ�����
	capture.release();
	//�ر����д���
	destroyAllWindows();
	return 0;
}