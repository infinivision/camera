#ifndef __MTCNN_H__
#define __MTCNN_H__

#include <vector>
#include "ncnn/net.h"

using namespace std;
using namespace cv;

class Bbox {
public:
    Bbox(){};

    inline void scale(float factor_x, float factor_y) {
        this->x1 = round(this->x1 * factor_x);
        this->x2 = round(this->x2 * factor_x);
        this->y1 = round(this->y1 * factor_y);
        this->y2 = round(this->y2 * factor_y);

        for (int i=0; i<5; i++) {
            this->ppoint[i] *= factor_x;
            this->ppoint[i+5] *= factor_y;
        }
    }

    float score;
    int x1;
    int y1;
    int x2;
    int y2;
    float area;
    bool exist;
    float ppoint[10];
    float regreCoord[4];
};

struct orderScore
{
    float score;
    int oriOrder;
};


class MTCNN{
public:
    MTCNN();
    MTCNN(const std::string& model_path);
    void detect(ncnn::Mat& img_, std::vector<Bbox>& finalBbox);

private:
    void generateBbox(ncnn::Mat score, ncnn::Mat location, vector<Bbox>& boundingBox_, vector<orderScore>& bboxScore_, float scale);
    void nms(vector<Bbox> &boundingBox_, std::vector<orderScore> &bboxScore_, const float overlap_threshold, string modelname="Union");
    void refineAndSquareBbox(vector<Bbox> &vecBbox, const int &height, const int &width);

    ncnn::Net pnet_, rnet_, onet_;
    ncnn::Mat img;

    const float nms_threshold[3] = {0.5, 0.7, 0.7};
    const float threshold[3] = {0.7, 0.6, 0.8};
    const float mean_vals[3] = {127.5, 127.5, 127.5};
    const float norm_vals[3] = {0.0078125, 0.0078125, 0.0078125};
    std::vector<Bbox> firstBbox_, secondBbox_,thirdBbox_;
    
    std::vector<orderScore> firstOrderScore_, secondBboxScore_, thirdBboxScore_;
    int img_w, img_h;
};


#endif
