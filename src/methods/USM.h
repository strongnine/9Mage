#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

class USM
{
public:
    USM();
    ~USM();

    void processing(const cv::Mat& src, cv::Mat& dst);

private:
};