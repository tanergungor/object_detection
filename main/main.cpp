#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// The kernel size for the dilation
#define DILATION_SIZE 10
// The kernel size for the erosion
#define EROSION_SIZE  18

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if(argc != 2)
    {
     cout << "Usage: main.exe [IMAGE URL]" << endl;
     return -1;
    }

    // Read the image
    Mat src = imread(argv[1], IMREAD_COLOR);

    // Check for invalid input
    if(!src.data)
    {
        cout << argv[1] << endl ;
        cout << "Could not open or find the image" << endl ;
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

    // Create the dilation imaging
    // more information --> http://homepages.inf.ed.ac.uk/rbf/HIPR2/dilate.htm
    Mat src_dilation;
    // Create the dilation kernel
    Mat dilation_element = getStructuringElement(MORPH_ELLIPSE, Size(2 * DILATION_SIZE, 2 * DILATION_SIZE), Point(DILATION_SIZE, DILATION_SIZE));
    // Dilate the binary image
    dilate(src_binary, src_dilation, dilation_element);

    Mat labels, stats, centroids;

    // Get the connected components and stats
    int label_size = connectedComponentsWithStats(src_dilation, labels, stats, centroids);

    // Suppress the structures connected to image border
    for(int i = 1; i < label_size; i++)
    {
        // Check if the blob is connected to image border
        if(stats.at<int>(i, CC_STAT_LEFT) == 0 ||
           stats.at<int>(i, CC_STAT_TOP)  == 0 ||
           stats.at<int>(i, CC_STAT_LEFT) + stats.at<int>(i, CC_STAT_WIDTH ) == src_dilation.size().width  ||
           stats.at<int>(i, CC_STAT_TOP)  + stats.at<int>(i, CC_STAT_HEIGHT) == src_dilation.size().height)
        {
            // Create the blob
            Mat label;
            // Extract the current blob from the image
            compare(labels, i, label, CMP_EQ);
            // Delete the current blob from the image, because it is connected to the border
            src_dilation = src_dilation - label;
        }
    }

    // Create the erosion imaging
    // more information --> http://homepages.inf.ed.ac.uk/rbf/HIPR2/erode.htm
    Mat src_erosion;
    // Create the erosion kernel
    Mat erosion_element = getStructuringElement(MORPH_ELLIPSE, Size(2 * EROSION_SIZE, 2 * EROSION_SIZE), Point(EROSION_SIZE, EROSION_SIZE));
    // Erode the binary image
    erode(src_dilation, src_erosion, erosion_element);

    // Create a vector that holds contours or blobs
    vector<vector<Point>> contours;

    // WARNING::: tune parameters
    // Find the contours or blobs
    findContours(src_erosion, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    // WARNING::: MAYBE ORE THAN ONE BLOB CAN BE DETECTED!!!
    // Check if blob is detected
    if(contours.size() > 0)
    {
        // Get the moments
        Moments mu = moments(contours[0], false);

        // Get the mass centers:
        Point2f mc((float)(mu.m10/mu.m00), (float)(mu.m01/mu.m00));

        // Draw the center of mass
        circle(src, mc, 5, Scalar(255, 0, 0), -1);
    }

    // Create a window for display
    namedWindow("Output Image", WINDOW_AUTOSIZE);

    // Show our image inside it
    imshow("Output Image", src);

    imwrite("images/output.jpg", src);

    // Wait for a keystroke in the window
    waitKey(0);

    return 0;
}
