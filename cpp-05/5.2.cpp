#include <opencv2/opencv.hpp>

//HSV画像とポスタリゼーションプログラム

int main() {
    cv::VideoCapture cap("../image/MOV_0202.mp4");
    if(!cap.isOpened()){
        std::cout << "could not open video" << std::endl;
        std::cin.get();
        return -1;
    }

    cv::namedWindow("Posterization", CV_WINDOW_NORMAL);
    cv::resizeWindow("Posterization", 640, 360);
    cv::Mat src, hsv, poster;

    //色数のリストを以下で指定．180を割り切るために180の約数にする
    // std::vector<int> lColor = {180, 1, 2, 3, 4, 5, 6, 9, 10, 12, 15, 18, 20, 30, 36, 45, 60, 90};
    std::vector<int> lColor = {2};

    bool loop = true;
    while (loop)
    {
        cap >> src;
        if(src.empty()) {
            break;
        }

        cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);//BGRからHSVに変換

        for (int c =0; c < lColor.size(); c++) {
            std::vector<cv::Mat> ch(3);
            cv::split(hsv, ch);//H,S,Vの3チャンネルに分解
            int nColor = lColor[c];
            int colorRange = 180 / nColor;
            ch[0] /= colorRange;//Hを色数で割って，その整数部分のみに，
            ch[0] *= colorRange;//色数をかけると，代表値に置き換えられる
            cv::merge (ch, poster);
            cv::cvtColor(poster, poster, CV_HSV2BGR);//HSVからBGRに戻す

            //画面上の文字表示
            std::stringstream ss;
            ss << "C= " << nColor;
            cv::putText(poster, ss.str(), cv::Point(20, 30), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar::all(0));
            cv::imshow("Posterization", poster);//画像表示

            if(cv::waitKey(30) == 27) {
                loop = false;
                break;
            }
        } 
    }
    return 0;
}