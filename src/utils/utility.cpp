#include "utility.h"
#include "../methods/contrast.h"
#include "../methods/lighten.h"
#include "../methods/sharpen.h"

void proc_with_method(const cv::Mat& src, cv::Mat& dst, string method)
{
    if (method == "SEGCE") {
        SEGCE* segce = new SEGCE();
        segce->processing(src, dst);
    }
    else if (method == "USM") {
        USM* usm = new USM();
        usm->processing(src, dst);
    }
    else if (method == "AGC") {
        AutoGammaCorrection(src, dst);
    }

    return;
}

int get_diff_time(chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}