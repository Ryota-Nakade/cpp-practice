//処理時間を計測・表示するプログラム

#include <opencv2/opencv.hpp>

void showFPS(cv::Mat &img, double period) {
    double fps = 1000.0 / period;
    //文字表示
    std::stringstream st;
    st.precision(1);//小数点以下を1桁に設定
    st << std::fixed << period << " ms " << fps << " fps";
    cv::putText(img, st.str(), cv::Point(16,64), CV_FONT_ITALIC, 1.5, cv::Scalar::all(255), 5);
}

int main () {
    cv::VideoCapture cap("../image/MOV_0202.mp4");
    int numDilate = 8;
    double tickS = cv::getTickFrequency();
    cv::namedWindow("dilate処理", CV_WINDOW_NORMAL);
    cv::resizeWindow("dilate処理", 640, 480);

    cv::createTrackbar("処理数", "dilate処理", &numDilate, 32);
    cv::Mat src, dst;

    while (1)
    {
        cap >> src;

        if(src.empty()) {
            break;
        }
        int64 startTick = cv::getTickCount();
        dilate(src, dst, cv::noArray(), cv::Point(-1, -1), numDilate);
        double period = (cv::getTickCount() - startTick) * 1000.0 / tickS;

        showFPS(dst, period);
        cv::imshow("dilate処理", dst);
        if (cv::waitKey(10) == 27) {
            break;
        }
    }
    
}