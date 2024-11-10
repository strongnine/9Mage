#include "utility.h"
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

unordered_map<string, int> method_map{
    {"SEGCE", 0},
    {"USM", 1},
    {"AGC", 2},
    {"PicPost", 3},
};

void proc_with_method(const cv::Mat& src, cv::Mat& dst, string method)
{
    switch (method_map.at(method)) {
    case (METHOD_INDEX::SEGCE_METHOD):
    {
        SEGCE* segce = new SEGCE();
        segce->processing(src, dst);
        break;
    }
    case (METHOD_INDEX::USM_METHOD):
    {
        USM* usm = new USM();
        usm->processing(src, dst);
        break;
    }
    case (METHOD_INDEX::AGC_METHOD):
    {
        AutoGammaCorrection(src, dst);
    }
    case (METHOD_INDEX::PIC_POST_METHOD):
    {
        pic_post(src, dst);
        break;
    }
    default: break;
    }

    return;
}

int get_diff_time(chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}