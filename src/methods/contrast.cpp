#include "contrast.h"
#include <_types/_uint8_t.h>
#include <arm_neon.h>
#include <sys/_types/_int32_t.h>

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

void SEGCE::pixel_mapping(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat table(this->ymap);
    table.convertTo(table, CV_8U);

    vector<cv::Mat> channels;
    split(src, channels);
    for (int c = 0; c < 3; c++) {
        channels[c].convertTo(channels[c], CV_8U);
        cv::LUT(channels[c], table, channels[c]);
    }

    merge(channels, dst);
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

// TODO: 该函数未进行 -O3 编译优化之前效率很低，需要对 NEON 指令的写法再优化
void SEGCE::pixel_mapping_neon(const uint8_t* __restrict src, uint8_t* __restrict dst)
{
    int block_sz  = 16;
    int block_num = width * 3 / block_sz;

    // 原始 this->ymap 中的索引
    // [ 0  1  2  3 |  4  5  6  7 | ... ]
    // [16 17 18 19 | 20 21 22 23 | ... ]
    // [32 33 34 35 | 36 37 38 39 | ... ]
    // [48 49 50 51 | 52 53 54 55 | ... ]
    // 
    // 读取到寄存器中 table 对应在 this->ymap 中的索引
    // [ 0  4  8 12 | 16 20 24 28 | ... ]
    // [ 1  5  9 13 | 17 21 25 29 | ... ]
    // [ 2  6 10 14 | 18 22 26 30 | ... ]
    // [ 3  7 11 15 | 19 23 27 31 | ... ]

    uint8x16_t   index, vec_src, vec_dst, tmp1_u8x16, tmp2_u8x16, tmp3_u8x16, tmp4_u8x16;
    uint8x16_t   diff = vdupq_n_u8(64);
    uint8x16x4_t table, table1, table2, table3, table4;
    for (int i = 0; i < 4; i++) {
        table         = vld4q_u8((uint8_t*)(this->ymap.data() + i * 16));
        table1.val[i] = table.val[0];

        table         = vld4q_u8((uint8_t*)(this->ymap.data() + 64 + i * 16));
        table2.val[i] = table.val[0];

        table         = vld4q_u8((uint8_t*)(this->ymap.data() + 128 + i * 16));
        table3.val[i] = table.val[0];

        table         = vld4q_u8((uint8_t*)(this->ymap.data() + 192 + i * 16));
        table4.val[i] = table.val[0];
    }

    // block_sz 为 16，每次可以并行处理 16 个像素
    for (int h = 0; h < height; h++) {
        const uint8_t* p_src = src + h * stride;
        uint8_t*       p_dst = dst + h * stride;
        for (int i = 0; i < block_num * block_sz; i += block_sz) {
            vec_src    = vld1q_u8(p_src);
            tmp1_u8x16 = vqtbl4q_u8(table1, vec_src);

            vec_src    = vsubq_u8(vec_src, diff);
            tmp2_u8x16 = vqtbl4q_u8(table2, vec_src);

            vec_src    = vsubq_u8(vec_src, diff);
            tmp3_u8x16 = vqtbl4q_u8(table3, vec_src);

            vec_src    = vsubq_u8(vec_src, diff);
            tmp4_u8x16 = vqtbl4q_u8(table4, vec_src);

            vec_dst = veorq_u8(veorq_u8(veorq_u8(tmp1_u8x16, tmp2_u8x16), tmp3_u8x16), tmp4_u8x16);

            vst1q_u8(p_dst, vec_dst);

            p_src += block_sz;
            p_dst += block_sz;
        }
        // 处理余下的像素点
        for (int i = block_num * block_sz; i < width * 3; i++) {
            *p_dst = this->ymap[*p_src];

            p_src++;
            p_dst++;
        }
    }
}

void SEGCE::processing(const cv::Mat& src, cv::Mat& dst, string acc)
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
    pixel_mapping(src, dst);
    // pixel_mapping_neon(src.data, dst.data);
    // pixel_mapping(src.data, dst.data);
    return;
}