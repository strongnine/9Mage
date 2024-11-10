#include "picpost.h"
#include <opencv2/opencv.hpp>

void pic_post(const cv::Mat& src, cv::Mat& dst)
{

    cv::Mat img_in, img_s, blur, img_out, blur_cut;
    int     height = src.rows;
    int     width  = src.cols;
    int     H, W;
    if (width == height) {   // 比例 1:1
        H = 1800;
        W = 1800;
    }
    else {   // 比例 3:4
        H = 2400;
        W = 1800;
    }
    // int H = 2400, W = 1800;                       // 比例 3:4
    int Hb = int(H * 1.5), Wb = int(W * 1.5);     // 放大 1.5
    int Hs = int(H * 0.75), Ws = int(W * 0.75);   // 缩小到 0.8

    int round_r = 20;   // 圆角的半径
    int Oc[2];          // 圆心的位置

    int filter_sz = 201;
    cv::GaussianBlur(src, blur, cv::Size(filter_sz, filter_sz), 0);

    cv::resize(src, img_in, cv::Size(W, H));
    cv::resize(img_in, img_s, cv::Size(Ws, Hs));
    cv::resize(blur, blur, cv::Size(Wb, Hb));

    blur_cut = blur(cv::Rect((Wb - W) / 2, (Hb - H) / 2, W, H));
    img_out  = blur_cut.clone();
    img_s.copyTo(img_out(cv::Rect((W - Ws) / 2, (H - Hs) / 2, Ws, Hs)));

    Oc[0]  = round_r;
    Oc[1]  = round_r;
    int h0 = (H - Hs) / 2, h1 = (H - Hs) / 2, h2 = H - (H - Hs) / 2 - 1, h3 = H - (H - Hs) / 2 - 1;
    int w0 = (W - Ws) / 2, w1 = W - (W - Ws) / 2 - 1, w2 = (W - Ws) / 2, w3 = W - (W - Ws) / 2 - 1;
    for (int i = 0; i < round_r; i++) {
        w0 = (W - Ws) / 2;
        w1 = W - (W - Ws) / 2 - 1;
        w2 = (W - Ws) / 2;
        w3 = W - (W - Ws) / 2 - 1;
        for (int j = 0; j < round_r; j++) {
            if (round(sqrt(pow((abs((float)i - Oc[0])), 2) + pow((abs((float)j - Oc[1])), 2))) >
                round_r) {
                img_out.at<cv::Vec3b>(h0, w0) = blur_cut.at<cv::Vec3b>(h0, w0);
                img_out.at<cv::Vec3b>(h1, w1) = blur_cut.at<cv::Vec3b>(h1, w1);
                img_out.at<cv::Vec3b>(h2, w2) = blur_cut.at<cv::Vec3b>(h2, w2);
                img_out.at<cv::Vec3b>(h3, w3) = blur_cut.at<cv::Vec3b>(h3, w3);
            }
            w0++;
            w1--;
            w2++;
            w3--;
        }
        h0++;
        h1++;
        h2--;
        h3--;
    }
    cv::GaussianBlur(img_out, dst, cv::Size(3, 3), 0);
    // dst = img_out;
}