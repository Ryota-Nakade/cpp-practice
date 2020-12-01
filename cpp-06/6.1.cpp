#include <opencv2/opencv.hpp>

//ヒストグラム表示プログラム

int main() {
    cv::VideoCapture cap("../image/MOV_0201.mp4");
    if(!cap.isOpened()) {
        std::cout << "Could not open video file" << std::endl;
        std::cin.get();
        return -1;
    }

    cv::namedWindow("source", CV_WINDOW_NORMAL);
    cv::resizeWindow("source", 640, 360);

    cv::Mat src;

    std::vector<std::string> cName{"Blue", "Green", "Red"};
    std::vector<cv::Scalar> cScalar{cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255)};

    cv::Mat hist(cv::Size(256, 256), CV_8UC3);//ヒストグラム画面のサイズ

    while (1)
    {
        cap >> src;
        if(src.empty()) {
            break;
        }
        cv::imshow("source", src);
        std::vector<cv::Mat> ch(3);
        cv::split(src, ch);

        std::vector<int> size{256};//binの本数
        std::vector<float> range{0, 256};//ヒストグラムの範囲．左の値”以上”，右の値”未満”
        hist.setTo(255);
        for (int c = 0; c < 2; c++ ) {
            std::vector<int> channel{c};
            cv::Mat bins;
            cv::calcHist(ch, channel, cv::noArray(), bins, size, range);
            cv::Point prev = cv::Point(0, 255);
            for (int x = 0; x < 256; x++) {
                cv::Point current = cv::Point(x,256 - (int) (bins.at<float>(x) / 75 ));//50で割って大きさ調整する
                cv::line(hist, prev, current, cScalar[c]);
                prev = current;
            }
        }
        cv::imshow("histgram", hist);
        if (cv::waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}