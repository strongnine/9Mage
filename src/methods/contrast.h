#include <arm_neon.h>
#include <numeric>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;

uint64_t GetPIDTimeInNanoseconds(uint64_t start, uint64_t end);

class SEGCE
{
public:
    SEGCE();
    ~SEGCE();

    void processing(const cv::Mat& src, cv::Mat& dst, string acc = "none");

private:
    int yu = 255;
    int yd = 0;
    int region_num;

    int height;
    int width;
    int stride;

    float _EPS            = 1e-7;
    float sum_entropy     = 0.0;
    float sum_region_hist = 0.0;
    float sum_fk          = 0.0;

    vector<int>         global_histogram;
    vector<vector<int>> spatial_histogram;
    vector<float>       entropy;
    vector<float>       fk;
    vector<float>       fk_norm;
    vector<float>       F_cdf;
    vector<int>         ymap;

    void calc_global_histogram(const cv::Mat& img);
    void calc_global_entropy();
    void calc_spatial_histogram(const cv::Mat& img);
    void calc_spatial_entropy();
    void calc_mapping();
    void pixel_mapping(const cv::Mat& src, cv::Mat& dst);

    void calc_spatial_histogram(const uint8_t* p_img);
    void pixel_mapping(const uint8_t* p_src, uint8_t* p_dst);
    void pixel_mapping_neon(const uint8_t* __restrict src, uint8_t* __restrict dst);
};