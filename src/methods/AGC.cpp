#include "AGC.h"

void AutoGammaCorrection(const cv::Mat& src, cv::Mat& dst)
{
    int     H = src.rows;
    int     W = src.cols;
    cv::Mat img_gray;
    cv::cvtColor(src, img_gray, cv::COLOR_BGR2GRAY);

    int   pix_sum  = 0;
    float pix_mean = 0.0;
    int   cnt      = 0;
    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
            pix_sum += img_gray.at<uchar>(h, w);
            cnt++;
        }
    }
    pix_mean    = (float)pix_sum / (255.0 * cnt);   // 计算均值
    float gamma = log10(0.35) / log10(pix_mean);    // 计算 gamma 值

    cv::Vec3b pix_src;
    cv::Vec3b pix_out;
    cv::Mat   out = cv::Mat(H, W, CV_8UC3);
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


int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "AGC <img_path>" << std::endl;
        return 1;
    }

    std::string src_path = argv[1];
    int         index    = src_path.find_last_of('/');

    std::string dir_path = src_path.substr(0, index);
    std::string img_name = src_path.substr(index + 1, -1);

    index               = img_name.find_last_of('.');
    std::string img_ext = img_name.substr(index + 1, -1);
    img_name            = img_name.substr(0, index);

    cv::Mat img_src;

    std::cout << "读取图片: " << img_name << '.' << img_ext << std::endl;
    std::cout << "处理中..." << std::endl;

    img_src = cv::imread(src_path, cv::IMREAD_COLOR);
    if (!img_src.data) {
        std::cout << "No img_src data" << std::endl;
        return -1;
    }

    // std::string save_path = argv[2];

    std::string save_path = dir_path + "/" + img_name + "_AGC." + img_ext;
    cv::Mat     img_res;

    AutoGammaCorrection(img_src, img_res);

    cv::imwrite(save_path, img_res);

    std::cout << "结果图已保存: " << save_path << std::endl;

    return 0;
}