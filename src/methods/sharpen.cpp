#include "sharpen.h"

USM::USM() {}
USM::~USM() {}

void USM::processing(const cv::Mat& src, cv::Mat& dst)
{
    int H = src.rows;
    int W = src.cols;

    // 公式：y(n, m) = x(n, m) + lambda * z(n, m)
    // 其中 z(n, m) 可由 x(n, m) 通过高通滤波获取

    // 参考 Photoshop 和 Imageshop 的算法，具有三个参数 半径、数量、阈值
    // 关键就在于条件这三个参数
    int     radius    = 25;
    int     amount    = 100;
    int     threshold = 0;
    cv::Mat blur;
    cv::GaussianBlur(src, blur, cv::Size(radius, radius), 0);

    cv::Mat out = cv::Mat(H, W, CV_8UC3);
    int     src_pix, blur_pix, diff, dst_pix;

    for (int c = 0; c < 3; c++) {
        for (int h = 0; h < H; h++) {
            for (int w = 0; w < W; w++) {
                src_pix  = src.at<cv::Vec3b>(h, w)[c];
                blur_pix = blur.at<cv::Vec3b>(h, w)[c];
                diff     = src_pix - blur_pix;
                if (abs(diff) > threshold) {
                    dst_pix = src_pix + amount * diff / 100;
                    dst_pix = (uchar)((((ushort)dst_pix | ((short)(255 - dst_pix) >> 15)) &
                                       ~dst_pix >> 15));
                }
                else {
                    dst_pix = src_pix;
                }
                out.at<cv::Vec3b>(h, w)[c] = dst_pix;
            }
        }
    }
    dst = out;
}