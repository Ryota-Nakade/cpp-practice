#include <opencv2/opencv.hpp>

void printMat(std::string str, cv::Mat &m) {
    std::cout << "Size(attr) : [" << m.cols << "." << m.rows << "]" << std::endl;
    std::cout << "Dim(attr)" << m.dims << std::endl;
    std::cout << "Size(attr)" << m.size() << std::endl;
    std::cout << "Total (method): " << m.total() << std::endl;
    std::cout << "Channels(attr)" << m.channels() << std::endl;
    std::cout << "Depth(attr)" << m.depth() << std::endl;
    std::cout << "Type (method): " << m.type() << std::endl;

    imshow("dst_image", m);

    cv::waitKey();
}

int main () {
    cv::Mat img1(480, 640, CV_8UC1);
    printMat("img1", img1);

    cv::Size s2(640, 480);
    cv::Mat img2(s2, CV_8UC1);
    printMat("img2", img2);

    cv::Scalar color(237, 149, 100);
    cv::Mat img3(cv::Size(400,400), CV_8UC3, color);
    printMat("img3", img3);

    cv::Mat img4(s2 * 2, CV_8UC3, cv::Scalar::all(128));
    printMat("img4", img4);

    cv::Mat img5 = cv::Mat::zeros(cv::Size(320, 150), CV_8UC3);
    printMat("img5", img5);

    cv::Mat img6;
    img6.create(cv::Size(300, 300), CV_32FC3);
    img6.setTo(cv::Scalar(0.0, 1.0, 0.0));
    printMat("img6", img6);

    cv::Mat img7 = img3.clone();
    printMat("img7", img7);

    cv::Mat img8;
    img3.copyTo(img8);
    printMat("img8", img8);

    return 0;
}