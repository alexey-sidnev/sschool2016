#include <opencv2/opencv.hpp>

#include <iostream>

using namespace std;
using namespace cv;

const int kEscapeKey = 27;
const char* kOptions =
    "{ i image        | ../data/space.jpg | image to process          }"
    "{ h ? help usage |                     | print help message        }";

const char* kAbout = "This is mouse interactions sample application.\n";

const int kSpeedLimit = 20;
const size_t kCollisionLimit = 30;
const int kDelay = 25;

struct EventData {
  Point pressed_position;
  vector<Point> points_position;
  vector<Point> points_direction;
  vector<int> points_collision;
};

void Process(const Mat& in, Mat& out, const vector<Point> &points) {
  out = in.clone();

  for (auto& point : points)
    circle(out, point, 2, Scalar(0,0,255), 2);
}

void MyMouseCallback(int event, int x, int y, int, void *data) {
  auto event_data = static_cast<EventData*>(data);

  if (event == EVENT_LBUTTONDOWN) {
    event_data->pressed_position.x = x;
    event_data->pressed_position.y = y;
  } else if (event == EVENT_LBUTTONUP) {
    int xSpeed = (x - event_data->pressed_position.x) / 4;
    if (xSpeed > kSpeedLimit) {
      xSpeed = kSpeedLimit;
    } else if (xSpeed < -kSpeedLimit) {
      xSpeed = -kSpeedLimit;
    }

    int ySpeed = (y - event_data->pressed_position.y) / 4;
    if (ySpeed > kSpeedLimit) {
      ySpeed = kSpeedLimit;
    } else if (ySpeed < -kSpeedLimit) {
      ySpeed = -kSpeedLimit;
    }

    event_data->points_position.emplace_back(x, y);
    event_data->points_direction.emplace_back(xSpeed, ySpeed);
    event_data->points_collision.emplace_back(kCollisionLimit);
  }
}

bool UpdatePointPosition(Point &position, Point &direction, const Size& frame_size) {
  bool has_collision = false;
  position.x += direction.x;
  position.y += direction.y;

  if (position.x < 0) {
    position.x = 0;
    direction.x = -direction.x;
    has_collision = true;
  }
  if (position.x >= frame_size.width) {
    position.x = frame_size.width - 1;
    direction.x = -direction.x;
    has_collision = true;
  }

  if (position.y < 0) {
    position.y = 0;
    direction.y = -direction.y;
    has_collision = true;
  }
  if (position.y >= frame_size.height) {
    position.y = frame_size.height - 1;
    direction.y = -direction.y;
    has_collision = true;
  }

  return has_collision;
}

int main(int argc, const char** argv) {
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  String image_path = parser.get<String>("image");

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

  Size image_size = Size(image.cols, image.rows);

  namedWindow(image_path, CV_WINDOW_AUTOSIZE);
  imshow(image_path, image);

  EventData data;
  setMouseCallback(image_path, MyMouseCallback, &data);

  Mat out_image;
  int key = 0;

  while (key != kEscapeKey) {
    Process(image, out_image, data.points_position);
    for (int i = 0; i < data.points_position.size(); ++i) {
      bool has_collision = UpdatePointPosition(data.points_position[i], data.points_direction[i], image_size);
      if (has_collision)
        --data.points_collision[i];
      if (data.points_collision[i] <= 0) {
        data.points_position.erase(data.points_position.begin() + i);
        data.points_direction.erase(data.points_direction.begin() + i);
        data.points_collision.erase(data.points_collision.begin() + i);
      }
    }
    imshow(image_path, out_image);

    key = waitKey(kDelay) & 0x00FF;
  }

  return 0;
}

