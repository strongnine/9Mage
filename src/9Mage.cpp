#include <chrono>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

#include "./utils/cmdline.h"   // https://github.com/tanakh/cmdline
#include "./utils/utility.h"

using namespace std;

int main(int argc, char** argv)
{
    string program_name = (string)argv[0] + " <path_to_read> <path_to_save>";

    cmdline::parser opt;
    opt.set_program_name(program_name);
    // opt.add<string>("src", 's', "The path of the src image. ", true);
    // opt.add<string>("dst", 'd', "The path of the result image. ", true);
    opt.add<string>("method",
                    'm',
                    "The method to use. ",
                    true,
                    "",
                    cmdline::oneof<string>("SEGCE", "USM", "AGC"));
    opt.add("show", 's', "Show result image. ");
    opt.add("nosave", '\0', "Donot save the result image. ");

    opt.add("help", '?', "Print usage. ");
    opt.parse_check(argc, argv);

    string method_name = opt.get<string>("method");

    // string read_path = opt.get<string>("src");
    string  read_path = argv[1];
    cv::Mat img_src;

    cout << "读取图片中..." << endl;

    img_src = cv::imread(read_path, 1);
    if (!img_src.data) {
        cout << "No img_src data" << endl;
        return -1;
    }

    // string  save_path = opt.get<string>("dst");
    string  save_path = argv[2];
    cv::Mat img_res;

    cout << "开始使用方法 " << method_name << " 处理图片..." << endl;

    chrono::system_clock::time_point tick = chrono::system_clock::now();   // 记录开始时间
    proc_with_method(img_src, img_res, method_name);                       // 开始处理图片
    chrono::system_clock::time_point tock = chrono::system_clock::now();   // 记录结束时间

    cout << "总共时间花费：" << get_diff_time(tick, tock) * 0.001 << " ms" << endl;


    if (opt.exist("show")) {
        cv::imshow("result image", img_res);
        cv::waitKey();
    }
    cv::imwrite(save_path, img_res);

    return 0;
}