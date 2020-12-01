#include <opencv2/opencv.hpp>

//マウスイベント練習とミニチュアぼかし

void changeBrightness(int position, void* userdata) {
    double* br = (double*)(userdata);
    *br = (double)position /10.0;
}

void setRectangle(int event, int x, int y, int flags, void* userdata) {//x,y=イベント発生時のマウスの(x,y)座標
    int* pt = (int*)(userdata);
    switch (event) {
        case CV_EVENT_LBUTTONDOWN:
            if (0 < x && x < 640 && 0 < y && y < 480) {
                pt[0] = x, pt[1] = y;
            }
            break;
        case CV_EVENT_MOUSEMOVE:
            if (flags == CV_EVENT_FLAG_LBUTTON) {
                if (0 < x && x < 640 && 0 < y && y < 480 && x != pt[0] && y != pt[1]) {
                    pt[2] = x, pt[3] = y;
                }
            }
            break;
    }
}

int main () {

    cv::Mat src, dst;
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
    cv::namedWindow("dst video", CV_WINDOW_NORMAL);
    //resizeWindow関数でウィンドウの大きさ調整する
    cv::resizeWindow("dst video", 640, 480);

    //blur
    int sizeBlur = 12;
    cv::createTrackbar("Blur", "dst video", &sizeBlur, 30);

    //speed
    int speed = 5;
    cv::createTrackbar("Speed", "dst video", &speed, 50);

    //brightness
    double brightness = 2.0;
    cv::createTrackbar("Brightness", "dst video", NULL, 50, changeBrightness, &brightness);
    cv::setTrackbarPos("Brightness", "dst video", 20);//初期位置を設定

    //ROI
    int pt[] = {40, 100, 600, 320};//対角点を指定
    cv::setMouseCallback("dst video", setRectangle, pt);

    while (1)
    {
        cap >> src;
        if (src.empty()) {
            break;
        }
        cv::resize(src, src, cv::Size(640,480));
        src = src * brightness;

        cv::blur(src, dst, cv::Size(sizeBlur + 1, sizeBlur + 1));
        cv::Rect roi(cv::Point(pt[0], pt[1]),cv::Point(pt[2], pt[3]));
        src(roi).copyTo(dst(roi));//ROIのみをdstにコピー

        cv::imshow("dst video", dst);

        if(cv::waitKey(speed + 1) == 27) {
            break;
        }
    }
    return 0;
}