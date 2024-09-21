#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cmdline.h"
#include "./methods/secedct.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    cmdline::parser opt;
    // opt.add<string>("src", 's', "The path of the src image. ", true);
    // opt.add<string>("dst", 'd', "The path of the result image. ", true);
    // opt.add<string>("method", 'm', "The method to use. ", true);
    opt.add("show", 's', "If show the image. ");

    opt.add<string>("help", 'h', "Print usage. ", false);
    opt.parse_check(argc, argv);

    // if (argc != 3) {s
    //     cout << "Usage: 9Mage <img_src path> <save path>" << endl;
    //     return -1;
    // }

    // string read_path = opt.get<string>("src");
    string read_path = argv[1];
    Mat   img_src;

    img_src = imread(read_path, 1);
    if (!img_src.data) {
        cout << "No img_src data" << endl;
        return -1;
    }

    // string  save_path = opt.get<string>("dst");
    string  save_path = argv[2];
    Mat     img_res;
 
    SECEDCT secedct;
    secedct.processing(img_src, img_res);

    if (opt.exist("show")) {
        imshow("result image", img_res);
        waitKey();
    }
    imwrite(save_path, img_res);

    return 0;
}