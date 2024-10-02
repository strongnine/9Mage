#include <opencv2/core.hpp>
#include <chrono>
#include <vector>

using namespace std;

void proc_with_method(const cv::Mat& src, cv::Mat& dst, string method);
int get_diff_time(chrono::system_clock::time_point start, chrono::system_clock::time_point end);