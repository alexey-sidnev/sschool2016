#include <opencv2/opencv.hpp>

#include <iostream>

using namespace std;
using namespace cv;

const char* kWindowName = "Video";
const int kEscapeKey = 27;
const char* kOptions =
    "{ v video        | ../data/test.mov  | video to process          }"
    "{ d delay        |         25        | delay (in ms) between frames }"
    "{ h ? help usage |                   | print help message        }";

const char* kAbout = "This is video playback sample application.\n";

void ProcessRect(const Mat& in, Mat& out, Rect blockRect) {
  Mat block, gray, blur, equalize, derivative, derivative_color;

  out = in.clone();
  block = out(blockRect);

  cvtColor(block, gray, COLOR_BGR2GRAY);
  medianBlur(gray, blur, 3);
  equalizeHist(blur, equalize);
  Sobel(equalize, derivative, -1, 1, 1, 3);
  cvtColor(derivative, derivative_color, COLOR_GRAY2BGR);

  derivative_color.copyTo(block);
}

void ProcessRound(const Mat& in, Mat& out, Rect blockRect) {
  Mat block, gray, blur, equalize, derivative, derivativeColor;

  out = in.clone();
  block = out(blockRect);

  Mat mask(block.rows, block.cols, CV_8U, Scalar(0));

  cvtColor(block, gray, COLOR_BGR2GRAY);
  medianBlur(gray, blur, 3);
  equalizeHist(blur, equalize);
  Sobel(equalize, derivative, -1, 1, 1, 3);
  cvtColor(derivative, derivativeColor, COLOR_GRAY2BGR);

  ellipse(mask, Point(block.cols / 2, block.rows / 2),
                Size(block.cols / 2, block.rows / 2), 0., 0., 360., Scalar(1), -1);

  derivativeColor.copyTo(block, mask);
}


void UpdateRectPosition(Rect &position, Point2i &direction, const Size& imSize) {
  position.x += direction.x;
  position.y += direction.y;

  if (position.x < 0) {
    position.x = 0;
    direction.x = -direction.x;
  }
  if (position.x + position.width >= imSize.width) {
    position.x = imSize.width - 1 - position.width;
    direction.x = -direction.x;
  }

  if (position.y < 0) {
    position.y = 0;
    direction.y = -direction.y;
  }
  if (position.y + position.height >= imSize.height) {
    position.y = imSize.height - 1 - position.height;
    direction.y = -direction.y;
  }
}

int main(int argc, const char** argv) {
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  String video_path = parser.get<String>("video");
  size_t delay = parser.get<size_t>("delay");

  if (!parser.check()) {
    parser.printErrors();
    return 0;
  }

  VideoCapture capture(video_path);
  if (!capture.isOpened()) {
    cout << "Failed to open video file '" << video_path << "'" << endl;
    return 0;
  }

  Size frame_size = Size(capture.get(CV_CAP_PROP_FRAME_WIDTH),
                        capture.get(CV_CAP_PROP_FRAME_HEIGHT));
  size_t fps = capture.get(CV_CAP_PROP_FPS);

  cout << "Video file: " << video_path << endl;
  cout << "Delay: " << delay << " ms" << endl;
  cout << "Frame size: " << frame_size << ", " << fps << " FPS" << endl;

  namedWindow(kWindowName, WINDOW_AUTOSIZE);
  moveWindow(kWindowName, 0, 0);

  Mat frame;
  const size_t block_size = 201;
  Rect block_position(100, 100, block_size, block_size + 50);
  Point2i direction(5, 15);
  int mode = 0;

  capture >> frame;

  while (!frame.empty()) {
    Mat out_image;
    switch (mode) {
      case 0:
        ProcessRect(frame, out_image, block_position);
        break;
    case 1:
      ProcessRound(frame, out_image, block_position);
      break;
    };
    UpdateRectPosition(block_position, direction, frame_size);
    imshow(kWindowName, out_image);

    int key = waitKey(delay) & 0x00FF;
    if (key == kEscapeKey)
      break;
    if (key == ' ')
        mode = (mode + 1) % 2;

    capture >> frame;
  }

  return 0;
}
