#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<OpenNI.h>
using namespace cv;
using namespace std;

int main(int argc, char*argv[]){
	IplImage *orgImg = cvLoadImage("tu.jpg",1);
	if (!orgImg)exit(0);
	IplImage *grayImg = cvCreateImage(cvGetSize(orgImg),IPL_DEPTH_8U,1);
	IplImage *tempRGBImg = cvCreateImage(cvGetSize(orgImg), IPL_DEPTH_8U, 3);

	IplImage *redImage = cvCreateImage(cvGetSize(orgImg), IPL_DEPTH_8U, 1);
	IplImage *greenImage = cvCreateImage(cvGetSize(orgImg), IPL_DEPTH_8U, 1);
	IplImage *blueImage = cvCreateImage(cvGetSize(orgImg), IPL_DEPTH_8U, 1);

	 
	CvScalar pix;
	cvZero(tempRGBImg); 
	//cvZero(redImage); cvZero(greenImage); cvZero(blueImage);
	//for (int j = 0; j < orgImg->height; j++){
	//	for (int i = 0; i < orgImg->width; i++){
	//		pix = cvGet2D(orgImg,j,i);
	//		cvSet2D(redImage,j,i,cvScalar(pix.val[2],0,0));
	//		cvSet2D(greenImage, j, i, cvScalar(pix.val[1], 0, 0));
	//		cvSet2D(blueImage, j, i, cvScalar(pix.val[0], 0, 0));
	//	}
	//}
	cvSplit(orgImg, blueImage, greenImage, redImage, NULL);//

	//cvShowImage("redImage", redImage);
	//cvShowImage("blueImage", blueImage);
	//cvShowImage("greenImage", greenImage);
	//cvWaitKey(0);

	//红色通道的平均灰度
	double sumRed = 0.0, numPixel = 0,avgPixRed=0.0, redTh=0.0;
	double sumGreen = 0.0, avgPixGreen = 0.0, greenTh = 0.0;
	double sumBlue = 0.0, avgPixBlue = 0.0, blueTh = 0.0;

	for (int j = 0; j < redImage->height; j++){
		for (int i = 0; i < redImage->width; i++){
			pix = cvGet2D(redImage, j, i);
			sumRed += pix.val[0];
			pix = cvGet2D(blueImage, j, i);
			sumBlue += pix.val[0];
			pix = cvGet2D(greenImage, j, i);
			sumGreen += pix.val[0];
		}
	}
	numPixel = redImage->height*redImage->width;
	avgPixRed = sumRed / numPixel;
	redTh = avgPixRed*0.6;
	avgPixGreen = sumGreen / numPixel;
	greenTh = avgPixGreen*1.1;
	avgPixBlue = sumBlue / numPixel;
	blueTh = avgPixBlue*1.1;

	//获取红色通道的单通道图像
	//cvCvtColor(orgImg,grayImg,CV_RGB2GRAY);
	cvZero(grayImg);
	for (int j = 0; j < orgImg->height; j++){
		for (int i = 0; i < orgImg->width; i++){
			pix = cvGet2D(orgImg,j,i);
			if (pix.val[0] < avgPixBlue && pix.val[1] < avgPixGreen && pix.val[2] > redTh){
				cvSet2D(tempRGBImg, j, i, pix);
			    i = i;
			} 
		}
	}

	cvCvtColor(tempRGBImg, grayImg, CV_RGB2GRAY);

	//cvShowImage("orgImage",orgImg);
	//cvShowImage("tempRGBImg", tempRGBImg);
	//cvShowImage("grayImg", grayImg);
	//cvWaitKey(0);
	//自适应的得到红色单通道图像的二值图像
	IplImage* imgTh = cvCreateImage(cvGetSize(orgImg), IPL_DEPTH_8U, 1);
	cvZero(imgTh);
	double redsigSum = 0.0, redsigAvgPix = 0.0;
	for (int j = 0; j < grayImg->height; j++){
		for (int i = 0; i < grayImg->width; i++){
			pix = cvGet2D(grayImg,j,i);
			redsigSum += pix.val[0];
		}
	}
	redsigAvgPix = redsigSum / (grayImg->height*grayImg->width);//（在此可以适当乘以一个因子，阈值变大）
	for (int j = 0; j < grayImg->height; j++){
		for (int i = 0; i < grayImg->width; i++){
			pix = cvGet2D(grayImg, j, i);
			if (pix.val[0] > redsigAvgPix)
				cvSet2D(imgTh,j,i,cvScalar(255,0,0));
		}
	}
	cvShowImage("imgTh", imgTh);
	//cvWaitKey(0);


	cvErode(imgTh, imgTh, 0, 1);//腐蚀
	cvDilate(imgTh, imgTh, 0, 1);//膨胀
	cvShowImage("pengzhang", imgTh);
	//cvWaitKey(0);

	//找图像中的最大块区域
	int maxNum = 0,i=0;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;
	CvSeq* _contour = 0;
	double maxarea = 0.0;
	double minarea = 0.0;
	IplImage *Fill_img = cvCreateImage(cvGetSize(orgImg), IPL_DEPTH_8U, 1);
	int contour_num = cvFindContours(imgTh, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cvZero(Fill_img);//清空数组
	_contour = contour;
	CvScalar color = CV_RGB(255, 255, 255);

	for (; contour != 0; contour = contour->h_next)
	{
		double tmparea = fabs(cvContourArea(contour));
		//if (tmparea < minarea)
		//{
		//	cvSeqRemove(contour, 0); //删除面积小于设定值的轮廓    
		//	continue;
		//}
		//if (tmparea > maxarea)
		//{
		//	cvSeqRemove(contour, 0); //删除面积大于设定值的轮廓    
		//	continue;
		//}
		if (tmparea > maxarea)
		{
			maxNum = i;
			maxarea = tmparea;
			cvZero(Fill_img);
			cvDrawContours(Fill_img, contour, color, color, -1, CV_FILLED, 8);//填充内部轮廓
		}
		i++;
	}


	cvDrawContours(Fill_img, contour, color, color, -1, CV_FILLED, 8);//填充内部轮廓 
	cvShowImage("Fill_img", Fill_img);
	//cvWaitKey(0);

	// 计算重心
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	cvMoments(Fill_img, moments, 1); // The actual moment values 
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
	double area = cvGetSpatialMoment(moments, 0, 0);
	double posX = moment10 / area;
	double posY = moment01 / area;
	//显示并绘制重心
	CvPoint center; center.x = int(posX); center.y = int(posY);
	cout << "重心坐标"<< endl;
	cout << "x= " << center.x << "; y= " << center.y << endl;
	cvCircle(Fill_img, center, 3, CV_RGB(120, 150, 100), 1, 8, 0);

	cvShowImage("重心", Fill_img);
	cvWaitKey(0);

	cvReleaseImage(&orgImg);
	cvReleaseImage(&grayImg);
	cvReleaseImage(&tempRGBImg);
	cvReleaseImage(&redImage);
	cvReleaseImage(&greenImage);
	cvReleaseImage(&blueImage);
	cvReleaseImage(&imgTh);
	cvReleaseImage(&Fill_img);

	return 0;
}