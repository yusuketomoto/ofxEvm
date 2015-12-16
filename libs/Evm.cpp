#include "Evm.h"
#include "EvmUtils.h"
using namespace std;
using namespace cv;

EulerianVideoMagnification::EulerianVideoMagnification()
	: first_frame(true)
	, pyramid_levels(6)
	, alpha(10)
	, lambda_c(16)
	, r1(0.4)
	, r2(0.05)
	, chrom_attenuation(0.1)
{
}

template <typename T>
void allocate(T& image, int width, int height, int cv_type)
{
	int iw = image.cols;
	int ih = image.rows;
	int it = CV_MAKETYPE(image.depth(), image.channels());
	if (iw != width || ih != height || it != cv_type)
	{
		image.create(height, width, cv_type);
	}
}

void EulerianVideoMagnification::amplify(const Mat& src, vector<Mat>& pyramid,
										 Mat& dst, const FilterType type)
{
	allocate<Mat>(dst, src.cols, src.rows, CV_32FC3);

	Mat src32f;
	src.convertTo(src32f, CV_32FC3, 1 / 255.f);

	rgb2ntsc(src32f, src32f);
	buildLaplacianPyramid(src32f, pyramid);

	if (!first_frame)
	{
		for (int i = 0; i < pyramid.size(); i++)
		{
			Mat temp1 = (1 - r1) * lowpass_1[i] + r1 * pyramid[i];
			Mat temp2 = (1 - r2) * lowpass_2[i] + r2 * pyramid[i];
			lowpass_1[i] = temp1;
			lowpass_2[i] = temp2;
			filtered[i] = temp1 - temp2;
		}
		width = src.cols, height = src.rows;
		amplifyByAlpha();
	}
	else
	{
		lowpass_1 = pyramid;
		lowpass_2 = pyramid;
		filtered = pyramid;
		first_frame = false;
	}

	Mat reconstructed;
	reconstructImageFromLaplacianPyramid(filtered, reconstructed);
    Vec3f chrom(1, chrom_attenuation, chrom_attenuation);
    for (int y=0; y<reconstructed.rows; y++) {
        for (int x=0; x<reconstructed.cols; x++) {
            Vec3f& px = reconstructed.at<Vec3f>(y,x);
            px = px.mul(chrom);
        }
    }
    dst = src32f + reconstructed;

	ntsc2rgb(dst, dst);
}

void EulerianVideoMagnification::amplifyByAlpha()
{
	float delta = lambda_c / 8.0 / (1.0 + alpha);
	float exaggeration_factor = 2.0;
	float lambda = sqrt(width * width + height * height) / 3.0;

	for (int l = pyramid_levels; l >= 0; l--)
	{
		float curr_alpha = lambda / delta / 8 - 1;
		curr_alpha *= exaggeration_factor;

		float coef;
		if (l == pyramid_levels || l == 0)
			coef = 0;
		else if (curr_alpha > alpha)
			coef = alpha;
		else
			coef = curr_alpha;

		filtered[l] = filtered[l] * coef;

		lambda /= 2.0;
	}
}

void EulerianVideoMagnification::setAmplification(float value)
{
	alpha = value;
}
float EulerianVideoMagnification::getAmplification() const
{
	return alpha;
}
void EulerianVideoMagnification::setCutoff(float value)
{
	lambda_c = value;
}
float EulerianVideoMagnification::getCutoff() const
{
	return lambda_c;
}
void EulerianVideoMagnification::setFreqMax(float value)
{
	r1 = value;
}
float EulerianVideoMagnification::getFreqMax() const
{
	return r1;
}
void EulerianVideoMagnification::setFreqMin(float value)
{
	r2 = value;
}
float EulerianVideoMagnification::getFreqMin() const
{
	return r2;
}
void EulerianVideoMagnification::setChromeAttenuation(float value)
{
	chrom_attenuation = value;
}
float EulerianVideoMagnification::getChromeAttenuation() const
{
	return chrom_attenuation;
}
