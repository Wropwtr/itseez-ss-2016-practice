#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
"{ @image         | <none> | image to process            }"
"{ gray           |        | convert ROI to gray scale   }"
"{ median         |        | apply median filter for ROI }"
"{ edges          |        | detect edges in ROI         }"
"{ pix            |        | pixelize ROI                }";

struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};

static void OnMouse(int event, int x, int y, int, void *m)
{
	MouseCallbackState MState = (MouseCallbackState*) m;
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		MState.is_selection_started = true;
		MState.is_selection_finished = false;
		MState.point_first = Point(x, y);
		break;
	case EVENT_LBUTTONUP:
		MState.is_selection_started = false;
		MState.is_selection_finished = true;
		MState.point_second = Point(x, y);
		break;
	case EVENT_MOUSEMOVE:
		if (!MState.is_selection_finished) {
			MState.point_second = Point(x, y);
		}
		break;
	}
}


int main(int argc, const char** argv) {
	// Parse command line arguments.
	CommandLineParser parser(argc, argv, kOptions);
	parser.about(kAbout);

	MouseCallbackState MState;

	Mat src = imread(parser.get<std::string>(0), 1);
	namedWindow("Image", WINDOW_NORMAL);
	resizeWindow("Image", 640, 480);
	imshow("Image", src);

	setMouseCallback("Image", OnMouse, (void*)MState);
	Rect roi = Rect(MState.point_first, MState.point_second);
	rectangle(src, roi, 50);
	waitKey();
	// wait for event

	ImageProcessorImpl ip;
	if (parser.has("gray"))
		Mat dst = ip.CvtColor(src, roi);

	if (parser.has("median"))
		Mat dst = ip.Filter(src, roi, 1);

	if (parser.has("pix"))
		Mat dst = ip.DetectEdges(src, roi, 1, 10, 5, 3);

	//
	
	return 0;
}
