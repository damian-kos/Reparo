#pragma once
#include <ZXing/BarcodeFormat.h>
#include <ZXing/MultiFormatWriter.h>
#include <ZXing/BitMatrix.h>
#include <opencv2/opencv.hpp>

namespace Barcode {
  // Convert ZXing BitMatrix to OpenCV Mat
  inline cv::Mat ConvertBitMatrixToImage(const ZXing::BitMatrix& bitMatrix) {
    int width = bitMatrix.width();
    int height = bitMatrix.height();

    // Create a new OpenCV image (8-bit, single channel)
    cv::Mat image(height, width, CV_8UC1);

    // Fill the image based on the BitMatrix
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        // BitMatrix uses 'true' for black pixels, but in OpenCV 255 is white
        // So we need to invert the values
        image.at<uchar>(y, x) = bitMatrix.get(x, y) ? 0 : 255;
      }
    }

    return image;
  }

  inline void SaveImage(const cv::Mat& image, const std::string& filename) {
    // Use OpenCV's imwrite function to save the image
    cv::imwrite(filename, image);
  }

  inline void CreateBarcode(const std::string& data,
    const std::string& outputPath,
    ZXing::BarcodeFormat format = ZXing::BarcodeFormat::Code128,
    int width = 400,
    int height = 200) {
    try {
      // Create MultiFormatWriter object
      ZXing::MultiFormatWriter writer(format);

      // Encode data with specified format and dimensions
      auto bit_matrix = writer.encode(data, width, height);

      // Convert BitMatrix to an OpenCV image
      cv::Mat image = ConvertBitMatrixToImage(bit_matrix);

      // Save the image
      SaveImage(image, outputPath);

      std::cout << "Barcode successfully created and saved to: " << outputPath << std::endl;
    }
    catch (const std::exception& e) {
      std::cerr << "Error creating barcode: " << e.what() << std::endl;
    }
  }
}