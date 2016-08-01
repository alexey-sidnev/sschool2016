#include <opencv2/opencv.hpp>

#include <iostream>

using namespace std;
using namespace cv;

const int kEscapeKey = 27;
const char* kOptions =
    "{ i image        | ../data/kitten.jpg | image to process          }"
    "{ o output       |                     | save path                 }"
    "{ h ? help usage |                     | print help message        }";

const char* kAbout = "This is image processing sample application.\n";

Mat CorrectGamma(Mat& img, double gamma) {
    Mat lut_matrix(1, 256, CV_8UC1);
    uchar * ptr = lut_matrix.ptr();
    for (int i = 0; i < 256; i++)
        ptr[i] = pow((double)i / 255.0, gamma) * 255.0;

    Mat result;
    LUT(img, lut_matrix, result);

    return result;
}

Mat Inverse(Mat& img) {
    Mat lut_matrix(1, 256, CV_8UC1);
    uchar * ptr = lut_matrix.ptr();
    for (int i = 0; i < 256; i++)
        ptr[i] = 255 - i;

    Mat result;
    LUT(img, lut_matrix, result);

    return result;
}
Mat hist(Mat src) {
    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int bins = 256;
    int histSize[] = { bins };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { sranges };
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = { 0};

    calcHist(&src, 1, channels, Mat(), // do not use mask
        hist, 1, histSize, ranges,
        true, // the histogram is uniform
        false);
    double maxVal = 0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);

    int scale = 4;
    Mat histImg = Mat::zeros(256, bins*scale, CV_8UC1);

    for (int h = 0; h < bins; h++)
    {
        float binVal = hist.at<float>(h);
        int intensity = cvRound(binVal * 255 / maxVal);
        rectangle(histImg, Point(h*scale, 256),
            Point((h + 1)*scale - 1, 256-intensity ),
            Scalar::all(intensity),
            CV_FILLED);
    }

    return histImg;
}

int main(int argc, const char** argv) {
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  String image_path = parser.get<String>("image");
  bool has_output = parser.has("output");
  String output_path = parser.get<String>("output");

  if (!parser.check()) {
    parser.printErrors();
    return 0;
  }

  Mat image;
  image = imread(image_path, CV_LOAD_IMAGE_COLOR);

  if (!image.data) {
    cout << "No image data for: " << image_path << endl;
    return 0;
  }

  Mat out_image;
  
  cvtColor(image, image, CV_BGR2GRAY);
  out_image = image.clone();

  if (has_output)
    imwrite(output_path, out_image);

  namedWindow(image_path, CV_WINDOW_AUTOSIZE);
  moveWindow(image_path, 80, 0);
  namedWindow("Out image", CV_WINDOW_AUTOSIZE);
  moveWindow("Out image", 80 + image.cols, 0);

  
  
  //1
  /*
  threshold(out_image, out_image, 128, 255, THRESH_BINARY);
  //adaptiveThreshold(out_image, out_image, 255, ADAPTIVE_THRESH_MEAN_C,
  //    THRESH_BINARY, 7, 7);
  imshow(image_path, image);
  imshow("Out image", out_image);
  */

  //2
  /*
  //out_image = CorrectGamma(out_image, 0.1);
  out_image = Inverse(out_image);
  imshow(image_path, image);
  imshow("Out image", out_image);
  */
  //3
  /*
  image = CorrectGamma(image, 0.3);
  imshow(image_path, image);
  
  out_image = hist(image);
  imshow("Out image", out_image);

  Mat h, eq;
  equalizeHist(image, eq);
  namedWindow("equalize", CV_WINDOW_AUTOSIZE);
  imshow("equalize", eq);
  namedWindow("equalizeHist", CV_WINDOW_AUTOSIZE);
  h = hist(eq);
  imshow("equalizeHist", h);
  imwrite("qqq.jpg", eq);
  */

  //4
  /*
  Mat blur;
  medianBlur(image, blur, 3);
  //GaussianBlur(image, blur, Size(5,5), 2, 2);
  out_image = image - blur;
  medianBlur(out_image, out_image, 3);
  equalizeHist(out_image, out_image);
  imshow(image_path, image);
  imshow("Out image", out_image);
  */

  //4.2
  /*
  imshow(image_path, image);
  imshow("Out image", out_image);
  for (int i = 0; i < 20; ++i) {
      waitKey(0);
      boxFilter(out_image, out_image, -1, Size(3, 3));
      imshow("Out image", out_image);
  }
  */

  //4.3
  /*
  //Sobel(image, out_image, -1, 1, 1);
  //out_image = CorrectGamma(out_image, 0.4);
  //equalizeHist(out_image, out_image);
  Laplacian(image, out_image, -1);
  GaussianBlur(out_image, out_image, Size(3, 3), 2, 2);
  out_image = image - out_image;
  imshow(image_path, image);
  imshow("Out image", out_image);
  */
  //5
  /*
  erode(image, out_image, Mat());
  imshow(image_path, image);
  imshow("Out image", out_image);

  for (int i = 0; i < 20; ++i) {
      waitKey(0);
      erode(out_image, out_image, Mat());
      imshow("Out image", out_image);
  }
  */
  //6
  
  morphologyEx(image, out_image, MORPH_GRADIENT, Mat());
  imshow(image_path, image);
  imshow("Out image", out_image);
  
  waitKey(0);

  return 0;
}
