#include <opencv2/opencv.hpp>

//BGRカラーの分解・表示プロhグラム

int main () {
    cv::VideoCapture cap("../image/MOV_0202.mp4");
    if (!cap.isOpened()) {
        std::cout << "Could not open video!" << std::endl;
        std::cin.get();
        return -1;
    }

    cv::Size frame((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    cv::Mat src;
    cv::Mat imgZero = cv::Mat::zeros(frame, CV_8UC1);//pixel値が全て0の1チャンネル画像

    std::vector<cv::Mat> dst(3);
    std::vector<std::string> colors{"Blue", "Green", "Red"};

    while (1)
    {
        cap >> src;

        if(src.empty()) {
            break;
        }

        std::vector<cv::Mat> ch(3);//分離用3チャンネル分
        cv::split(src, ch);//BGRに分解してchにいれる
        for (int i=0; i < ch.size(); i++) {
            std::vector<cv::Mat> temp {imgZero, imgZero, imgZero};
            temp[i] = ch[i];//1チャンネルのみの値を持つ3チャンネル画像の生成（他2チャンネルの値は0）
            merge(temp, dst[i]);//出力用カラー画像の合成
            cv::imshow(colors[i], dst[i]);
        }

        cv::Scalar ave = mean(src);//Scalar型 ave[i]にB,G,Rのピクセル値の平均が入る

        std::cout.precision(0);
        std::cout << std::fixed << "BGR average " << ave << "; ";
        cv::Point maxLoc;
        cv::minMaxLoc(ave, NULL, NULL, NULL, &maxLoc);
        std::cout << colors[maxLoc.y] + "vector is 1st" << std::endl;

        if(cv::waitKey(30)==27) {
            break;
        } 
    }
    return 0;
}