#include <opencv2/opencv.hpp>

//移動物体の抽出

int main() {
    cv::VideoCapture cap("../image/2020-11-30-150819.mp4");
    if(!cap.isOpened()) {
        std::cout << "Could not open video file!" << std::endl;
        std::cin.get();
        return -1;
    }
    cv::namedWindow("source", CV_WINDOW_NORMAL);
    cv::resizeWindow("source", 640, 480);
    cv::namedWindow("background", CV_WINDOW_NORMAL);
    cv::resizeWindow("background", 640, 480);
    cv::namedWindow("difference", CV_WINDOW_NORMAL);
    cv::resizeWindow("difference", 640, 480);
    cv::namedWindow("Inverse", CV_WINDOW_NORMAL);
    cv::resizeWindow("Inverse", 640, 480);

    cv::Size frame((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    cv::Mat src, diff, diffInv, srcFloat, dstFloat, diffFloat;
    dstFloat.create(frame, CV_32FC3);//小数領域の確保
    dstFloat.setTo(0.0);//0.0で初期化

    while (1) {
        cap >> src;
        if(src.empty()) {
            break;
        }
        cv::imshow("source", src);

        src.convertTo(srcFloat, CV_32FC3, 1 / 255.0);
        cv::addWeighted(srcFloat, 0.01, dstFloat, 0.99, 0, dstFloat, -1);
        cv::imshow("background", dstFloat);

        cv::absdiff(srcFloat, dstFloat, diffFloat);//差分の絶対値を得る (Absolute value)
        cv::imshow("difference", diffFloat);

        diffFloat.convertTo(diff, CV_8UC3, 255.0);//反転させるために符号なし整数型に戻す
        cv::bitwise_not(diff, diffInv);//階調を反転
        cv::imshow("Inverse", diffInv);

        if(cv::waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}