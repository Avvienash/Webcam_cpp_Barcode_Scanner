# Barcode Scanner Project

## Introduction

This GitHub repository contains the source code for a barcode scanner project developed by Avvienash Jaganathan and Ming Han Chiong. This barcode scanner is designed to capture and decode barcodes from a live camera feed using OpenCV. The decoded barcode is then analyzed to calculate a parity digit and display the decoded barcode information.

## Theory Behind Barcode Decoding

The barcode decoding method used in this project is based on analyzing the variations in the width of bars and spaces within a barcode. Here's an overview of the theory behind the method:

1. **Image Processing**:
   - The first step is to capture a video frame from a camera feed.
   - The frame is converted to grayscale to simplify the processing.
   - A thresholding technique is applied to create a binary image, where black pixels represent the barcode bars, and white pixels represent the spaces between bars.

2. **Finding the Centerline**:
   - The code identifies the centerline of the barcode by analyzing the middle row of pixels in the binary image. The centerline is crucial for decoding because it helps determine the width of bars and spaces accurately.

3. **Barcode Start Detection**:
   - The code looks for the starting point of the barcode content. This is typically done by scanning from left to right until the first black pixel (bar) is encountered after the white border.

4. **Finding Barcode Elements**:
   - Once the start of the barcode is detected, the code identifies specific patterns of bars and spaces that correspond to the elements of the barcode.
   - These patterns are identified by looking for transitions between bars and spaces (e.g., 1 0 1) and determining the width of each element (T1, T2, T3, T4).

5. **Decoding Elements**:
   - The code assigns values to the elements based on the calculated widths. For example, certain combinations of T1, T2, T3, and T4 widths correspond to specific digits.
   - Parity and modulo arrays are used to store the decoded values.

6. **Parity Digit Calculation**:
   - The code calculates a parity digit based on the decoded elements. The parity digit is used to check the integrity of the barcode data.

7. **Display and Output**:
   - The decoded barcode elements, parity digit, and other information are displayed or printed to the console.

8. **Reversing and Correcting** (Optional):
   - If the initial parity check fails (parity digit is incorrect), the code may attempt to reverse the barcode and perform decoding again. This is done because some barcodes may be printed in reverse order.

9. **Continuous Scanning**:
   - The code allows for continuous scanning of barcodes until the user presses a specific key (e.g., 'x') to exit the program.

The method described above is a simplified explanation of how the code decodes 1D barcodes. The actual decoding process can vary depending on the barcode symbology being used (e.g., Code 39, Code 128, UPC), and more complex barcodes may require additional decoding rules and error correction techniques. However, the fundamental concept of analyzing the widths of bars and spaces remains a common approach in 1D barcode decoding.

## Code Explanation

The barcode scanner code consists of a C++ program that utilizes the OpenCV library for capturing and processing video frames from a camera feed. Below is an explanation of the key components of the code:

- **Header Includes**: The code includes necessary header files for OpenCV and other standard libraries.

- **Global Variables**: Several global variables are defined, including arrays for parity and modulo values, an index variable, and the image width.

- **`barcode_scanner()` Function**: This function captures video from the camera, processes frames to convert them to grayscale and threshold them, and identifies the centerline of the barcode. It then displays the processed image in a window.

- **`remove_border_left()` Function**: This function is used to remove the left border of the barcode by finding the starting point of the barcode content.

- **`code_first()` Function**: This function identifies the starting point of the barcode content after the left border has been removed.

- **`T_value()` Function**: This function calculates the value of T1, T2, T3, and T4 based on specific patterns in the barcode. It updates the parity and modulo arrays accordingly.

- **`T()` and `Tr()` Functions**: These functions calculate the values of T1, T2, T3, and T4 for the left and right halves of the barcode.

- **`convert()` Function**: This function converts an integer array into a string.

- **`reverseArray()` Function**: This function reverses the elements of an integer array.

- **`decode()` Function**: This function initiates the decoding process. It removes the left border, calculates Ts, and decodes the barcode elements.

- **`main()` Function**: The `main()` function initializes the barcode scanner, captures video, decodes barcodes, calculates the parity digit, and displays the results. It allows continuous scanning until the user presses a specific key (e.g., 'x') to exit the program.

## Languages and Tools

<p align="left"> <a href="https://www.w3schools.com/cpp/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/> </a> 
  <a href="https://opencv.org/" target="_blank" rel="noreferrer"> <img src="https://www.vectorlogo.zone/logos/opencv/opencv-icon.svg" alt="opencv" width="40" height="40"/> </a> 
</p>

## Contributors

- Avvienash Jaganathan
- Ming Han Chiong

Please feel free to contribute to this project or report any issues you encounter. We appreciate your feedback and contributions.
