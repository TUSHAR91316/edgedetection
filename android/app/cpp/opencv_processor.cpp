#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

Mat detectEdges(const Mat &input) {
    Mat gray, edges, output;
    cvtColor(input, gray, COLOR_RGBA2GRAY);
    Canny(gray, edges, 100, 200);

    output = Mat::zeros(input.size(), CV_8UC4);
    for (int y = 0; y < edges.rows; y++) {
        for (int x = 0; x < edges.cols; x++) {
            if (edges.at<uchar>(y, x) > 0) {
                output.at<Vec4b>(y, x) = Vec4b(0, 255, 0, 255);
            }
        }
    }
    return output;
}
