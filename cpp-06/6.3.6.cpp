#include<iostream>
#include<cmath>
#include<opencv2/opencv.hpp>

//ネガポジ反転とヒストグラム表示

int main()
{
    cv::VideoCapture cap("../image/MOV_0202.mp4");
    if (!cap.isOpened()) {
        std::cout << "error" << std::endl;
        return -1;
    }

    cv::Mat src, dst;

    cv::namedWindow("src", cv::WINDOW_NORMAL);
    cv::namedWindow("dst", cv::WINDOW_NORMAL);
    cv::namedWindow("hist", cv::WINDOW_NORMAL);
    cv::resizeWindow("src", 640, 360);
    cv::resizeWindow("dst", 640, 360);
    cv::resizeWindow("hist", 640, 360);

    std::vector<std::string> cName{"Blue", "Green", "Red"};
    std::vector<cv::Scalar> cScalar{cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255)};

    cv::Mat hist(cv::Size(256, 256), CV_8UC3);

    while (1) {
        cap >> src;
        if(src.empty()) {
            break;
        }

        src.convertTo(dst, src.type(), -1.0, 255.0);
        std::vector<cv::Mat> ch(3);
        cv::split(dst, ch);

        std::vector<int> size{256};//binの本数
        std::vector<float> range{0, 256};//ヒストグラムの範囲．左の値”以上”，右の値”未満”
        hist.setTo(255);
        for (int c = 0; c < 3; c++ ) {
            std::vector<int> channel{c};//チャンネル指定用
            cv::Mat bins;//ヒストグラムの結果格納用配列
            cv::calcHist(ch, channel, cv::noArray(), bins, size, range);
            cv::Point prev = cv::Point(0, 255);
            for (int x = 0; x < 256; x++) {
                cv::Point current = cv::Point(x,256 - (int) (bins.at<float>(x) / 75 ));//75で割って大きさ調整する
                cv::line(hist, prev, current, cScalar[c]);
                prev = current;
            }
        }

        cv::imshow("src", src);
        cv::imshow("dst", dst);
        cv::imshow("hist", hist);

        if(cv::waitKey(30) == 27) {
            break;
        };
    }
    return 0;
}
