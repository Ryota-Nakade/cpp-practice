//コントラスト強調とヒストグラム表示

#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(0);
    if(!cap.isOpened()) {
        std::cout << "can't open video" << std::endl;
        return -1;
    }
    cv::Mat src, dst, dst_cvt;

    cv::namedWindow("dst", cv::WINDOW_NORMAL);
    cv::namedWindow("dst_cvt", cv::WINDOW_NORMAL);
    cv::resizeWindow("dst", 640, 360);
    cv::resizeWindow("dst_cvt", 640, 360);

    std::vector<std::string> cName{"Blue", "Green", "Red"};
    std::vector<cv::Scalar> cScalar{cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), 
        cv::Scalar(0, 0, 0), cv::Scalar(0, 255, 125)};

    cv::Mat hist(cv::Size(256, 256), CV_8UC3);//ヒストグラム画面のサイズ
    cv::Mat hist_cvt(cv::Size(256, 256), CV_8UC3);//ヒストグラム画面のサイズ

    int min = 50;//しきい値．これ以下の輝度は0になる
    int max = 200;//しきい値．これ以上の輝度は255になる

    while (1) {
        cap >> src;
        
        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
        dst.convertTo(dst_cvt, dst.type(), 255.0 / (max - min), -255.0 * min / (max - min));

        std::vector<cv::Mat> ch(3);
        cv::split(src, ch);
        ch.push_back(dst);
        ch.push_back(dst_cvt);
        
        std::vector<int> size{256};//binの本数
        std::vector<float> range{0, 256};//ヒストグラムの範囲．左の値”以上”，右の値”未満”
        hist.setTo(255);
        for (int c = 3; c < 5; c++ ) {
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
        cv::imshow("dst", dst);
        cv::imshow("dst_cvt", dst_cvt);
        cv::imshow("histgram", hist);

        if(cv::waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}