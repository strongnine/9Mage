#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "./methods/contrast.h"
#include "cmdline.h"

using namespace std;

int main(int argc, char** argv)
{
    cmdline::parser opt;
    // opt.add<string>("src", 's', "The path of the src image. ", true);
    // opt.add<string>("dst", 'd', "The path of the result image. ", true);
    opt.add<string>("method", 'm', "The method to use. ", true, "",
                    cmdline::oneof<string>("SEGCE", "USM"));
    opt.add("show", 's', "Show result image. ");
    opt.add("nosave", '\0', "Donot save the result image. ");

    opt.add<string>("help", 'h', "Print usage. ", false);
    opt.parse_check(argc, argv);

    // if (argc != 3) {s
    //     cout << "Usage: 9Mage <img_src path> <save path>" << endl;
    //     return -1;
    // }

    // string read_path = opt.get<string>("src");
    string  read_path = argv[1];
    cv::Mat img_src;

    img_src = cv::imread(read_path, 1);
    if (!img_src.data) {
        cout << "No img_src data" << endl;
        return -1;
    }

    // string  save_path = opt.get<string>("dst");
    string  save_path = argv[2];
    cv::Mat img_res;

    SEGCE secedct;
    secedct.processing(img_src, img_res);

    if (opt.exist("show")) {
        cv::imshow("result image", img_res);
        cv::waitKey();
    }
    cv::imwrite(save_path, img_res);

    return 0;
}