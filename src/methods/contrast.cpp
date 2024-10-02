#include "contrast.h"


SEGCE::SEGCE() {}

SEGCE::~SEGCE() {}

void SEGCE::calc_global_histogram(const cv::Mat& img)
{
    int H = img.rows;   // Height
    int W = img.cols;   // Width

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

void SEGCE::calc_global_entropy()
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

void SEGCE::calc_spatial_histogram(const cv::Mat& img)
{
    int   H     = img.rows;   // Height
    int   W     = img.cols;   // Width
    float ratio = (float)H / W;
    region_num  = 256;

    int M, N;
    M = round(sqrt(region_num * ratio));   // height region num
    N = round(sqrt(region_num / ratio));   //  width region num

    region_num        = N * M;
    spatial_histogram = vector<vector<int>>(256, vector<int>(region_num));

    int gray_level;
    int region_idx = 0;
    int dH         = int(H / M);
    int dW         = int(W / N);

    for (int h = 0; h < M * dH; h++) {
        for (int w = 0; w < N * dW; w++) {
            region_idx = int(h / dH) * N + int(w / dW);
            gray_level = (int)img.at<uchar>(h, w);
            spatial_histogram[gray_level][region_idx]++;
        }
    }
}

void SEGCE::calc_spatial_histogram(const uint8_t* img)
{
    float ratio      = (float)this->height / this->width;
    this->region_num = 256;

    int M, N;
    M = round(sqrt(region_num * ratio));   // height region num
    N = round(sqrt(region_num / ratio));   //  width region num

    region_num        = N * M;
    spatial_histogram = vector<vector<int>>(256, vector<int>(region_num));

    int gray_level;
    int region_idx = 0;
    int dH         = int(this->height / M);
    int dW         = int(this->width / N);

    for (int h = 0; h < M * dH; h++) {
        const uint8_t* p_img = img + h * width;
        for (int w = 0; w < N * dW; w++) {
            region_idx = int(h / dH) * N + int(w / dW);
            gray_level = p_img[w];
            spatial_histogram[gray_level][region_idx]++;
        }
    }
}

void SEGCE::calc_spatial_entropy()
{
    float S_k, hist_value;

    entropy = vector<float>(256, 0.0);

    for (int k = 0; k < 256; k++) {
        S_k = 0.0;
        sum_region_hist =
            accumulate(spatial_histogram[k].begin(), spatial_histogram[k].end(), 0.0) + this->_EPS;

        for (int r = 0; r < region_num; r++) {
            hist_value = (float)spatial_histogram[k][r] / sum_region_hist;
            if (hist_value != 0) {
                S_k += -(hist_value * log2(hist_value));
            }
        }
        entropy[k] = S_k;
        sum_entropy += entropy[k];
    }
}

void SEGCE::calc_mapping()
{
    this->fk      = vector<float>(256, 0.0);
    this->fk_norm = vector<float>(256, 0.0);
    this->F_cdf   = vector<float>(256, 0.0);
    this->ymap    = vector<int>(256, 0);

    for (int k = 0; k < 256; k++) {
        fk[k] = entropy[k] / (sum_entropy - entropy[k] + this->_EPS);
        sum_fk += fk[k];
    }

    fk_norm[0]    = fk[0] / (sum_fk + this->_EPS);
    F_cdf[0]      = fk_norm[0];
    this->ymap[0] = round(F_cdf[0] * (yu - yd) + yd);
    for (int k = 1; k < 256; k++) {
        fk_norm[k]    = fk[k] / (sum_fk + this->_EPS);
        F_cdf[k]      = F_cdf[k - 1] + fk_norm[k];
        this->ymap[k] = round(F_cdf[k] * (yu - yd) + yd);
    }
}

void SEGCE::pixel_mapping(const cv::Mat& src, cv::Mat& dst, const vector<int>& map)
{
    cv::Mat table(map);
    table.convertTo(table, CV_8U);
    src.convertTo(src, CV_8U);
    cv::LUT(src, table, dst);
}

void SEGCE::pixel_mapping(const uint8_t* src, uint8_t* dst)
{
    for (int h = 0; h < height; h++) {
        const uint8_t* p_src = src + h * this->stride;
        uint8_t*       p_dst = dst + h * this->stride;
        for (int w = 0; w < width; w++) {
            p_dst[w * 3 + 0] = this->ymap[p_src[w * 3 + 0]];
            p_dst[w * 3 + 1] = this->ymap[p_src[w * 3 + 1]];
            p_dst[w * 3 + 2] = this->ymap[p_src[w * 3 + 2]];
        }
    }
}

void SEGCE::processing(const cv::Mat& src, cv::Mat& dst)
{
    this->height = src.rows;
    this->width  = src.cols;
    this->stride = this->width * 3;

    cv::Mat img_gary;
    dst = cv::Mat(this->height, this->width, CV_8UC3);
    cvtColor(src, img_gary, cv::COLOR_BGR2GRAY);

    // calc_spatial_histogram(img_gary);
    calc_spatial_histogram(img_gary.data);
    calc_spatial_entropy();
    calc_mapping();
    pixel_mapping(src.data, dst.data);

    // vector<cv::Mat> channels;
    // split(src, channels);
    // for (int c = 0; c < 3; c++) {
    //     this->pixel_mapping(channels[c], channels[c], this->ymap);
    // }

    // merge(channels, dst);
    return;
}