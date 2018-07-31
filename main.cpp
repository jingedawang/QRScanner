#include <iostream>
#include <zbar.h>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;
using namespace zbar;

struct Result {
    string resultStr;
    vector<Point2i> location;
};

ostream& operator<< (ostream& o, Result& result)
{
    o << "data: " << result.resultStr << endl;
    o << "position: ";
    for (auto point : result.location)
    {
        o << point << " ";
    }
    o << endl;
}

//zbar接口
vector<Result> ZbarDecoder(Mat img)
{
    string result;
    ImageScanner scanner;
    const void *raw = (&img)->data;
    // configure the reader
    scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
    // wrap image data
    Image image(img.cols, img.rows, "Y800", raw, img.cols * img.rows);
    // scan the image for barcodes
    int n = scanner.scan(image);
    vector<Result> results;
    for (auto it = scanner.get_results().symbol_begin(); it != scanner.get_results().symbol_end(); it.operator++())
    {
        Result result;
        string data = (*it).get_data();
        result.resultStr = data;
        for (int i=0; i < (*it).get_location_size(); i++)
        {
            Point2i point;
            point.x = (*it).get_location_x(i);
            point.y = (*it).get_location_y(i);
            result.location.push_back(point);
        }
        results.push_back(result);
    }
    return results;
}

// 对二值图像进行识别，如果失败则开运算进行二次识别
vector<Result> GetQRInBinImg(Mat binImg)
{
    vector<Result> results = ZbarDecoder(binImg);
    if(results.empty())
    {
        Mat openImg;
        Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
        morphologyEx(binImg, openImg, MORPH_OPEN, element);
        results = ZbarDecoder(openImg);
    }
    return results;
}

// 用户调用接口
vector<Result> GetQR(Mat img)
{
    Mat binImg;
    //在otsu二值结果的基础上，不断增加阈值，用于识别模糊图像
    double thre = threshold(img, binImg, 0, 255, cv::THRESH_OTSU);
    vector<Result> results;
    while(results.empty() && thre<255)
    {
        threshold(img, binImg, thre, 255, cv::THRESH_BINARY);
        results = GetQRInBinImg(binImg);
        thre += 20;//阈值步长设为20，步长越大，识别率越低，速度越快
    }
    return results;
}

int main()
{
    // Input image.
    Mat img = imread("/home/wang/图片/QRCode7.jpg");
    Mat imgGray;

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    // Input image should firstly convert to gray image.
    // If your input is not a BGR 3-channel image, change the 3rd parameter to an appropriate one.
    cvtColor(img, imgGray, CV_BGR2GRAY);
    // Get QR result.
    vector<Result> results = GetQR(imgGray);
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();
    // Show time expense.
    cout << "Time used: " << ttrack << endl;

    // Print results.
    for (int i=0; i<results.size(); i++)
    {
        cout << results[i] << endl;
    }

    // Show original image.
    imshow("QRCode", img);
    waitKey(0);
}