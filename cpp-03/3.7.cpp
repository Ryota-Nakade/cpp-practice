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

    cv::Size sizeRec (800, 600);
    //MP4で保存．コーデックはH.264
    //cv::Sizeは書き込みたい動画のサイズと一致させること
    cv::VideoWriter rec("3.7_video_dst.mp4", cv::VideoWriter::fourcc('H', '2', '6', '4'), 30, sizeRec);
    cv::Mat src, tmp, gaussian, moving;

    while (1)
    {
        cap >> src;

        cv::GaussianBlur(src, tmp, cv::Size(15, 15), 5.0, 5.0);
        cv::resize(tmp, gaussian, sizeRec);
        cv::blur(src, tmp, cv::Size(100, 5));
        cv::resize(tmp, moving, sizeRec);
        cv::resize(src, src, cv::Size(0,0), 2.0, 2.0, CV_INTER_CUBIC);

        cv::imshow("source image", src);
        cv::imshow("Gaussian image", gaussian);
        cv::imshow("moving image", moving);

        rec << gaussian;

        if (cv::waitKey(30) == 27) {//loop break with ESC (ASCII code 27) key
            std::cout << "Loop break" << std::endl;
            break;
        }
    }
    return 0;
}