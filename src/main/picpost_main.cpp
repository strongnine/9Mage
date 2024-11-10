#include "../methods/picpost.h"

#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

int main(int argc, char** argv)
{
    std::string read_path = argv[1];
    cv::Mat     img_src;

    std::cout << "读取图片: " << read_path << std::endl;
    std::cout << "处理中..." << std::endl;

    img_src = cv::imread(read_path, 1);
    if (!img_src.data) {
        std::cout << "No img_src data" << std::endl;
        return -1;
    }

    std::string save_path = argv[2];
    cv::Mat     img_res;

    pic_post(img_src, img_res);

    cv::imwrite(save_path, img_res);

    std::cout << "结果图已保存: " << save_path << std::endl;

    return 0;
}