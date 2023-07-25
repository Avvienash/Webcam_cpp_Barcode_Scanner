////////////////////////////////////////////////////////////////

// Written by Avvienash Jaganathan and Ming Han Chiong
/////////////////////////////////////////////////////////////////
#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <sstream>  // for string streams
#include <string>  // for string
using namespace std;
using namespace cv;

int parity[12];
int modulo[12];
int index = -1;
int width = 640;

int* barcode_scanner()
{
    CvCapture* capture = 0;
    IplImage* processedImage = 0;



    // Initializes capturing video from camera
    capture = cvCaptureFromCAM(0);
    if (!capture) {
        fprintf(stderr, "Could not initialize capturing...\n");
        return 0;
    }


    // Creates window
    cvNamedWindow("Camera image", 1);
    const int width = 640;
    const int height = 480;
    static int centerline[width];

    // Camera image
    IplImage* frame = 0;


    do {
        // Grabs and returns a frame from camera
        frame = cvQueryFrame(capture);
        if (!frame) {
            break;
        }
        
        

        float min_pixel = 255;
        float max_pixel = 0;

        for (int y = 0; y < frame->height; y++) {
            for (int x = 0; x < frame->width; x++) {

                uchar* row = (uchar*)(frame->imageData + frame->widthStep * y);

                //STEP 1: Convert to grayscale
                int gray = (row[x * 3] + row[x * 3 + 1] + row[x * 3 + 2]) / 3;

                if (gray < min_pixel) {
                    min_pixel = gray; // Find min pixel
                }

                if (gray > max_pixel) {
                    max_pixel = gray; // Find max pixel
                }

            }
        }

        // calculate threshold
        float threshold = 0.5 * (min_pixel + max_pixel);




        for (int y = 0; y < frame->height; y++) {
            for (int x = 0; x < frame->width; x++) {

                uchar* row = (uchar*)(frame->imageData + frame->widthStep * y);


                int gray = (row[x * 3] + row[x * 3 + 1] + row[x * 3 + 2]) / 3;

                int bin = 255; // white

                if (gray < threshold) {
                    bin = 0; // black
                }

                // Replace 3 channel with grayscale pixel
                row[x * 3] = bin;
                row[x * 3 + 1] = bin;
                row[x * 3 + 2] = bin;

                if (y == height / 2) {
                    centerline[x] = bin / 255; // find the centerline & convert into binary
                    row[x * 3] = 255;
                    row[x * 3 + 1] = 0;
                    row[x * 3 + 2] = 0;
                }


            }
        }
        // Shows the resulting image in the window
        cvShowImage("Camera image", frame);
    } while ('q' != cvWaitKey(10));


    if (frame != nullptr) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {

                uchar* row = (uchar*)(frame->imageData + frame->widthStep * y);

                row[x * 3] = centerline[x] * 255;
                row[x * 3 + 1] = centerline[x] * 255;
                row[x * 3 + 2] = centerline[x] * 255;


            }

            // change the centerline value
            if (centerline[x] == 1)
            {
                centerline[x] = 0;
            }
            else if (centerline[x] == 0)
            {
                centerline[x] = 1;
            }
        }
        cvNamedWindow("Code", 1);
        cvShowImage("Code", frame);
    }

    cout << "Press 'q' quit" << endl;
    while ('q' != cvWaitKey(10)) {
    }


    // Releases the CvCapture structure
    cvReleaseCapture(&capture);
    // Destroys all the HighGUI windows
    cvDestroyAllWindows();


    return centerline;
}

int remove_border_left(int start,int* code)
{
    for (int i = start; i < width; i++) {
        if (*(code + i) == 1)
        {
            start = i;
            break;
        }
    }

    return start;
}


int code_first(int start,int *code)
{
    int pointer = start;
    int p1 = 0;
    int p2 = 0;
    int p3 = 0;
    for (int i = start; i < width; i++) {
        if ((p1 == 0) && (*(code+i) == 0))
        {
            p1 = 1;
            continue;
        }

        if ((p1 == 1) && (p2 == 0)&& (*(code + i) == 1))
        {
            p2 = 1;
            continue;
        }

        if ((p1 == 1) && (p2 == 1) && (p3 == 0) && (*(code + i) == 0))
        {
            p3 = 1;
            pointer = i;
            break;
        }
    }

    return pointer;

}


void T_value(int T1, int T2, int T3, int T4)
{
    index = index + 1;

    if ((T1 != 2) && (T1 != 3) && (T1 != 4) && (T1 != 5))
    {
        cout << "T1 Error" << endl;
        return;
    }

    if ((T2 != 2) && (T2 != 3) && (T2 != 4) && (T2 != 5))
    {
        cout << "T2 Error" << endl;
        return;
    }
    

    switch (T1) 
    {
        case 2:
            switch (T2) 
            {
                case 2:
                    cout << "E6" << endl;
                    parity[index] = 0;
                    modulo[index] = 6;
                    break;
                case 3:
                    cout << "O0" << endl;
                    parity[index] = 1;
                    modulo[index] = 0;
                    break;
                case 4:
                    cout << "E4" << endl;
                    parity[index] = 0;
                    modulo[index] = 4;
                    break;
                case 5:
                    cout << "O3" << endl;
                    parity[index] = 1;
                    modulo[index] = 3;
                    break;
            }
            break;
        case 3:
            switch (T2)
            {
                case 2:
                    cout << "O9" << endl;
                    parity[index] = 1;
                    modulo[index] = 9;
                    break;
                case 3:
                    switch (T4)
                    {
                        case 2:
                            cout << "E2" << endl;
                            parity[index] = 0;
                            modulo[index] = 2;
                            break;
                        case 3:
                            cout << "E8" << endl;
                            parity[index] = 0;
                            modulo[index] = 8;
                            break;
                    }
                    break;
                case 4:
                    switch (T4)
                    {
                        case 2:
                            cout << "O1" << endl;
                            parity[index] = 1;
                            modulo[index] = 1;
                            break;
                        case 1:
                            cout << "O7" << endl;
                            parity[index] = 1;
                            modulo[index] = 7;
                            break;
                    }
                    break;
                case 5:
                    cout << "E5" << endl;
                    parity[index] = 0;
                    modulo[index] = 5;
                    break;

            }
            break;
        case 4:
            switch (T2)
            {
                case 2:
                    cout << "E9" << endl;
                    parity[index] = 0;
                    modulo[index] = 9;
                    break;
                case 3:
                    switch (T4)
                    {
                        case 2:
                            cout << "O2" << endl;
                            parity[index] = 1;
                            modulo[index] = 2;
                            break;
                        case 1:
                            cout << "O8" << endl;
                            parity[index] = 1;
                            modulo[index] = 8;
                            break;
                    }
                    break;
                case 4:
                    switch (T4)
                    {
                        case 1:
                            cout << "E1" << endl;
                            parity[index] = 0;
                            modulo[index] = 1;
                            break;
                        case 2:
                            cout << "E7" << endl;
                            parity[index] = 0;
                            modulo[index] = 7;
                            break;
                    }
                    break;
                case 5:
                    cout << "O5" << endl;
                    parity[index] = 1;
                    modulo[index] = 5;
                    break;
            }
            break;
        case 5:
            switch (T2)
            {
                case 2:
                    cout << "O6" << endl;
                    parity[index] = 1;
                    modulo[index] = 6;
                    break;
                case 3:
                    cout << "E0" << endl;
                    parity[index] = 0;
                    modulo[index] = 0;
                    break;
                case 4:
                    cout << "04" << endl;
                    parity[index] = 1;
                    modulo[index] = 4;
                    break;
                case 5:
                    cout << "E3" << endl;
                    parity[index] = 0;
                    modulo[index] = 3;
                    break;
            }

    }
}

int T(int pointer, int* code)
{
    int T1_s = pointer;
    int T1_e = pointer;

    int T2_s = pointer;
    int T2_e = pointer;

    int T3_s = pointer;
    int T3_e = pointer;

    int T4_s = pointer;
    int T4_e = pointer;

    int p1 = 0;
    int p2 = 0;
    int p3 = 0;
    for (int i = T4_s; i < width; i++)
    {
        if ((p1 == 0) && (p2 == 0) && (p3 == 0))
        {
            if (*(code + i) == 1)
            {
                p1 = 1;
                T4_e = i;
                T2_s = i;
            }
        }

        if ((p1 == 1) && (p2 == 0) && (p3 == 0))
        {
            if (*(code + i) == 0)
            {
                p2 = 1;
                T1_s = i;
                T3_e = i;
            }
        }

        if ((p1 == 1) && (p2 == 1) && (p3 == 0))
        {
            if (*(code + i) == 1)
            {
                p3 = 1;
                T2_e = i;
            }
        }

        if ((p1 == 1) && (p2 == 1) && (p3 == 1))
        {
            if (*(code + i) == 0)
            {
                T1_e = i;
                break;
            }
        }
    }
    int T = T1_e - T4_s;
    int T1 = round(7 * (double(T1_e - T1_s)) / double(T));
    int T2 = round(7 * (double(T2_e - T2_s)) / double(T));
    int T3 = round(7 * (double(T3_e - T3_s)) / double(T));
    int T4 = round(7 * (double(T4_e - T4_s)) / double(T));


    cout << "T1:" << " ";
    cout << T1 << endl;
    cout << "T2:" << " ";
    cout << T2 << endl;
    cout << "T3:" << " ";
    cout << T3 << endl;
    cout << "T4:" << " ";
    cout << T4 << endl;


    T_value(T1, T2, T3, T4);

    //cout << T1_s << endl;
    //cout << T1_e << endl;
    //cout << T2_s << endl;
    //cout << T2_e << endl;
    //cout << T3_s << endl;
    //cout << T3_e << endl;
    //cout << T4_s << endl;
    //cout << T4_e << endl;

    for (int i = T4_s; i < T1_e; i++)
    {
        cout << *(code + i) << " ";
    }

    return T1_e;
}

int Tr(int pointer, int* code)
{
    int T1_s = pointer;
    int T1_e = pointer;

    int T2_s = pointer;
    int T2_e = pointer;

    int T3_s = pointer;
    int T3_e = pointer;

    int T4_s = pointer;
    int T4_e = pointer;

    int p1 = 0;
    int p2 = 0;
    int p3 = 0;
    for (int i = T1_s; i < width; i++)
    {
        if ((p1 == 0) && (p2 == 0) && (p3 == 0))
        {
            if (*(code + i) == 0)
            {
                p1 = 1;
                T2_s = i;
            }
        }

        if ((p1 == 1) && (p2 == 0) && (p3 == 0))
        {
            if (*(code + i) == 1)
            {
                p2 = 1;
                T1_e = i;
                T3_s = i;
            }
        }

        if ((p1 == 1) && (p2 == 1) && (p3 == 0))
        {
            if (*(code + i) == 0)
            {
                p3 = 1;
                T2_e = i;
                T4_s = i;
            }
        }

        if ((p1 == 1) && (p2 == 1) && (p3 == 1))
        {
            if (*(code + i) == 1)
            {
                T4_e = i;
                T3_e = i;
                break;
            }
        }
    }
    int T = T4_e - T1_s;
    int T1 = round(7 * (double (T1_e - T1_s)) / double(T));
    int T2 = round(7 * (double(T2_e - T2_s)) / double(T));
    int T3 = round(7 * (double(T3_e - T3_s)) / double(T));
    int T4 = round(7 * (double(T4_e - T4_s)) / double(T));


    cout << "T1:" << " ";
    cout << T1 << endl;
    cout << "T2:" << " ";
    cout << T2 << endl;
    cout << "T3:" << " ";
    cout << T3 << endl;
    cout << "T4:" << " ";
    cout << T4 << endl;

    
    T_value(T1, T2, T3, T4);

    //cout << T1_s << endl;
    //cout << T1_e << endl;
    //cout << T2_s << endl;
    //cout << T2_e << endl;
    //cout << T3_s << endl;
    //cout << T3_e << endl;
    //cout << T4_s << endl;
    //cout << T4_e << endl;

    return T4_e;
}

string convert(int arr[], int n) 
{
    string s;
    for (int i = 0; i < n; i++) {
        s += to_string(arr[i]);
    }
    return s;
}

void reverseArray(int* arr, int width) 
{
    int temp;
    for (int i = 0; i < width / 2; i++) 
    {
        temp = *(arr + i);
        *(arr + i) = *(arr + width - i - 1);
        *(arr + width - i - 1) = temp;
    }
}

void decode(int* code)
{
    cout << "Code" << endl;
    for (int i = 0; i < width; i++)
    {
        cout << *(code + i) << " ";
    }


    // remove left border
    int s = 0;
    int start;
    start = remove_border_left(s, code);

    //cout << "Code Start point: " << endl;
    //cout << start << endl;

    //for (int i = start; i < width; i++)
    //{
    //    cout << *(code + i) << " ";
    //}

    // Calculate the Ts
    int pointer;

    // Step 1: pass through the 1 0 1
    pointer = code_first(start, code);
    cout << pointer << endl;


    // Step 2: T for first element
    cout << "Element 1" << endl;
    pointer = T(pointer, code);

    cout << "Element 2" << endl;
    pointer = T(pointer, code);

    cout << "Element 3" << endl;
    pointer = T(pointer, code);

    cout << "Element 4" << endl;
    pointer = T(pointer, code);

    cout << "Element 5" << endl;
    pointer = T(pointer, code);

    cout << "Element 6" << endl;
    pointer = T(pointer, code);

    // middle bars
    pointer = remove_border_left(pointer, code);
    pointer = code_first(pointer, code);
    pointer = remove_border_left(pointer, code);

    //// Right Side
    cout << "Right Side" << endl;

    cout << "Element 1" << endl;
    pointer = Tr(pointer, code);

    cout << "Element 2" << endl;
    pointer = Tr(pointer, code);

    cout << "Element 3" << endl;
    pointer = Tr(pointer, code);

    cout << "Element 4" << endl;
    pointer = Tr(pointer, code);

    cout << "Element 5" << endl;
    pointer = Tr(pointer, code);

    cout << "Element 6" << endl;
    pointer = Tr(pointer, code);
}

int main()
{
    // Run Barcode Scanner
    do
    {
        int* code;
        code = barcode_scanner();

        decode(code);
        string parity_string = convert(parity, 6);

        if (parity_string == "000000") // neeed to flip
        {
            index = -1;
            cout << "Lets Flip the Code" << endl;

            reverseArray(code, width);
            decode(code);
            parity_string = convert(parity, 6);
        }

        cout << "Pairty" << endl;
        for (int i = 0; i < 6; i++)
        {
            cout << *(parity + i) << " ";
        }




        cout << endl << "Pairty Digit: " << endl;

        int m;
        if (parity_string == "111111")
        {
            cout << "0" << endl;
            m = 0;
        }
        else if (parity_string == "110100")
        {
            cout << "1" << endl;
            m = 1;
        }
        else if (parity_string == "110010")
        {
            cout << "2" << endl;
            m = 2;
        }
        else if (parity_string == "110001")
        {
            cout << "3" << endl;
            m = 3;
        }
        else if (parity_string == "101100")
        {
            cout << "4" << endl;
            m = 4;
        }
        else if (parity_string == "100110")
        {
            cout << "5" << endl;
            m = 5;
        }
        else if (parity_string == "100011")
        {
            cout << "6" << endl;
            m = 6;
        }
        else if (parity_string == "101010")
        {
            cout << "7" << endl;
            m = 7;
        }
        else if (parity_string == "101001")
        {
            cout << "8" << endl;
            m = 8;
        }
        else if (parity_string == "100101")
        {
            cout << "9" << endl;
            m = 9;
        }
        else
        {
            cout << "No valid Parity Digit" << endl;
            m = 0;
        }


        cout << endl << "Code" << endl;
        for (int i = 0; i < 12; i++)
        {
            cout << *(modulo + i) << " ";
        }
        cout << endl;

        for (int i = 0; i < 11; i++)
        {
            if (i % 2 == 0)
            {
                m = m + 3 * (modulo[i]);
            }
            else
            {
                m = m + modulo[i];
            }

        }
        cout << "m:" << endl;

        cout << m << endl;
        cout << (10 - (m % 10)) << endl;

        index = -1;
    
    } while ('x' != cvWaitKey(10));
    
    return 0;

}
