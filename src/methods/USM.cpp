#include "USM.h"

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

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "USM <img_path>" << std::endl;
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

    std::string save_path = dir_path + "/" + img_name + "_USM." + img_ext;
    cv::Mat     img_res;

    USM* usm = new USM();
    usm->processing(img_src, img_res);

    cv::imwrite(save_path, img_res);

    std::cout << "结果图已保存: " << save_path << std::endl;

    return 0;
}