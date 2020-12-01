#include <opencv2/opencv.hpp>

//特徴点検出とマッチングプログラム

int main() {
    cv::VideoCapture cap("../image/MOV_0202.mp4");
    if (!cap.isOpened()) {
        std::cout << "COuld not open video!" << std::endl;
        std::cin.get();
        return -1;
    }
    
    cv::Mat src;
    
    return 0; 
}