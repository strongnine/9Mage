#include <chrono>
#include <opencv2/core.hpp>
#include <vector>

using namespace std;

enum METHOD_INDEX
{
    SEGCE_METHOD,
    USM_METHOD,
    AGC_METHOD,
    PIC_POST_METHOD,
};

void proc_with_method(const cv::Mat& src, cv::Mat& dst, string method);
int  get_diff_time(chrono::system_clock::time_point start, chrono::system_clock::time_point end);

void pic_post(const cv::Mat& src, cv::Mat& dst);