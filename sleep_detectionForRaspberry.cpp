#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#define PIN_NUM 27 // gpio 16

using namespace dlib;
using namespace std;
using namespace cv;

image_window win;
shape_predictor sp;
std::vector<cv::Point> righteye;
std::vector<cv::Point> lefteye;
char c;
cv::Point p;

double compute_EAR(std::vector<cv::Point> vec)
{

    double a = cv::norm(cv::Mat(vec[1]), cv::Mat(vec[5]));
    double b = cv::norm(cv::Mat(vec[2]), cv::Mat(vec[4]));
    double c = cv::norm(cv::Mat(vec[0]), cv::Mat(vec[3]));
    //compute EAR
    double ear = (a + b) / (2.0 * c);
    return ear;
}

int main()
{
    int frameCounter = 0;
    float threshold = 0.2;

    wiringPiSetup();
    softToneCreate(PIN_NUM);

    try {
        cv::VideoCapture cap(0);

        if (!cap.isOpened()) {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        cap.set(CAP_PROP_FRAME_WIDTH, 640); //use small resolution for fast processing
        cap.set(CAP_PROP_FRAME_HEIGHT, 480);
        cap.set(CAP_PROP_FPS, 30); //fps to 30

        // Load face detection and deserialize  face landmarks model.
        frontal_face_detector detector = get_frontal_face_detector();

        deserialize("../shape_predictor_68_face_landmarks.dat") >> sp;

        // Grab and process frames until the main window is closed by the user.
        while (1) {
            // Grab a frame
            cv::Mat temp;
            if (!cap.read(temp)) {
                break;
            }

            cv_image<bgr_pixel> cimg(temp);
            full_object_detection shape;
            
            // Detect faces
            std::vector<rectangle> faces = detector(cimg);
            
            // Find the pose of each face.
            if (faces.size() > 0) {

                shape = sp(cimg, faces[0]); //work only with 1 face

                for (int b = 36; b < 42; ++b) {
                    p.x = shape.part(b).x();
                    p.y = shape.part(b).y();
                    lefteye.push_back(p);
                }
                for (int b = 42; b < 48; ++b) {
                    p.x = shape.part(b).x();
                    p.y = shape.part(b).y();
                    righteye.push_back(p);
                }
                //Compute Eye aspect ration for eyes
                double right_ear = compute_EAR(righteye);
                double left_ear = compute_EAR(lefteye);
                
                //if the avarage eye aspect ratio of lef and right eye less than 0.2, the status is sleeping.
                if ((right_ear + left_ear) / 2 < threshold) {
                    frameCounter++;
                    if (frameCounter >= 27) {
                        cout << "SLEEP" << endl;
                        softToneWrite(PIN_NUM, 262);
                    }
                    else {
                        cout << "drowsing" << endl;
                        softToneWrite(PIN_NUM, 131);
                    }
                }
                else {
                    frameCounter = 0;
                    softToneWrite(PIN_NUM, 0);
                }
                righteye.clear();
                lefteye.clear();

                c = (char)waitKey(30);
                if (c == 27)
                    break;
            }
        }
    }
    catch (serialization_error& e) {
        cout << "Check the path to dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl
             << e.what() << endl;
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }
}
