#pragma once

#include "Evm.h"
#include "ofxCv.h"

#define OFX_EVM_BEGIN_NAMESPACE \
	namespace ofx               \
	{
#define OFX_EVM_END_NAMESPACE }

OFX_EVM_BEGIN_NAMESPACE

class Evm
{

public:
	template <typename T>
	void update(T& frame)
	{
		cv::Mat frame_mat = ofxCv::toCv(frame);
		update(frame_mat);
	}

	void update(cv::Mat& frame)
	{
		using namespace ofxCv;

		cv::Mat result_mat;
		evm.amplify(frame, pyramid, result_mat);

		toOf(result_mat, result);
		result.update();
	}

	void draw()
	{
		if (!result.isAllocated()) return;
		result.draw(0, 0);
	}

	void amplification(float value)
	{
		evm.setAmplification(value);
	}
	float amplification() const
	{
		return evm.getAmplification();
	}
	void cutoff(float value)
	{
		evm.setCutoff(value);
	}
	float cutoff() const
	{
		return evm.getCutoff();
	}
	void freqMax(float value)
	{
		evm.setFreqMax(value);
	}
	float freqMax() const
	{
		return evm.getFreqMax();
	}
	void freqMin(float value)
	{
		evm.setFreqMin(value);
	}
	float freqMin() const
	{
		return evm.getFreqMin();
	}
	void chromeAttenuation(float value)
	{
		evm.setChromeAttenuation(value);
	}
	float chromeAttenuation() const
	{
		return evm.getChromeAttenuation();
	}

protected:
	EulerianVideoMagnification evm;
	ofFloatImage result;
	vector<cv::Mat> pyramid;
};

OFX_EVM_END_NAMESPACE

typedef ofx::Evm ofxEvm;
