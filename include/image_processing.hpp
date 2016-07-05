#pragma once

#include <memory>
#include <string>

using namespace cv;

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

class ImageProcessor {
 public:
   virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) = 0;
   virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, 
     const int kSize) = 0;
   virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi, 
     const int filterSize, const int lowThreshold, const int ratio, 
     const int kernelSize) = 0;
   virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi, 
     const int kDivs) = 0;
};

class ImageProcessorImpl : public ImageProcessor {
public:
	virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi);
	virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi,
		const int size);
	virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi,
		const int filter_size, const int low_threshold,
		const int ratio, const int kernel_size);
	virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi,
		const int divs);
};

Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi) 
{
	Mat src_copy = src.clone();
	Mat src_copy_roi = src_copy(roi);
	Mat dst_gray_roi = src_copy_roi.clone();
	cvtColor(src_copy_roi, dst_gray_roi, CV_RGB2GRAY);
	std::vector<Mat> channels(3);
	for (int i = 0; i < 3; i++)
		channels[i] = dst_gray_roi.clone();
	Mat dst_roi;
	merge(channels, dst_roi);
	src_copy_roi = dst_roi;
	return src_copy;
}

Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi,
	const int size)
{
	Mat src_copy = src.clone();
	Mat src_copy_roi = src_copy(roi);
	medianBlur(src_copy_roi, src_copy_roi, size);
	return src_copy;
}

Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filter_size, const int low_threshold,
	const int ratio, const int kernel_size) 
{
	Mat src_copy = src.clone();
	Mat src_copy_roi = src_copy(roi);
	Mat dst_gray_roi = src_copy_roi.clone();
	cvtColor(src_copy_roi, dst_gray_roi, CV_RGB2GRAY);
	Mat gray_blurred;
	blur(dst_gray_roi, gray_blurred, Size(filter_size, filter_size));
	Canny(gray_blurred, gray_blurred, low_threshold, low_threshold * ratio, kernel_size);
	Mat dst = src.clone();
	Mat dst_roi = dst(roi);
	//Scalar::all(dst_roi);
	//todo
}

Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
	const int divs) {
	//todo
}
