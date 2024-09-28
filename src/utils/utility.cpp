#include "utility.h"
#include "../methods/contrast.h"
#include "../methods/sharpen.h"
#include "../methods/lighten.h"

void proc_with_method(const cv::Mat& src, cv::Mat& dst, string method)
{
    if (method == "SEGCE") {
        SEGCE* segce;
        segce->processing(src, dst);
    }
    else if (method == "USM") {
        USM* usm;
        usm->processing(src, dst);
    } else if (method == "AGC") {
        AutoGammaCorrection(src, dst);
    }

    return;
}