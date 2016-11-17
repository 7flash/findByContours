#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
using namespace cv;
using namespace std;

void thresh1_callback(int, void*);
void thresh2_callback(int, void*);
void contourSizeMinFirst_callback(int, void*);
void contourSizeMaxFirst_callback(int, void*);
void contourSizeMinSecond_callback(int, void*);
void contourSizeMaxSecond_callback(int, void*);
void partWindowMouseCallback(int event, int x, int y, int flags, void* userdata);
void fullWindowMouseCallback(int event, int x, int y, int flags, void* userdata);
void draw(int thresh1, int thresh2, int contourSizeMinFirst, int contourSizeMaxFirst, int contourSizeMinSecond, int contourSizeMaxSecond);

int thresh1 = 240;
int thresh2 = 240;
int contourSizeMinFirst = 20000;
int contourSizeMinSecond = 2000;
int contourSizeMaxFirst = 130000;
int contourSizeMaxSecond = 130000;

vector<vector<Point> > contours;
vector<vector<Point> > contours2;

int main()
{
    namedWindow("Controls", 1);
    createTrackbar( "Threshold 1:", "Controls", &thresh1, 255, thresh1_callback);
    createTrackbar( "Threshold 2:", "Controls", &thresh2, 255, thresh2_callback);
    createTrackbar( "Contour size MIN 1:", "Controls", &contourSizeMinFirst, 100000, contourSizeMinFirst_callback);
    createTrackbar( "Contour size MAX 1:", "Controls", &contourSizeMaxFirst, 300000, contourSizeMaxFirst_callback);
    createTrackbar( "Contour size MIN 2:", "Controls", &contourSizeMinSecond, 100000, contourSizeMinSecond_callback);
    createTrackbar( "Contour size MAX 2:", "Controls", &contourSizeMaxSecond, 300000, contourSizeMaxSecond_callback);
    draw(thresh1, thresh2, contourSizeMinFirst, contourSizeMaxFirst, contourSizeMinSecond, contourSizeMaxSecond);
    waitKey(0);
    return 0;
}

void draw(int thresh1, int thresh2, int contourSizeMinFirst, int contourSizeMaxFirst, int contourSizeMinSecond, int contourSizeMaxSecond)
{
    Mat src;
    Mat src2;
    src = imread("c564828e5a0615bccbc5a11a12d8c837.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    src2 = imread("14bedc2f8e603338cb0c90fe68841114.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat gray;
    Mat gray2;
    threshold(src, gray, thresh1, 255,THRESH_BINARY);
    threshold(src2, gray2, thresh2, 255, THRESH_BINARY);
    vector<Vec4i> hierarchy;
    vector<Vec4i> hierarchy2;
    findContours( gray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
    findContours( gray2, contours2, hierarchy2, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
    vector<vector<Point> >::iterator it = contours.begin();
    vector<vector<Point> >::iterator it2 = contours2.begin();
    for( ; it != contours.end(); )
    {
        if(contourArea(*it) < contourSizeMinFirst || contourArea(*it) > contourSizeMaxFirst) {
            it = contours.erase(it);
        } else {
            it++;
        }
    }
    for( ; it2 != contours2.end(); )
    {
        if(contourArea(*it2) < contourSizeMinSecond || contourArea(*it2) > contourSizeMaxSecond) {
            it2 = contours2.erase(it2);
        }
        else
        {
            it2++;
        }
    }
    RNG rng(12345);
    Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
    Mat drawing2 = Mat::zeros( gray2.size(), CV_8UC3 );
    map<int, int> matchContours;
    map<int, double> matchWeights;
    for(map<int, double>::iterator it = matchWeights.begin(); it != matchWeights.end(); ++it)
    {
        it->second = -1;
    }
    for(int i = 0; i < contours.size(); i++)
    {
        for(int j=0; j < contours2.size(); j++)
        {
            double match = cv::matchShapes(contours[i], contours2[j], CV_CONTOURS_MATCH_I1, 0);
            if(matchWeights.find(i) == matchWeights.end() || match < matchWeights[i])
            {
                matchContours[i] = j;
                matchWeights[i] = match;
            }
        }
    }
    for(int i = 0; i < contours2.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours(drawing2, contours2, i, color, 1, 8);
    }
    for(int i = 0; i< contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours(drawing, contours, i, color, 1, 8);
    }
    for (map<int, int>::iterator it2 = matchContours.begin(); it2 != matchContours.end(); ++it2)
    {
        cout << it2->first << " : " << it2->second << endl;
        Scalar color = Scalar(255, 0, 0);
        drawContours(drawing2, contours2, it2->second, color, 2, 8);
    }
    namedWindow("Full", WINDOW_NORMAL);
    namedWindow("Part", WINDOW_NORMAL);
    imshow("Full", drawing2 );
    imshow("Part", drawing);
    resizeWindow("Full", 1000, 1080);
    setMouseCallback("Full", fullWindowMouseCallback, NULL);
    setMouseCallback("Part", partWindowMouseCallback, NULL);
}

void thresh1_callback(int, void*)
{
    draw(thresh1, thresh2, contourSizeMinFirst, contourSizeMaxFirst, contourSizeMinSecond, contourSizeMaxSecond);
}

void thresh2_callback(int, void*)
{
    draw(thresh1, thresh2, contourSizeMinFirst, contourSizeMaxFirst, contourSizeMinSecond, contourSizeMaxSecond);
}

void contourSizeMinFirst_callback(int, void*)
{
    draw(thresh1, thresh2, contourSizeMinFirst, contourSizeMaxFirst, contourSizeMinSecond, contourSizeMaxSecond);
}

void contourSizeMaxFirst_callback(int, void*)
{
    draw(thresh1, thresh2, contourSizeMinFirst, contourSizeMaxFirst, contourSizeMinSecond, contourSizeMaxSecond);
}

void contourSizeMinSecond_callback(int, void*)
{
    draw(thresh1, thresh2, contourSizeMinFirst, contourSizeMaxFirst, contourSizeMinSecond, contourSizeMaxSecond);
}

void contourSizeMaxSecond_callback(int, void*)
{
    draw(thresh1, thresh2, contourSizeMinFirst, contourSizeMaxFirst, contourSizeMinSecond, contourSizeMaxSecond);
}

void partWindowMouseCallback(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
        pair<int, int> matchContour = make_pair(-1, -1);
        vector<Point> currentContour;
        for(int i = 0; i<contours.size(); i++)
        {
            if(pointPolygonTest(contours[i], Point2f(x, y), false) == 1)
            {
                if(matchContour.first == -1 || contourArea(contours[i]) <= matchContour.second)
                {
                    matchContour = make_pair(i, contourArea(contours[i]));
                }
            }
        }
        if(matchContour.first != -1) {
            currentContour = contours[matchContour.first];
            cout << "Current part contour ID: " << matchContour.first << endl;
        }
     }
}

void fullWindowMouseCallback(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
        pair<int, int> matchContour = make_pair(-1, -1);
        vector<Point> currentContour;
        for(int i = 0; i<contours2.size(); i++)
        {
            if(pointPolygonTest(contours2[i], Point2f(x, y), false) == 1)
            {
                if(matchContour.first == -1 || contourArea(contours2[i]) <= matchContour.second)
                {
                    matchContour = make_pair(i, contourArea(contours2[i]));
                }
            }
        }
        if(matchContour.first != -1) {
            currentContour = contours2[matchContour.first];
            cout << "Current full contour ID: " << matchContour.first << endl;
        }
     }
}
