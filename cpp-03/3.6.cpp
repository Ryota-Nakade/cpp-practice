#include <opencv2/opencv.hpp>

int main() {
    //ファイルorカメラを開く
    //X in "cap(X)" is device number. default = 0
    //cv::VideoCapture cap(X);
    cv::VideoCapture cap("../image/MOV_0202.mp4");

    //エラー処理
    if (!cap.isOpened()) {
        std::cout << "Could not open video" << std::endl;
        std::cin.get();
        return -1;
    }

    //ウィンドウのリサイズをするために，ここで宣言する．（表示上のサイズ）
    //出力画像のサイズそのものを変えるわけではない．

    //imshowだとMatサイズに一致するサイズが自動に開かれてしまうため
    //WINDOW_NORMALを指定するとリサイズ可能になる．値0
    //WINDOW_AUTOSIZEではMatのサイズ固定になる．値1
    cv::namedWindow("Inverse imager", CV_WINDOW_NORMAL);
    //resizeWindow関数でウィンドウの大きさ調整する
    cv::resizeWindow("Inverse imager", 800, 600);
    cv::Mat src, dst;

    while (1)
    {
        cap >> src;
        if (src.empty()) {
            break;
        }
        //-1:点対称　0:X軸対称　1:Y軸対称
        cv::flip(src, dst, 1);
        cv::imshow("source image", src);
        cv::imshow("Inverse imager", dst);

        if(cv::waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}