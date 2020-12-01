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

    cv::Mat src;

    //ビデオ属性の取得
    int frameWidth = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frameHeight = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    int frameCount =(int)cap.get(CV_CAP_PROP_FRAME_COUNT);
    int fps = (int)cap.get(CV_CAP_PROP_FPS);
    int wait = 1000/fps;

    std::cout << "(w, h) = (" << frameWidth << "," << frameHeight << ")" << std::endl;
    std::cout << "fps = " << fps << ", wait = " << wait << "ms" << std::endl;
    std::cout << "Length of video = " << frameCount << "frame. ( " << frameCount/fps << " sec )" << std::endl;

    bool loop = true;

    while (loop)
    {
        int pos = rand() % frameCount;//再生スタートの位置．0〜frameCountの間の乱数が得られる
        int window = rand() % 30 + 10;//再生するフレーム数．10〜39のフレーム数を得る
        int direction = 2* (rand() % 2) - 1;//再生方向．0,1を得た後，2内して1を引くと1，-1が得られる．

        std::cout << "START from " << pos << ", Number of frames : " << window << ", direction : " << direction << std::endl;

        for (int f=0; f<window; f++) {
            cap.set(CV_CAP_PROP_POS_FRAMES, pos);//フレーム番号の指定
            cap >> src;//srcに流し込む
            cv::imshow("シャッフル", src);
            pos += direction;
            //存在しないフレーム番号を参照しないように以下で設定
            pos = std::min(pos, frameCount-1);
            pos = std::max(pos,0);

            if(cv::waitKey(wait) == 27) {
                loop = false;
                break;
            }
        }
    }
    cap.release();
    cv::destroyWindow("シャッフル");

    std::cout << "enter key to close" << std::endl;
    std::cin.get();
    
    return 0;
}