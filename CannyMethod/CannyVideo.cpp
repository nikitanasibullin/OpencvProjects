#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>



int main() {


    cv::namedWindow("Example1",cv::WINDOW_NORMAL);
    cv::namedWindow("original", cv::WINDOW_NORMAL);

    cv::resizeWindow("Example1", 800, 600);
    cv::resizeWindow("original", 800, 600);

    cv::VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) {
        std::cerr << "Can not open web-camera!" << std::endl;
    }

    cv::Mat frame, chFrame, gaussed;
    while (true){
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        cv::cvtColor(frame, gaussed, cv::COLOR_BGR2GRAY);
        cv::pyrDown(gaussed, gaussed);
        cv::pyrDown(gaussed, gaussed);
        cv::Canny(gaussed, chFrame,10,100,3,true);
        cv::imshow("Example1", chFrame);
        cv::imshow("original", frame);
        if (cv::waitKey(33) >= 0) break;
    }
    return 0;
}
