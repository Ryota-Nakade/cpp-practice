#include <opencv2/opencv.hpp>

int main () {
    cv::Mat src = cv::imread("../image/DSC_0221_10.jpg");
    cv::Mat dst, dst2, dst3;

    if(src.empty()){
        std::cout << "Can't read image file!" << std::endl;
        std::cin.get();
        return -1;
    }

    //cv::Canny(src, dst, 20, 50);//src,dst,threshold,renzoku
    cv::stylization(src, dst2);
    cv::pencilSketch(src, dst, dst3);

    cv::imshow("Input image", src);
    cv::imshow("Output image", dst);
    cv::imshow("output2", dst2);
    cv::imshow("output3", dst3);

    std::vector<int> params{ cv::IMWRITE_JPEG_QUALITY, 50};
    cv::imwrite("image_dst.jpg", dst, params);//save as ...

    cv::waitKey();
    return 0;
}