#include <opencv2/opencv.hpp>

//マウスイベント練習と射影変換プログラム

int sw = 480, sh = 480, cr = 16;
int numNearPoint = -1;

cv::Mat src, dst;

std::vector<cv::Point2f> ptSrc {
    cv::Point2f(150, 100), cv::Point2f(250, 100), cv::Point2f(250, 200), cv::Point2f(150, 200)
};

std::vector<cv::Point2f> ptDst {
    cv::Point2f(0,0), cv::Point2f(sw, 0), cv::Point2f(sw, sh), cv::Point2f(0, sh)
};

//MouseCallback function
void onMouse(int event, int x, int y, int flags, void* usardata) {
    switch (event) {
    case CV_EVENT_LBUTTONDOWN: {
        double minDistance = cr;
        numNearPoint = -1;
        for (int i = 0; i < 4; i++) {
            double distance = cv::norm(cv::Point(x,y) - (cv::Point)ptSrc[i]);
            if (distance < minDistance) {
                numNearPoint = i;
                minDistance = distance;
            }
        }
        std::cout << numNearPoint << std::endl;
        break;
    }
    case CV_EVENT_MOUSEMOVE:
        std::cout << "ok case2" << std::endl;
        if ((flags & CV_EVENT_FLAG_LBUTTON ) && numNearPoint >= 0) {
            std::cout << "ok case2 if" << std::endl;
            ptSrc[numNearPoint] = cv::Point(x,y);
        }
        break;
    
    case CV_EVENT_RBUTTONDOWN:
        std::cout << "case3" << std::endl;
        ptSrc[0] = cv::Point2f(150, 100), ptSrc[1] = cv::Point2f(250, 100);
        ptSrc[2] = cv::Point2f(250, 200), ptSrc[3] = cv::Point2f(150, 200);
        break;
    }
}

int main() {
    cv::VideoCapture cap("../image/MOV_0202.mp4");
    //読み込みエラー時の処理
    if (!cap.isOpened()) {
        std::cout << "Could not open video file!!" << std::endl;
        std::cin.get();
        return -1;
    }

    //imshowだとMatサイズに一致するサイズが自動に開かれてしまうため
    //WINDOW_NORMALを指定するとリサイズ可能になる．値0
    //WINDOW_AUTOSIZEではMatのサイズ固定になる．値1
    cv::namedWindow("source video", CV_WINDOW_NORMAL);
    cv::namedWindow("dst video", CV_WINDOW_NORMAL);
    //resizeWindow関数でウィンドウの大きさ調整する
    cv::resizeWindow("source video", 640, 480);
    cv::resizeWindow("dst video", 640, 480);

    cv::setMouseCallback("source video", onMouse);

    while (1) {
        cap >> src;
        if(src.empty()) {
            break;
        }
        //射影変換行列の取得．入力4座標，出力4座標
        cv::Mat pM = getPerspectiveTransform(ptSrc, ptDst);
        //射影変換の実行．入力画像，出力画像，変換行列，出力サイズ，補間方法
        warpPerspective(src, dst, pM, cv::Size(sw, sh), CV_INTER_LINEAR);

        //ポリラインは整数でしか入力できないので，ここで変換
        std::vector<cv::Point> pt4{ ptSrc[0], ptSrc[1], ptSrc[2], ptSrc[3] };
        //上の座標pt4で指定された四角を描画
        polylines(src, pt4, true, cv::Scalar(255, 0, 0), 2);
        for (int i = 0; i < 4; i++) {
            circle(src, pt4[i], cr, cv::Scalar(0,0,0), 3);
        }
        cv::imshow("source video", src);
        cv::imshow("dst video", dst);
        if (cv::waitKey(30) == 27) {
            break;
        }
    }
    return 0;
}