#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>



int main() {

    //cv::namedWindow("Example1",cv::WINDOW_NORMAL);
    cv::namedWindow("original", cv::WINDOW_NORMAL);

    //cv::resizeWindow("Example1", 800, 600);
    cv::resizeWindow("original", 800, 600);

    cv::VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) {
        std::cerr << "Can not open web-camera!" << std::endl;
    }

    cv::Scalar lowerSkin = cv::Scalar(0, 45, 80);   //  area of skin's HSV
    cv::Scalar upperSkin = cv::Scalar(20, 255, 220); // 

    cv::Mat frame,mask;
    while (true){
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0);

        // skin mask in RGB
        mask = cv::Mat::zeros(frame.size(), CV_8UC1);
        for (int y = 0; y < frame.rows; y++) {
            for (int x = 0; x < frame.cols; x++) {
                cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x);
                int B = pixel[0], G = pixel[1], R = pixel[2];

                // pale skin conditions
                bool isSkin = (R > 95) && (G > 40) && (B > 20) && (R > G) && (R > B) && (abs(R - G) > 15) && (R - std::min(G, B) > 15);
                if (isSkin) mask.at<uchar>(y, x) = 255;
            }
        }

        // Улучшение маски (морфология)
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

        // Поиск контуров
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  

        //-------------------HSV FINDING SKIN METHOD----------------------------------------------------------------------------
        // 
        //// binarisation by skin color   
        //cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        //cv::inRange(hsv, lowerSkin, upperSkin, mask);

        //// cleaning noises
        //cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
        //cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

        //// finding contours
        //std::vector<std::vector<cv::Point>> contours;
        //cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        //--------------------------------------------------------------------------------------------------------------------

        if (!contours.empty()) {
            // max S  =  hand (in species cases - it is happend as face)
            auto maxContour = *std::max_element(
                contours.begin(),
                contours.end(),
                [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                }
            );

            cv::Rect rect = cv::boundingRect(maxContour);
            cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
        }

        //cv::imshow("Example1", mask);
        cv::imshow("original", frame);
        if (cv::waitKey(33) >= 0) break;
    }
    return 0;
}
