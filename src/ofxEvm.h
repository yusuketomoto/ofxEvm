//
//  ofxEvm.h
//  example_evm
//
//  Created by Yusuke on 2013/03/30.
//
//

#ifndef __example_evm__ofxEvm__
#define __example_evm__ofxEvm__

#include "ofMain.h"
#include "ofxCv.h"
#include "Evm.h"

enum EVM_TEMPORAL_FILTER {
    EVM_TEMPORAL_IIR = 0,
    EVM_TEMPORAL_IDEAL
};

class ofxEvm
{
public:
    ofxEvm();
    void update(const cv::Mat& mat);
    void draw(float x, float y);
    
    void setTemporalFilter(EVM_TEMPORAL_FILTER filter);
    void setParamsIIR(float _alpha, float _lambda_c, float _r1, float _r2, float _chromAttenuation);
    void setParamsIdeal(float _alpha, float _lambda_c, float _fl, float _fh, float _samplingRate, float _chromAttenuation);
    
    ofImage getMagnifiedImage();
    std::vector<cv::Mat_<cv::Vec3f> > getPyramid() const { return lapPyr; }
    
private:
    Evm evm;
    
    EVM_TEMPORAL_FILTER temporal_filter;
    int levels;
    float alpha_iir;
    float lambda_c_iir;
    float r1;
    float r2;
    float chromAttenuation_iir;
    float alpha_ideal;
    float lambda_c_ideal;
    float fl;
    float fh;
    float samplingRate;
    float chromAttenuation_ideal;
    
    std::vector<cv::Mat_<cv::Vec3f> > lapPyr;
    cv::Mat filtered;
    
    
};

#endif /* defined(__example_evm__ofxEvm__) */
