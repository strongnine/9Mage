#include "lighten.h"

void AutoGammaCorrection(const cv::Mat &src, cv::Mat &dst)
{
    int H = src.rows;
    int W = src.cols;
    cv::Mat img_gray;
    cv::cvtColor(src, img_gray, cv::COLOR_BGR2GRAY);

    int pix_sum = 0;
    float pix_mean = 0.0;
    int cnt = 0;
    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
            pix_sum += img_gray.at<uchar>(h, w);
            cnt++;
        }
    }
    pix_mean = (float)pix_sum / (255.0 * cnt);   // 计算均值
    float gamma = log10(0.35) / log10(pix_mean); // 计算 gamma 值

    cv::Vec3b pix_src;
    cv::Vec3b pix_out;
    cv::Mat out = cv::Mat(H, W, CV_8UC3);
    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
            pix_src = src.at<cv::Vec3b>(h, w);
            pix_out = out.at<cv::Vec3b>(h, w);
            pix_out[0] = (int)(pow((float)pix_src[0] / 255.0, gamma) * 255.0);
            pix_out[1] = (int)(pow((float)pix_src[1] / 255.0, gamma) * 255.0);
            pix_out[2] = (int)(pow((float)pix_src[2] / 255.0, gamma) * 255.0);
            out.at<cv::Vec3b>(h, w) = pix_out;
        }
    }
    dst = out;
}