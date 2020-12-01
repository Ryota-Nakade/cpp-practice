#include <opencv2/opencv.hpp>

//トラックバー練習と2値化プログラム

double brightness = 1.0;

void changeBrightness(int position, void* userdata) {
    brightness = (double)position /100.0 ;
    std::cout << "明るさ倍率" << brightness << std::endl;
}

int main() {
    int val = 128;
    cv::Mat src, gray, dst_otsu, dst;

    //ファイルorカメラを開く
    //X in "cap(X)" is device number. default = 0
    //cv::VideoCapture cap(X);
    cv::VideoCapture cap("../image/MOV_0202.mp4");

    //読み込みエラー時の処理
    if (!cap.isOpened()) {
        std::cout << "Could not open video file!!" << std::endl;
        std::cin.get();
        return -1;
    }

    //ウィンドウのリサイズをするために，ここで宣言する．（表示上のサイズ）
    //出力画像のサイズそのものを変えるわけではない．

    //imshowだとMatサイズに一致するサイズが自動に開かれてしまうため
    //WINDOW_NORMALを指定するとリサイズ可能になる．値0
    //WINDOW_AUTOSIZEではMatのサイズ固定になる．値1
    cv::namedWindow("source video", CV_WINDOW_NORMAL);
    cv::namedWindow("2ti video", CV_WINDOW_NORMAL);
    cv::namedWindow("2ti video otsu", CV_WINDOW_NORMAL);
    //resizeWindow関数でウィンドウの大きさ調整する
    cv::resizeWindow("source video", 800, 600);
    cv::resizeWindow("2ti video", 800, 600);
    cv::resizeWindow("2ti video otsu", 800, 600);

    //track bar
    cv::createTrackbar("Threshold", "2ti video", &val, 255);
    cv::createTrackbar("brightness", "source video", NULL, 200, changeBrightness);
    cv::setTrackbarPos("brightness", "source video", 100);

    while (1)
    {
        cap >> src;
        if(src.empty()) {
            return -1;
        }
        src = src * brightness;
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

        //Min:BlackーMax:White
        cv::threshold(gray, dst, val, 255, CV_THRESH_BINARY);
        //Min:White-Max:Black
        //cv::threshold(gray, dst,  val, 255, CV_THRESH_BINARY_INV);
        cv::threshold(gray, dst_otsu, val, 255, CV_THRESH_OTSU);
        cv::imshow("source video", src);
        cv::imshow("2ti video", dst);
        cv::imshow("2ti video otsu", dst_otsu);

        if(cv::waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}