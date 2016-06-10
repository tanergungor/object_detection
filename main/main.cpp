#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    /*
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }
    */

    const char * path = "images/03.jpg";


    // Read the image
    Mat src = imread(path, IMREAD_COLOR);

    // Check for invalid input
    if(! src.data )
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    // Create the grayscale image
    Mat src_gray = src;

    // Check if it is RGB or not
    if(src.channels() == 3)
    {
        // Convert the RGB image to grayscale
        cvtColor(src, src_gray, CV_BGR2GRAY);
    }

    // Create the binary image
    Mat src_binary;
    // Apply threshold operation to discard the objects whose color are not white
    threshold( src_gray, src_binary, 205, 255, THRESH_BINARY );


















    Mat src_opening;
    double dilation_size = 10;
    Mat dilation_element = getStructuringElement(MORPH_ELLIPSE, Size(2 * dilation_size, 2 * dilation_size), Point(dilation_size, dilation_size));
    dilate(src_binary, src_opening, dilation_element);

    // delete the component in the border
    // Get connected components and stats
    Mat labels, stats, centroids, only;

    int nLabels = connectedComponentsWithStats(src_opening, labels, stats, centroids);

    // Statistics
    // cout << "Show statistics and centroids:" << endl;
    // cout << "stats:" << endl << "(left,top,width,height,area)" << endl << stats << endl << endl;
    // cout << "centroids:" << endl << "(x, y)" << endl << centroids << endl << endl;

    for(int i = 1; i < nLabels; i++)
    {
        if(stats.at<int>(i, CC_STAT_LEFT) == 0 ||
           stats.at<int>(i, CC_STAT_TOP)  == 0 ||
           stats.at<int>(i, CC_STAT_LEFT) + stats.at<int>(i, CC_STAT_WIDTH ) == src_opening.size().width  ||
           stats.at<int>(i, CC_STAT_TOP)  + stats.at<int>(i, CC_STAT_HEIGHT) == src_opening.size().height)
        {
            compare(labels, i, only, CMP_EQ);

            src_opening = src_opening - only;
        }
    }

    Mat src_closing;
    double erosion_size = 18;
    Mat erosion_element = getStructuringElement(MORPH_ELLIPSE, Size(2 * erosion_size, 2 * erosion_size), Point(erosion_size, erosion_size));
    erode(src_opening, src_closing, erosion_element);


























    // ONLY ONE BLOB
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // Find contours
    findContours(src_closing, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    if(contours.size() != 0)
    {
        // Get the moments
        Moments mu = moments(contours[0], false);

        // Get the mass centers:
        Point2f mc = Point2f(mu.m10/mu.m00 , mu.m01/mu.m00);

        // Draw the center of mass
        circle(src, mc, 5, Scalar(255, 0, 0), -1);
    }










    // Create a window for display
    namedWindow("Output Image - 4", WINDOW_AUTOSIZE);

    // Show our image inside it
    imshow("Output Image - 4", src);

    imwrite("images/output.jpg", src);

    // Wait for a keystroke in the window
    waitKey(0);

    return 0;
}
