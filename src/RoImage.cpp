#include "RoImage.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "RoBarcode.h"

struct TextMetrics {
  cv::Size size;
  cv::HersheyFonts font = cv::FONT_HERSHEY_DUPLEX;
  int baseline = 0;
};


namespace RoImage {
  int PutWrappedText(cv::Mat& image, const std::vector<std::string>& lines, int x, int y, const TextMetrics& metrics, const float& font_scale = 1) {

    int start_y = y;
    for (size_t i = 0; i < lines.size(); ++i) {
      const auto& line = lines[i];

      cv::putText(image, line, cv::Point(x, start_y), metrics.font, font_scale, cv::Scalar(), 1, cv::LINE_AA, false);

      // Increment start_y for the next line
      start_y += metrics.size.height + metrics.baseline;
    }
    return start_y;
  }

  std::vector<std::string> GetWrappedLines(const int& width, const std::string& text, TextMetrics& metrics) {

    // Initialize vectors to store both original lines and final wrapped lines
    std::vector<std::string> lines;         // Stores initial lines split by newline
    std::vector<std::string> lines_to_put;  // Stores final wrapped lines

    // Split the input text into lines based on newline characters (\n)
    std::istringstream text_stream(text);
    std::string segment;
    while (std::getline(text_stream, segment, '\n')) {
      lines.push_back(segment);

      // Handle empty lines (preserve paragraph breaks)
      if (segment.empty()) {
        lines_to_put.push_back("");
        continue;  // Skip processing for empty lines
      }

      // Check if the entire line fits within the width constraint
      // Subtract 300 pixels as margin space
      if (cv::getTextSize(segment, metrics.font, 1, 1, &metrics.baseline).width < width) {
        lines_to_put.push_back(segment);  // If it fits, add the whole line
        continue;                         // Move to next line
      }

      // If line is too long, process it word by word
      std::istringstream words(segment);    // Split line into words
      std::string word;
      std::string curr_line;               // Buffer for building current line

      // Process each word in the line
      while (words >> word) {
        // Create test line by adding the next word
        // If curr_line is empty, test_line is just the word
        // Otherwise, add a space between curr_line and the new word
        std::string test_line = curr_line.empty() ? word : curr_line + " " + word;

        // Check if adding the new word would make the line too long
        int text_width = cv::getTextSize(test_line, metrics.font, 1, 1, &metrics.baseline).width;

        if (text_width > width && !curr_line.empty()) {
          // If adding the word exceeds width and curr_line isn't empty,
          // save current line and start a new line with the word
          lines_to_put.push_back(curr_line);
          curr_line = word;
        }
        else {
          // Word fits on current line, so add it
          curr_line = test_line;
        }
      }

      // After processing all words, add any remaining text as the last line
      if (!curr_line.empty()) {
        lines_to_put.push_back(curr_line);
      }
    }
    return lines_to_put;
  }

  void Image() {
    if (ImGui::Button("Create Image")) {
      int height = 297 * 4;
      int width = 210 * 4;

      cv::Mat image(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

      TextMetrics metrics;

      std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco";
      metrics.size = cv::getTextSize(text, metrics.font, 1, 1, &metrics.baseline);
      std::vector<std::string> lines = GetWrappedLines(400, text, metrics);
      int y = PutWrappedText(image, lines, 400, 40, metrics);


      text = "orem ipsum dolor sit amet, consectetur adipiscing elit";
      metrics.size = cv::getTextSize(text, metrics.font, 0.5, 1, &metrics.baseline);
      lines = GetWrappedLines(50, text, metrics);
      PutWrappedText(image, lines, 40, y, metrics, 0.5);

      cv::imwrite("temp.jpg", image); // Save the image

    }
    if (ImGui::Button("Barcode")) {
      Barcode::CreateBarcode("2341234134", "barcode.jpg");
    }
  }
}