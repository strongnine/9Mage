#include <opencv2/core/hal/interface.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "./methods/secedct.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if (argc != 3) {
        cout << "Usage: 9Mage <img_src path> <save path>" << endl;
        return -1;
    }

    char* imgName = argv[1];
    Mat   img_src;

    img_src = imread(imgName, 1);
    if (!img_src.data) {
        cout << "No img_src data" << endl;
        return -1;
    }
    Mat     img_res;
    SECEDCT secedct;
    secedct.processing(img_src, img_res);

    imwrite(argv[2], img_res);

    return 0;
}