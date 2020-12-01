#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap("../image/MOV_0202.mp4");
    if (!cap.isOpened()) {
        std::cout << "Could not open video file!!" << std::endl;
        std::cin.get();
        return -1;
    }

    cv::Mat img, dst;
    int i=0;

    while (1)
    {
        /* code */
        cap >> img;
        if(img.empty()) {
            break;
        }
        cv::Canny(img, dst, 70, 100);//src,dst,threshold,renzoku
        
        cv::imshow("video", dst);
        if (cv::waitKey(20) == 27) {
            //ASCII code27 == Esc key. when Esc was pushed, it will break.
            //waitKey(xx) <- xx is frame rate.
            std::cout << "broken" << std::endl;
            break;
        }
        i++;
        std::cout << "ok No " << i << std::endl;
    }
    return 0;
}