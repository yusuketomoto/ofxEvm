//
//  Evm.cpp
//  example_evm
//
//  Created by Yusuke on 2013/03/30.
//
//

#include "Evm.h"
#include "Utils.h"
using namespace cv;
using namespace std;

Evm::Evm()
:levels(6)
,is_not_first_frame(false)
{
}

Evm::~Evm()
{
}

void Evm::reset()
{
    is_not_first_frame = false;
}

void Evm::amplify_spatial_lpyr_temporal_iir(const Mat& src, vector<Mat_<Vec3f> >& lapPyr, Mat& dst,
                                            float alpha, float lambda_c, float r1, float r2, float chromAttenuation)
{
    Mat_<Vec3f> s = src.clone();
    rgb2ntsc(src, s);
    
    buildLaplacianPyramid(s, lapPyr);
    
    if (is_not_first_frame) {
        //temporal iir
        for (int i=0; i<lapPyr.size(); i++) {
            Mat temp1 = (1-r1)*lowpass1[i] + r1*lapPyr[i];
            Mat temp2 = (1-r2)*lowpass2[i] + r2*lapPyr[i];
            lowpass1[i] = temp1;
            lowpass2[i] = temp2;
            filtered[i] = temp1 - temp2;
        }
        //amplify
        amplifyByAlpha(src, alpha, lambda_c);
    } else { // first frame
        lowpass1 = lapPyr;
        lowpass2 = lapPyr;
        filtered = lapPyr;
        is_not_first_frame = true;
    }
    
    dst = s + reconstructImgFromLapPyramid(filtered).mul(Vec3f(1,chromAttenuation,chromAttenuation));

    s = dst.clone();
    ntsc2rgb(s, s);
    dst = s.clone();
}

void Evm::amplify_spatial_lpyr_temporal_ideal(const cv::Mat &src, std::vector<cv::Mat_<cv::Vec3f> > &lapPyr, cv::Mat &dst, float alpha, float lambda_c, float fl, float fh, float samplingRate, float chromAttenuation)
{
    Mat_<Vec3f> s = src.clone();
    rgb2ntsc(src, s);
    
    buildLaplacianPyramid(s, lapPyr);
    
    if (is_not_first_frame) {
        //temporal ideal
        for (int i=0; i<lapPyr.size(); i++) {
            //TODO:implement temporal ideal filter
            //
        }
        //amplify
//        amplifyByAlpha(src, alpha, lambda_c);
    } else {
        lowpass1 = lapPyr;
        lowpass2 = lapPyr;
        filtered = lapPyr;
        is_not_first_frame = true;
    }
    
    dst = s + reconstructImgFromLapPyramid(filtered).mul(Vec3f(1,chromAttenuation,chromAttenuation));
    
    s = dst.clone();
    ntsc2rgb(s, s);
    dst = s.clone();
}

void Evm::amplifyByAlpha(const Mat& src, float alpha, float lambda_c)
{
    int w = src.cols;
    int h = src.rows;
    
    //amplify each spatial frequency bands according to Figure 6 of paper
    float delta = lambda_c/8.0/(1.0+alpha);
    //the factor to boost alpha above the bound shown in the paper. (for better visualization)
    float exaggeration_factor = 2.0;
    //compute the representative wavelength lambda for the lowest spatial frequency band of Laplacian pyramid
    float lambda = sqrt(w*w + h*h)/3; // 3 is experimental constant
    
    for (int l=levels; l>=0; l--) {
        //compute modified alpha for this level
        float currAlpha = lambda/delta/8 - 1;
        currAlpha *= exaggeration_factor;
        if (l==levels || l==0) { //ignore the highest and lowest frequency band
            filtered[l] = filtered[l] * 0;
        }
        else if (currAlpha > alpha) {
            filtered[l] = filtered[l] * alpha;
        }
        else {
            filtered[l] = filtered[l] * currAlpha;
        }
        //go one level down on pyramid, representative lambda will reduce by factor of 2
        lambda /= 2.0;
    }

}


void Evm::buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr) {
    lapPyr.clear();
    Mat currentImg = img;    
    for (int l=0; l<levels; l++) {
        Mat down,up;
        pyrDown(currentImg, down);
        pyrUp(down, up, currentImg.size());
        Mat lap = currentImg - up;
        lapPyr.push_back(lap);
        currentImg = down;
    }
    lapPyr.push_back(currentImg);
}

Mat_<Vec3f> Evm::reconstructImgFromLapPyramid(vector<Mat_<Vec3f> >& lapPyr) {
    Mat currentImg = lapPyr[levels];
    for (int l=levels-1; l>=0; l--) {
        Mat up;
        pyrUp(currentImg, up, lapPyr[l].size());
        currentImg = up + lapPyr[l];
    }
    return currentImg;
}


