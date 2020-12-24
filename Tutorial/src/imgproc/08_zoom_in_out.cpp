
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>


const char* window_name = "Pyramids Demo";

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/tesla.jpg";

void zoom_in_out()
{
    std::cout << "\n Zoom In-Out demo \n "
        "------------------  \n"
        " * [i] -> Zoom in   \n"
        " * [o] -> Zoom out  \n"
        " * [ESC] -> Close program \n\n";

    
    // Loads an image
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    
    for (;;)
    {
        cv::imshow(window_name, src);
        char c = (char)cv::waitKey(0);
        if (c == 27)
        {
            break;
        }
        else if (c == 'i')
        {
            cv::pyrUp(src, src, cv::Size(src.cols * 2, src.rows * 2));
            printf("** Zoom In: Image x 2 \n");
        }
        else if (c == 'o')
        {
            cv::pyrDown(src, src, cv::Size(src.cols / 2, src.rows / 2));
            printf("** Zoom Out: Image / 2 \n");
        }
    }
}