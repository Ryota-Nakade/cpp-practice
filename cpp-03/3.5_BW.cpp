#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap("../image/MOV_0202.mp4");
    //MP4で保存．コーデックはH.264
    //cv::Sizeは元動画のサイズと一致させること
    cv::VideoWriter rec("3.5_BW.mp4", cv::VideoWriter::fourcc('H', '2', '6', '4'), 30, cv::Size(1280, 720));

    //読み込みエラー時の処理
    if (!cap.isOpened()) {
        std::cout << "Could not open video file!!" << std::endl;
        std::cin.get();
        return -1;
    }

    cv::Mat src, gray, dst;
    int i=0;

    while (1)
    {
        /* code */
        //動画から入力
        cap >> src;
        if(src.empty()) {
            break;
        }
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);//2値化する都合上，1チャンネルにしないといけない
        cv::threshold(gray, gray, 128, 255, CV_THRESH_BINARY);//2値化．grayをgrayに上書きしている
        cv::cvtColor(gray, dst, cv::COLOR_GRAY2BGR);//書き込む時の都合上，3チャンネルに戻さないといけない
                
        //videoウィンドウで表示
        cv::imshow("video", dst);

        rec << dst;

        if (cv::waitKey(20) == 27) {
            //ASCII code27 == Esc key. when Esc was pushed, it will break.
            //waitKey(xx) <- xx is frame rate.
            std::cout << "broken" << std::endl;
            break;
        }
        i++;
        std::cout << "ok No " << i << std::endl;
    }
    return 0;
}