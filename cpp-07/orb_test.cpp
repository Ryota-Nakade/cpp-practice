#include <opencv2/opencv.hpp>

//ORB特徴を抽出して，動画に表示するだけのプログラム
//処理時間も表示する

//処理時間を計測する
void showFPS(cv::Mat &img, double period, float matchRate) {
    double fps = 1000.0 / period;
    //処理時間表示
    std::stringstream st;
    st.precision(3);//小数点以下を1桁に設定
    st << std::fixed << period << " ms / " << fps << " fps / " << matchRate << " matching";
    cv::putText(img, st.str(), cv::Point(16,64), CV_FONT_ITALIC, 1.0, cv::Scalar::all(255), 2);
}

int main(){

    // 画像ファイルを読み込む．
    // cv::VideoCapture cap("../image/MOV_0202.mp4");
    cv::VideoCapture cap(0);
    cv::Mat ref(cv::imread("../image/2020-12-01-170029.jpg"));
    //読み込みエラー処理
    if(!cap.isOpened()) {
        std::cout << "Could not open video!" << std::endl;
        std::cin.get();
        return -1;
    }
    //ウィンドウサイズ宣言＆リサイズ可能にする
    cv::namedWindow("ORB detector", CV_WINDOW_NORMAL);
    cv::resizeWindow("ORB detector", 640, 360);
    cv::namedWindow("dst", CV_WINDOW_NORMAL);
    cv::resizeWindow("dst", 640, 360);
    cv::Mat src;
    cv::Mat src_old, dst;

    //特徴点表示のときの四角の大きさ．中心からこの座標値離れた位置に四角の頂点が来る
    cv::Point2f rec {5.0, 5.0};

    //CPUのクロック数を取得
    double tickS = cv::getTickFrequency();

    while (1)
    {
        cap >> src;
        if(src.empty()) {
            break;
        }

        int64 startTick = cv::getTickCount();//処理開始時の時刻取得

        //ORB抽出器
        std::vector<cv::KeyPoint> keypoint1, keypoint2;//特徴点を格納する領域を用意
        cv::Mat descriptor1, descriptor2;
        cv::Ptr<cv::ORB> feature = cv::ORB::create(50, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);//ORBオブジェクト生成
        feature->detectAndCompute(src, cv::noArray(), keypoint1, descriptor1);
        feature->detectAndCompute(ref, cv::noArray(), keypoint2, descriptor2);

        // 特徴点マッチング
        std::vector<cv::DMatch> allMatch, goodMatch;
        cv::BFMatcher matcher(cv::NORM_HAMMING);
        matcher.match(descriptor1, descriptor2, allMatch);
        
        for (double i = 0; i < keypoint1.size(); i++) {
            cv::KeyPoint *point = &(keypoint1[i]);
            //cv::circle(src, itk->pt, 6, cv::Scalar(0,255,255), 1);//円で表示
            cv::rectangle(src, point->pt - rec, point->pt + rec, cv::Scalar(0, 255, 255), 1);//四角で表示
        }

        for (int i = 0; i < (int)allMatch.size(); i++) {
            if (allMatch[i].distance < 30) {
                goodMatch.push_back(allMatch[i]);
            }
        }

        //cv::drawMatches(src_old, orbkeypoints_old, src, orbkeypoints, goodMatch, dst, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        cv::drawMatches(src, keypoint1, ref, keypoint2, goodMatch, dst, cv::Scalar(255, 0, 0), cv::Scalar(0, 0, 255), std::vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        float matchRate = (float)goodMatch.size() / (float)keypoint2.size();//一致率
        double period = (cv::getTickCount() - startTick) * 1000.0 / tickS;//処理にかかった時間取得

        showFPS(src, period, matchRate);//処理時間表示関数
        cv::imshow("ORB detector",src);//画像表示
        cv::imshow("dst", dst);
        std::cout << "特徴点数:" << descriptor2.rows << " / 次元数:" << descriptor2.cols << " / matchs: " << matchRate << std::endl;

        if(cv::waitKey(1) == 27) {//ESCキーで終了
            break;
        }
    }
    return 0;
}

