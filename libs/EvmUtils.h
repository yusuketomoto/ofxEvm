#pragma once
#include <opencv2/opencv.hpp>

template <typename T>
void rgb2ntsc(const T& src, T& dst)
{
	using namespace cv;

	T ntsc = src.clone();
	Mat V = (Mat_<float>(3, 3) << 1, 1, 1, 0.956, -0.272, -1.106, 0.621, -0.647,
			 1.703);
	V = V.inv();

	for (int j = 0; j < src.rows; j++)
	{
		for (int i = 0; i < src.cols; i++)
		{
			ntsc.template at<Vec3f>(j, i)(0) =
				src.template at<Vec3f>(j, i)(0) * V.at<float>(0, 0) +
				src.template at<Vec3f>(j, i)(1) * V.at<float>(0, 1) +
				src.template at<Vec3f>(j, i)(2) * V.at<float>(0, 2);

			ntsc.template at<Vec3f>(j, i)(1) =
				src.template at<Vec3f>(j, i)(0) * V.at<float>(1, 0) +
				src.template at<Vec3f>(j, i)(1) * V.at<float>(1, 1) +
				src.template at<Vec3f>(j, i)(2) * V.at<float>(1, 2);

			ntsc.template at<Vec3f>(j, i)(2) =
				src.template at<Vec3f>(j, i)(0) * V.at<float>(2, 0) +
				src.template at<Vec3f>(j, i)(1) * V.at<float>(2, 1) +
				src.template at<Vec3f>(j, i)(2) * V.at<float>(2, 2);
		}
	}
	ntsc.copyTo(dst);
}

template <typename T>
void ntsc2rgb(const T& src, T& dst)
{
	using namespace cv;

	T rgb = src.clone();
	Mat V = (Mat_<float>(3, 3) << 1.0, 0.956, 0.621, 1.0, -0.272, -0.647, 1.0,
			 -1.106, 1.703);
	V = V.t();

	for (int j = 0; j < src.rows; j++)
	{
		for (int i = 0; i < src.cols; i++)
		{
			rgb.template at<Vec3f>(j, i)(0) =
				src.template at<Vec3f>(j, i)(0) * V.at<float>(0, 0) +
				src.template at<Vec3f>(j, i)(1) * V.at<float>(0, 1) +
				src.template at<Vec3f>(j, i)(2) * V.at<float>(0, 2);

			rgb.template at<Vec3f>(j, i)(1) =
				src.template at<Vec3f>(j, i)(0) * V.at<float>(1, 0) +
				src.template at<Vec3f>(j, i)(1) * V.at<float>(1, 1) +
				src.template at<Vec3f>(j, i)(2) * V.at<float>(1, 2);

			rgb.template at<Vec3f>(j, i)(2) =
				src.template at<Vec3f>(j, i)(0) * V.at<float>(2, 0) +
				src.template at<Vec3f>(j, i)(1) * V.at<float>(2, 1) +
				src.template at<Vec3f>(j, i)(2) * V.at<float>(2, 2);
		}
	}
	rgb.copyTo(dst);
}
