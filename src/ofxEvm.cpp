//
//  ofxEvm.cpp
//  example_evm
//
//  Created by Yusuke on 2013/03/30.
//
//

#include "ofxEvm.h"

ofxEvm::ofxEvm()
:temporal_filter(EVM_TEMPORAL_IIR)
,levels(6)
,alpha_iir(10)
,lambda_c_iir(16)
,r1(0.4)
,r2(0.05)
,chromAttenuation_iir(0.1)
,alpha_ideal(150)
,lambda_c_ideal(6)
,fl(140.0/60.0)
,fh(160.0/60.0)
,samplingRate(30)
,chromAttenuation_ideal(1)
{
}

void ofxEvm::update(const cv::Mat& mat)
{
    cv::Mat mat32f;
    mat.convertTo(mat32f, CV_32F, 1.0/255.0);
    
    switch (temporal_filter) {
        case EVM_TEMPORAL_IIR:
            evm.amplify_spatial_lpyr_temporal_iir(mat32f, lapPyr, filtered,
                                                  alpha_iir, lambda_c_iir, r1, r2, chromAttenuation_iir);
            break;
        case EVM_TEMPORAL_IDEAL:
            evm.amplify_spatial_lpyr_temporal_ideal(mat32f, lapPyr, filtered,
                                                    alpha_ideal, lambda_c_ideal, fl, fh,
                                                    samplingRate, chromAttenuation_ideal);
            break;
    }
}

void ofxEvm::draw(float x, float y)
{
    ofImage res;
    cv::Mat result = filtered;
    result.convertTo(result, CV_8U, 255.0);
    res.setFromPixels(result.data, result.cols, result.rows, OF_IMAGE_COLOR);
    res.draw(x, y);
}

void ofxEvm::setTemporalFilter(EVM_TEMPORAL_FILTER filter)
{
    if (temporal_filter != filter) {
        evm.reset();
        temporal_filter = filter;
    }
}

void ofxEvm::setParamsIIR(float _alpha, float _lambda_c, float _r1, float _r2, float _chromAttenuation)
{
    alpha_iir = _alpha;
    lambda_c_iir = _lambda_c;
    r1 = _r1;
    r2 = _r2;
    chromAttenuation_iir = _chromAttenuation;
}

void ofxEvm::setParamsIdeal(float _alpha, float _lambda_c, float _fl, float _fh, float _samplingRate, float _chromAttenuation)
{
    alpha_ideal = _alpha;
    lambda_c_ideal = _lambda_c;
    fl = _fl;
    fh = _fh;
    samplingRate = _samplingRate;
    chromAttenuation_ideal = _chromAttenuation;
}

