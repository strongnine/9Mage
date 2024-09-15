#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

uint64_t GetPIDTimeInNanoseconds(uint64_t start, uint64_t end);

class SECEDCT {
public:
    SECEDCT();
    ~SECEDCT();

    void processing(const cv::Mat& src, cv::Mat& dst);

private:
    int   yu = 255;
    int   yd = 0;
    int   region_num;
    float _EPS = 1e-7;
    float sum_entropy = 0.0;
    float sum_region_hist = 0.0;
    float sum_fk = 0.0;

    vector<int>         global_histogram;
    vector<vector<int>> spatial_histogram;
    vector<float>       entropy;
    vector<float>       fk;
    vector<float>       fk_norm;
    vector<float>       F_cdf;
    vector<int>         ymap;

    void calc_global_histogram(const Mat& img);
    void calc_global_entropy();
    void calc_spatial_histogram(const Mat& img);
    void calc_spatial_entropy();
    void calc_mapping();
    void pixel_mapping(const Mat& src, Mat& dst, const vector<int>& map);
};