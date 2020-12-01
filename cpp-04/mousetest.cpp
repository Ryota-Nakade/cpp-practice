#include <opencv2/opencv.hpp>

#include <iostream>
using namespace std;
using namespace cv;

Rect2i rectangle_value;


void mouse_callback(int event, int x, int y, int flags, void *userdata)
{
    bool *isClick = static_cast<bool *>(userdata);
    if (event == EVENT_LBUTTONDOWN) {
        *isClick = true;
        cout << "Draw rectangle\n"
            << " start position (x, y) : " << x << ", " << y << endl;
        rectangle_value = Rect2i(x, y, 0, 0);
    }
    if (event == EVENT_LBUTTONUP) {
        *isClick = false;
        cout << " end   position (x, y) : " << x << ", " << y << endl;
        rectangle_value.width = x - rectangle_value.x;
        rectangle_value.height = y - rectangle_value.y;
    }
    if (event == EVENT_MOUSEMOVE) {
        if (*isClick) {
            rectangle_value.width = x - rectangle_value.x;
            rectangle_value.height = y - rectangle_value.y;
        }
    }
}

int main()
{
    Mat img = imread("../image/DSC_0221_10.jpg");
    Mat draw_img = img.clone();
    string window_name = "example";
    bool isClick = false;
    int key;

    imshow(window_name, img);
    setMouseCallback(window_name, mouse_callback, &isClick);
    for (;;) {
        key = 0;

        // 左ボタンが押されたら描画開始
        if (isClick == true) {
            rectangle(draw_img, rectangle_value, Scalar(255, 0, 0), 3, CV_AA);
        }

        imshow(window_name, draw_img);
        draw_img = img.clone();

        // qキーが押されたら終了
        key = waitKey(1);
        if (key == 'q')
            break;
    }

    return 0;
}