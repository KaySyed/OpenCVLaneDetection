#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    // Read the image file
    Mat image = imread("img.png");

    Point2f p = Point(0, 0);
    Mat outputImagebw;
    vector<Point2f> whitePointsR;
    vector<Point2f> whitePointsL;
    int currentIndexL = 0;
    int currentIndexR = 0;
    int threshold = 230;
    int sectionSize = 20;

    cv::cvtColor(image, outputImagebw, CV_BGR2GRAY);

    for (int i = 0; i < outputImagebw.rows; i++)
    {
        //Point2f p = Point(0, 0);

        for (int j = outputImagebw.cols / 2; j > 0; j--)
        {
            if (outputImagebw.at<uchar>(i, j) > threshold)
            {
                if (whitePointsL.size() > 0)
                {
                    if (whitePointsL[currentIndexL - 1].y < i - sectionSize)
                    {
                        whitePointsL.push_back(Point(j, i));
                        circle(image, Point(j, i), 1, Scalar(255, 0, 0), 5, 8);
                        currentIndexL++;
                    }
                }
                else
                {
                    whitePointsL.push_back(Point(j, i));
                }
            }
        }
        for (int j = outputImagebw.cols / 2; j < outputImagebw.cols; j++)
        {
            if (outputImagebw.at<uchar>(i, j) > threshold)
            {
                if (whitePointsR.size() > 0)
                {
                    if (whitePointsR[currentIndexR - 1].y < i - sectionSize)
                    {
                        whitePointsR.push_back(Point(j, i));
                        circle(image, Point(j, i), 1, Scalar(0, 0, 255), 5, 8);
                        currentIndexR++;
                    }
                }
                else
                {
                    whitePointsR.push_back(Point(j, i));
                }
            }
        }
    }

    vector<Point2f> centerpoints;

    for (int i = 0; i < whitePointsL.size(); i++)
    {
        if (whitePointsL[i].y == whitePointsR[i].y && whitePointsL[i].y > image.rows / 2)
        {
            int centerx = ((whitePointsR[i].x - whitePointsL[i].x) / 2) + whitePointsL[i].x;
            Point p = Point(centerx, whitePointsL[i].y);
            circle(image, p, 1, Scalar(0, 0, 0), 5, 8);
            centerpoints.push_back(p);
        }
    }

    imshow("test", image);

    waitKey(30000);

    return 0;
}