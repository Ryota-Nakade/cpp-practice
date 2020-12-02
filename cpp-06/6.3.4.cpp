#include<iostream>
#include<cmath>
#include<opencv2/opencv.hpp>

//ガンマ変換をして表示するプログラム

int main(int argc, char **argv)
{
  cv::VideoCapture cap("../image/MOV_0202.mp4");

  cv::Mat src, dst;
  if (!cap.isOpened()) {
    std::cout << "error" << std::endl;
    return -1;
  }

  cv::namedWindow("src", cv::WINDOW_NORMAL);
  cv::namedWindow("dst", cv::WINDOW_NORMAL);
  cv::resizeWindow("src", 640, 360);
  cv::resizeWindow("dst", 640, 360);

  double gamma = 2.0;
  cv::Mat lut(256, 1, CV_8U);

  while (1) {
    cap >> src; 
    for (int i = 0; i < 256; i++) {
        lut.data[i] = (unsigned char)(255.0 * pow(i / 255.0, 1.0 / gamma));
    }

    LUT(src, lut, dst);

    cv::imshow("src", src);
    cv::imshow("dst", dst);

    if(cv::waitKey(30) == 27 ) {
        break;
    }
  }
  return 0;
}
