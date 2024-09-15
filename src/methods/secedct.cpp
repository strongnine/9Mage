#include <numeric>
#include <vector>
#include <opencv2/opencv.hpp>

#include "secedct.h"

using namespace cv;
using namespace std;

SECEDCT::SECEDCT() {}

SECEDCT::~SECEDCT() {}

void SECEDCT::calc_global_histogram(const Mat& img)
{
    int H = img.rows;  // Height
    int W = img.cols;  // Width
    this->global_histogram = vector<int>(256);
    int gray_level;

    entropy = vector<float>(256, 0.0);

    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
            gray_level = (int)img.at<uchar>(h, w);
            global_histogram[gray_level]++;
        }
    }
}

void SECEDCT::calc_global_entropy()
{
    float hist_value;

    entropy = vector<float>(256, 0.0);
    for (int k = 0; k < 256; k++) {
        hist_value = (float)global_histogram[k] / 255;
        if (hist_value != 0) {
            entropy[k] = -hist_value * log2(hist_value);
        }
        sum_entropy += entropy[k];
    }
}

void SECEDCT::calc_spatial_histogram(const Mat& img)
{
    int   H = img.rows;  // Height
    int   W = img.cols;  // Width
    float ratio = (float)H / W;
    region_num = 256;

    int M, N;
    M = round(sqrt(region_num * ratio));  // height region num
    N = round(sqrt(region_num / ratio));  //  width region num

    region_num = N * M;
    spatial_histogram = vector<vector<int>>(256, vector<int>(region_num));

    int gray_level;
    int region_idx = 0;
    int dH = int(H / M);
    int dW = int(W / N);

    for (int h = 0; h < M * dH; h++) {
        for (int w = 0; w < N * dW; w++) {
            region_idx = int(h / dH) * N + int(w / dW);
            gray_level = (int)img.at<uchar>(h, w);
            spatial_histogram[gray_level][region_idx]++;
        }
    }
}

void SECEDCT::calc_spatial_entropy()
{
    float S_k, hist_value;

    entropy = vector<float>(256, 0.0);

    for (int k = 0; k < 256; k++) {
        S_k = 0.0;
        sum_region_hist = accumulate(spatial_histogram[k].begin(),
                                     spatial_histogram[k].end(), 0.0) +
                          this->_EPS;

        for (int r = 0; r < region_num; r++) {
            // cout << k << ", " << r << ", " << spatial_histogram[k].size()
            // << endl;
            hist_value = (float)spatial_histogram[k][r] / sum_region_hist;
            if (hist_value != 0) {
                S_k += -(hist_value * log2(hist_value));
            }
        }
        entropy[k] = S_k;
        sum_entropy += entropy[k];
    }
}

void SECEDCT::calc_mapping()
{
    fk = vector<float>(256, 0.0);
    fk_norm = vector<float>(256, 0.0);
    F_cdf = vector<float>(256, 0.0);
    ymap = vector<int>(256, 0);

    for (int k = 0; k < 256; k++) {
        fk[k] = entropy[k] / (sum_entropy - entropy[k] + this->_EPS);
        sum_fk += fk[k];
    }

    fk_norm[0] = fk[0] / (sum_fk + this->_EPS);
    F_cdf[0] = fk_norm[0];
    ymap[0] = round(F_cdf[0] * (yu - yd) + yd);
    for (int k = 1; k < 256; k++) {
        fk_norm[k] = fk[k] / (sum_fk + this->_EPS);
        F_cdf[k] = F_cdf[k - 1] + fk_norm[k];
        ymap[k] = round(F_cdf[k] * (yu - yd) + yd);
    }
}

void SECEDCT::pixel_mapping(const Mat& src, Mat& dst,
                            const vector<int>& map)
{
    Mat table(map);
    table.convertTo(table, CV_8U);
    src.convertTo(src, CV_8U);
    LUT(src, table, dst);
}

void SECEDCT::processing(const cv::Mat& src, cv::Mat& dst)
{
    Mat img_hsv;
    cvtColor(src, img_hsv, COLOR_BGR2HSV);
    vector<Mat> channels;
    split(img_hsv, channels);

    // calc_global_histogram(channels[2]);  // 全局处理
    // calc_global_entropy();               // 全局处理
    calc_spatial_histogram(channels[2]);  // 局部处理
    calc_spatial_entropy();               // 局部处理
    calc_mapping();
    pixel_mapping(channels[2], channels[2], ymap);

    merge(channels, dst);
    cvtColor(dst, dst, COLOR_HSV2BGR);
}