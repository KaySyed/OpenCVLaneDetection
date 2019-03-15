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
    int countx = 0;
    for (int i = 0; i < whitePointsL.size(); i++)
    {
        if (whitePointsL[i].y == whitePointsR[i].y && whitePointsL[i].y > image.rows / 2)
        {
            int centerx = ((whitePointsR[i].x - whitePointsL[i].x) / 2) + whitePointsL[i].x;
            Point p = Point(centerx, whitePointsL[i].y);
            circle(image, p, 1, Scalar(0, 0, 0), 5, 8);
            centerpoints.push_back(p);
            countx += centerx;
        }
    }
    float angle; // = atan2(centerpoints[0].y - p2.y, p1.x - p2.x)
    int centerpointscount = centerpoints.size();

    int trackerror = (image.cols/2)- (countx/centerpointscount);

    cout<<"\nImage center = "<<image.cols/2<<"\nAverage = "<<countx/centerpointscount<<"\nTrack Error = "<<trackerror;
    if (centerpointscount > 1){
        Point p1 = Point(centerpoints[0].x, centerpoints[0].y);
        Point p2 = Point(centerpoints[centerpointscount-1].x, centerpoints[centerpointscount-1].y);
        angle = atan2(p2.y - p1.y, p2.x - p1.x);
        line(image, p2, p1, Scalar(0,255,255), 5, 8);
        angle = angle * 180 / 3.14;
    }
    int angleerror = (angle - 90);
    cout<< "\nAngle = "<<angle;
    cout<< "\nAngle Error = "<<angleerror<<"\n";
    String x = "Image center = " + to_string(image.cols/2) + ", Average = " + to_string(countx/centerpointscount) + ", Track Error = " + to_string(trackerror) + ", Angle = " + to_string(angle) + ", Angle Error = " + to_string(angleerror);
    putText(image, x , Point(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,0), 1.1);
    imshow("test", image);

    waitKey(30000);

    return 0;
}