
#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/Joy.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <camera_info_manager/camera_info_manager.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include<stdio.h>
#include <math.h>
#include <unistd.h>



using namespace std;


// this struct should hold all the interfaces for a given robot, each node should initialise the ones that it needs to use.


class camera_node{
private:
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::CameraPublisher camera;
    boost::shared_ptr<camera_info_manager::CameraInfoManager> cinfo_;

    int FLowH = 0;
    int FHighH = 23;

     int FLowS = 42;
    int FHighS = 255;

     int FLowV = 102;
    int FHighV = 255;
    
    int LLowH = 100;
    int LHighH = 164;

     int LLowS = 44;
    int LHighS = 182;

     int LLowV = 155;
    int LHighV = 255;

public:
    camera_node(int Hz);

};

camera_node::camera_node(int Hz):it_(nh_){
    ros::Rate rate(Hz);
    string camera_info_url, camera_name;
    camera_name = "fakeCamera";
    cinfo_.reset(new camera_info_manager::CameraInfoManager(nh_, camera_name, camera_info_url));

        if (!cinfo_->isCalibrated())
           {
             cinfo_->setCameraName(camera_name);
             sensor_msgs::CameraInfo camera_info;
             camera_info.header.frame_id = "godCamera";
             camera_info.width = 1280;
             camera_info.height = 1024;
             cinfo_->setCameraInfo(camera_info);
           }


    camera =it_.advertiseCamera("/camera",1);
    cv::Mat image,imgHSV,LimgThresholded,FimgThresholded, extracted;
    image = cv::imread("/home/god/uRobot_ws/src/urobot/src/wellLit.jpg");
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
    }
//     cv::namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
//    //imshow( "Display window", image );
//    //Create trackbars in "Control" window
//     cvCreateTrackbar("LowH", "Control", &FLowH, 179); //Hue (0 - 179)
//     cvCreateTrackbar("HighH", "Control", &FHighH, 179);

//      cvCreateTrackbar("LowS", "Control", &FLowS, 255); //Saturation (0 - 255)
//     cvCreateTrackbar("HighS", "Control", &FHighS, 255);

//      cvCreateTrackbar("LowV", "Control", &FLowV, 255); //Value (0 - 255)
//     cvCreateTrackbar("HighV", "Control", &FHighV, 255);

    // Setup SimpleBlobDetector parameters.
    cv::SimpleBlobDetector::Params params;

    // Change thresholds
    params.minThreshold = 10;
    params.maxThreshold = 12;

    // Filter by Area.
    params.filterByArea =false;
    params.minArea = 5;

    // Filter by Circularity
    params.filterByCircularity = false;
    params.minCircularity = 0.1;

    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 0.87;

    // Filter by Inertia
    params.filterByInertia = false;
    params.minInertiaRatio = 0.01;

    params.thresholdStep = 1;


    while(ros::ok()){
//        /////testing image segmentation. (to lazy to make the recieving node atm).
//        cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
//        cv::inRange(imgHSV, cv::Scalar(FLowH, FLowS, FLowV), cv::Scalar(FHighH,FHighS, FHighV), FimgThresholded); //Threshold the image
//        cv::inRange(imgHSV, cv::Scalar(LLowH, LLowS, LLowV), cv::Scalar(LHighH,LHighS, LHighV), LimgThresholded); //Threshold the image
//        //morphological opening (remove small objects from the foreground)
//        int erodeVal = 5;
//         cv::erode(FimgThresholded, FimgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erodeVal,erodeVal)) );
//         cv::dilate( FimgThresholded, FimgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erodeVal,erodeVal)) );
//         cv::erode(LimgThresholded, LimgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erodeVal,erodeVal)) );
//         cv::dilate( LimgThresholded, LimgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erodeVal,erodeVal)) );

//          //morphological closing (fill small holes in the foreground)
//         cv::dilate( FimgThresholded, FimgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erodeVal,erodeVal)) );
//         cv::erode(FimgThresholded, FimgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erodeVal,erodeVal)) );
//         cv::dilate( LimgThresholded, LimgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erodeVal,erodeVal)) );
//         cv::erode(LimgThresholded, LimgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erodeVal,erodeVal)) );


//         cv::threshold(FimgThresholded, FimgThresholded, 0.5, 255,cv::THRESH_BINARY_INV);
//         cv::threshold(LimgThresholded, LimgThresholded, 0.5, 255,cv::THRESH_BINARY_INV);


//         // Set up the detector with default parameters.
//         cv::SimpleBlobDetector detector(params);

//         // Detect blobs.
//         std::vector<cv::KeyPoint> Fkeypoints,Lkeypoints;


//         detector.detect(  FimgThresholded, Fkeypoints);
//         detector.detect(  LimgThresholded, Lkeypoints);

//         cv::Mat im_with_keypoints;
//         cv::drawKeypoints( image, Fkeypoints, im_with_keypoints, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
//         cv::drawKeypoints( im_with_keypoints, Lkeypoints, im_with_keypoints, cv::Scalar(0,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
//         cv::circle(im_with_keypoints, Lkeypoints.at(0).pt, 47, cv::Scalar(0,0,255) );


//         line(im_with_keypoints, Lkeypoints.at(0).pt, Fkeypoints.at(0).pt, cv::Scalar(255,0,255));
//         cv::Point2f centre = 0.55*Lkeypoints.at(0).pt + 0.45*Fkeypoints.at(0).pt;
//         cv::circle(im_with_keypoints, centre, 3, cv::Scalar(0,0,255) );

//         // Show blobs
//         cv::imshow("keypoints", im_with_keypoints );
//        ////// end of image testing.
        sensor_msgs::CameraInfoPtr ci(new sensor_msgs::CameraInfo(cinfo_->getCameraInfo()));
        cv_bridge::CvImage out_msg;
        out_msg.image = image;
        out_msg.encoding = sensor_msgs::image_encodings::BGR8;
        out_msg.header.stamp = ros::Time::now();
        out_msg.header.frame_id = "godCamera";
        camera.publish(*out_msg.toImageMsg(),*ci);
        cv::waitKey(1);
        ros::spinOnce();
        rate.sleep();
    }

}



int main(int argc, char** argv) {

    //ros setup
    ros::init(argc, argv, "camera_node");
    camera_node CN(30);
    return 0;
}
