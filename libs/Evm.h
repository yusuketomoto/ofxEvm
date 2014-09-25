#pragma once

#include <opencv2/opencv.hpp>

class EulerianVideoMagnification
{
public:
	enum FilterType
	{
		IIR
	};

	EulerianVideoMagnification();
	void amplify(const cv::Mat& img, std::vector<cv::Mat>& pyramid,
				 cv::Mat& dst, const FilterType type = IIR);

	void reset();
	void setAmplification(float value);
	float getAmplification() const;
	void setCutoff(float value);
	float getCutoff() const;
	void setFreqMax(float value);
	float getFreqMax() const;
	void setFreqMin(float value);
	float getFreqMin() const;
	void setChromeAttenuation(float value);
	float getChromeAttenuation() const;

protected:
	void amplifyByAlpha();

	bool first_frame;
	int pyramid_levels;

	std::vector<cv::Mat> filtered;
	std::vector<cv::Mat> lowpass_1;
	std::vector<cv::Mat> lowpass_2;
	float alpha;
	float lambda_c;
	float r1;
	float r2;
	float chrom_attenuation;
	int width, height;
};


template <typename T>
void buildLaplacianPyramid(const T& image, std::vector<T>& pyramid,
						   int levels = 6)
{
	using namespace cv;

	if (pyramid.size() != levels + 1) pyramid.resize(levels + 1);

	Mat current = image;
	for (int i = 0; i < levels; i++)
	{
		Mat down, up;
		pyrDown(current, down);
		pyrUp(down, up, current.size());
		Mat lap = current - up;
		pyramid[i] = lap;
		current = down;
	}
	pyramid[levels] = current;
}

template <typename T>
void reconstructImageFromLaplacianPyramid(const std::vector<T>& pyramid,
										  T& image)
{
	using namespace cv;

	assert(pyramid.size() > 0);

	int levels = pyramid.size() - 1;
	Mat current = pyramid[levels];
	for (int i = levels - 1; i >= 0; i--)
	{
		Mat up;
		pyrUp(current, up, pyramid[i].size());
		current = up + pyramid[i];
	}
	current.copyTo(image);
}
