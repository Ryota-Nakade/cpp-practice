#include <opencv2/opencv.hpp>

//周波数領域で表示・フィルタリングするプログラム

void onMouse(int event, int x, int y, int flags, void* userdata) {
    cv::Mat *mask = (cv::Mat*)(userdata);
    switch (event) {
    case CV_EVENT_MOUSEMOVE:
        std::cout << "case1" << std::endl;
        if((flags & CV_EVENT_FLAG_LBUTTON) ) {
            std::cout << "case1 if" << std::endl;
            cv::circle(*mask, cv::Point(x, y), 32, cv::Scalar(0.0), -1);
        }
        break;
    
    case CV_EVENT_RBUTTONDOWN:
        (*mask).setTo(1.0);
        break;
    }
}

void shiftImage(cv::Mat& img) {
    int cx = img.cols / 2, cy = img.rows / 2;
    cv::Mat q0(img, cv::Rect(0, 0, cx, cy)), q1(img, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(img, cv::Rect(0, cy, cx, cy)), q3(img, cv::Rect(cx, cy, cx, cy)), tmp;

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

int main () {
    cv::VideoCapture cap("../image/MOV_0202.mp4");

    if(!cap.isOpened()) {
        std::cout << "Could not open file" << std::endl;
        std::cin.get();
        return -1;
    }

    cv::Mat src, srcW, freqW, dstW;
    int oW = cv::getOptimalDFTSize((int)cap.get(CV_CAP_PROP_FRAME_WIDTH));
    int oH = cv::getOptimalDFTSize((int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    cv::Mat mask(cv::Size(oW, oH), CV_32FC2, cv::Scalar(1.0, 1.0));
    cv::namedWindow("source", CV_WINDOW_NORMAL);
    cv::resizeWindow("source", 640, 360);
    cv::namedWindow("dst Freq", CV_WINDOW_NORMAL);
    cv::resizeWindow("dst Freq", 640, 360);
    cv::namedWindow("IDFT image", CV_WINDOW_NORMAL);
    cv::resizeWindow("IDFT image", 640, 360);
    setMouseCallback("dst Freq", onMouse, &mask);

    while (1)
    {
        cap >> src;
        if(src.empty()) {
            break;
        }
        //DFT可能なサイズに変換
        cv::copyMakeBorder(src, src, 0, oH - src.rows, 0, oW - src.cols, CV_HAL_BORDER_CONSTANT, cv::Scalar::all(0));
        cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);//gray scaleに変換
        src.convertTo(src, CV_32FC1, 1.0 / 255.0);//浮動小数点に変換
        cv::imshow("source", src);
        std::vector<cv::Mat> chW { src, cv::Mat::zeros(src.size(), CV_32FC1)};//実部と虚部から成る2ch画像配列
        cv::merge(chW, srcW);

        //DFT
        cv::dft(srcW, freqW);
        shiftImage(freqW);//画像入れ替え関数呼び出し
        freqW = freqW.mul(mask);
        cv::split(freqW, chW);
        cv::imshow("dst Freq", chW[0] / 10);

        //IDFT
        shiftImage(freqW);
        cv::idft(freqW, dstW);
        cv::split(dstW, chW);
        cv::normalize(chW[0], chW[0], 0.0, 1.0, cv::NORM_MINMAX);
        cv::imshow("IDFT image", chW[0]);

        if(cv::waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}  