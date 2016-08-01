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
  capture >> frame;

  while (!frame.empty()) {
    imshow(kWindowName, frame);

    int key = waitKey(delay) & 0x00FF;
    if (key == kEscapeKey)
      break;

    capture >> frame;
  }

  return 0;
}
