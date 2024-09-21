#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class USM {
public:
    USM();
    ~USM();

    void processing(const cv::Mat& src, cv::Mat& dst);

private:
};