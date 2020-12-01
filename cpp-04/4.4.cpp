# include <opencv2/opencv.hpp>
using namespace cv;

//マウスイベント練習とお絵かきプログラム

cv::Mat img(cv::Size(640, 480), CV_8UC3, cv::Scalar(255, 255, 255));
int vR = 0, vG = 0, vB = 255, vS = 8;

void onMouse(int event, int x, int y, int flags, void* userdata) {
    std::cout << "onMouse" << std::endl;
    if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)){
        circle(img, cv::Point(x, y), vS, cv::Scalar(vB, vG, vR), CV_FILLED, CV_AA);
    }
    else if (event == CV_EVENT_RBUTTONDOWN){
        img.setTo(255);
    }
    cv::imshow("paint", img);
}

int main () {
    cv::imshow("paint", img);
    cv::setMouseCallback("paint", onMouse);

    cv::createTrackbar("red", "paint", &vR, 255);
    cv::createTrackbar("green", "paint", &vG, 255);
    cv::createTrackbar("blue", "paint", &vB, 255);
    cv::createTrackbar("size", "paint", &vS, 255);

    int key = cv::waitKey(0);
    if (key == 's') {
        cv::imwrite("paint.png", img);
    }
}