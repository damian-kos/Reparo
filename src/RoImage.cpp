#include "RoImage.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "RoBarcode.h"
#include "models/parts.h"
#include "models/customer.h"
#include "models/supplier.h"

struct TextMetrics {
  cv::Size size;
  cv::HersheyFonts font = cv::FONT_HERSHEY_SIMPLEX;
  int baseline = 0;
  float font_scale = 1; // Font scale factor
  int spacing = 8; // Space between lines
};


namespace RoImage {
  int PutWrappedText(cv::Mat& image, const std::vector<std::string>& lines, int x, int y, const TextMetrics& metrics, const float& font_scale = 1) {

    int start_y = y;
    for (size_t i = 0; i < lines.size(); ++i) {
      const auto& line = lines[i];

      cv::putText(image, line, cv::Point(x, start_y), metrics.font, metrics.font_scale, cv::Scalar(), 1, cv::LINE_AA, false);

      // Increment start_y for the next line
      start_y += metrics.size.height + metrics.baseline + metrics.spacing;
    }
    return start_y;
  }

  int PutWrappedText(cv::Mat& _image, const std::string& _line, int _x, int _y, const TextMetrics& metrics) {

    int start_y = _y;

    cv::putText(_image, _line, cv::Point(_x, _y), metrics.font, metrics.font_scale, cv::Scalar(), 1, cv::LINE_AA , false);

    start_y += metrics.size.height + metrics.baseline + metrics.spacing;
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
      if (cv::getTextSize(segment, metrics.font, metrics.font_scale, 1, &metrics.baseline).width < width) {
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
        int text_width = cv::getTextSize(test_line, metrics.font, metrics.font_scale, 1, &metrics.baseline).width;

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
      metrics.font_scale = 0.5f;

      std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco";
      metrics.size = cv::getTextSize(text, metrics.font, metrics.font_scale, 1, &metrics.baseline);
      std::vector<std::string> lines = GetWrappedLines(400, text, metrics);
      int y = PutWrappedText(image, lines, 400, 40, metrics);


      text = "orem ipsum dolor sit amet, consectetur adipiscing elit";
      metrics.size = cv::getTextSize(text, metrics.font, metrics.font_scale, 1, &metrics.baseline);
      lines = GetWrappedLines(50, text, metrics);
      PutWrappedText(image, lines, 40, y, metrics, 0.5);

      cv::imwrite("temp.jpg", image); // Save the image

    }
    if (ImGui::Button("Barcode")) {
      Barcode::CreateBarcode("2341234134", "barcode.jpg");
    }
    RepairImage();
    SaleInvoiceImage();
  }

  struct RepairTicketImgData {
    std::string label;
    std::string data;
  };

  void RepairImage() {
    std::vector<RepairTicketImgData> _data = {
      {"Repair number",       "2137"},
      {"Date",                "14/05/2025"},
      {"Contact",             "+44 7501 000 000"},
      {"Notes", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco"},
      {"Price", "21.37"},
      {"Terms & Conditions", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco"},
    };

    if (ImGui::Button("Create Repair")) {
      int _image_height = 297 * 4;
      int _image_width = 210 * 4;

      cv::Mat _image(_image_height, _image_width, CV_8UC3, cv::Scalar(255, 255, 255));

      TextMetrics _metrics;
      //_metrics.font_scale = 0.5f;

      int _current_y = 200;
      int _label_start_x = 40;
      int _label_width = 200;
      int _vertical_spacing = 10;
      int _margin = 30;
      int _data_start_x = _label_start_x + _label_width + _vertical_spacing;
      int _data_width = _image_width - _data_start_x - _margin;
      bool _is_first_entry = true;
      int _table_top_y = 0;
      int _table_bottom_y = 0;

      for (const auto& _entry : _data) {
        _metrics.size = cv::getTextSize(_entry.label, _metrics.font, _metrics.font_scale, 1, &_metrics.baseline);

        if (_is_first_entry) {
          TableTopBorder(_table_top_y, _current_y, _metrics);
          cv::line(_image, cv::Point(_margin, _table_top_y), cv::Point(_image_width - _margin, _table_top_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
          _is_first_entry = false;
        }

        std::vector<std::string> _wrapped_label_lines = GetWrappedLines(_label_width - _label_start_x, _entry.label, _metrics);
        int _label_end_y = PutWrappedText(_image, _wrapped_label_lines, _label_start_x, _current_y, _metrics);

        std::vector<std::string> _wrapped_data_lines = GetWrappedLines(_data_width, _entry.data, _metrics);
        int _data_end_y = PutWrappedText(_image, _wrapped_data_lines, _data_start_x, _current_y, _metrics);

        _current_y = std::max(_label_end_y, _data_end_y);

        _table_bottom_y = _current_y - (_metrics.size.height + (_metrics.spacing / 2) + _metrics.baseline);
        cv::line(_image, cv::Point(_margin, _table_bottom_y), cv::Point(_image_width - _margin, _table_bottom_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
      }

      // Draw vertical borders of the table
      cv::line(_image, cv::Point(_margin, _table_top_y), cv::Point(_margin, _table_bottom_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
      cv::line(_image, cv::Point(_data_start_x - 5, _table_top_y), cv::Point(_data_start_x - 5, _table_bottom_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
      cv::line(_image, cv::Point(_image_width - _margin, _table_top_y), cv::Point(_image_width - _margin, _table_bottom_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);

      cv::imwrite("repair_ticket.jpg", _image); // Save the image

    }
  }

  void TableTopBorder(int& _table_top_y, int _current_y, TextMetrics& _metrics) {
    _table_top_y = _current_y - (_metrics.size.height + (_metrics.spacing / 2));
  }

  struct SaleInvoiceImgData {
    std::string label;
    Part data;
  };

  struct FieldData {
    FieldData(std::string label, int base, double scale) : label(label), width(base* scale) {
      calc_width(base, scale);
    }
    std::string label;
    int width;

    void calc_width(int base, double scale) {
      width = base * scale;
    }
  };

  void SaleInvoiceImage() {
    std::string _invoice_number = "#INVOICE 2137";
    // insert invoice
  
    std::string _date = "Date: 14/05/2025";
    // insert date

    std::string _billing_to = "Billing to:";
    std::string _shipping_to = "Shipping to:";

    Customer _customer;    // insert customer
    _customer.id = 1;
    _customer.name = "John Doe John Doe John Doe John Doe John Doe John Doe John Doe John Doe";
    _customer.surname = "Smith";
    _customer.email = "mail@gmail.com";
    _customer.phone = "+44 7501 000 000";
    std::vector<std::string> _billing_address = {
      "123 Main St",
      "City",
      "State",
      "Country"
    };
    _customer.billing_addresses.SetLines(_billing_address);
    std::vector<std::string> _shipping_address = {
      "456 Elm St",
      "City",
      "State",
      "Country"
    };
    _customer.ship_addresses.SetLines(_shipping_address);


    Supplier _supplier;
    // this is going to be users data

    // Image details
    int _image_height = 297 * 4;
    int _image_width = 210 * 4;
    int _margin = 30;
    int _horizontal_spacing = 10;

    // Area for text wrapping in case if we have two groups of text one to the left and one to the right
    int _half_text_width = (_image_width - (_margin * 2)) / 2;

    if (ImGui::Button("Create Sale Invoice")) {

      cv::Mat _image(_image_height, _image_width, CV_8UC3, cv::Scalar(255, 255, 255));
      TextMetrics _metrics;
       
      // Ivoice Number
      _metrics.font_scale = 1.5f;
      _metrics.size = cv::getTextSize(_invoice_number, _metrics.font, _metrics.font_scale, 1, &_metrics.baseline);
      int _label_end_y = PutWrappedText(_image, _invoice_number, _margin, _margin + _metrics.size.height, _metrics);
      
      // Date
      _metrics.font_scale = 0.4f;
      _metrics.size = cv::getTextSize(_date, _metrics.font, _metrics.font_scale, 1, &_metrics.baseline);
      _label_end_y = PutWrappedText(_image, _date, _margin, _label_end_y, _metrics);
      
      // Spacing from the top of the document
      _label_end_y += 100;

      // Billing to & Shipping to label
      _metrics.font_scale = 0.5f;
      _metrics.size = cv::getTextSize(_billing_to, _metrics.font, _metrics.font_scale, 1, &_metrics.baseline);
      PutWrappedText(_image, _billing_to, _margin, _label_end_y, _metrics);
      _label_end_y = PutWrappedText(_image, _shipping_to, _half_text_width + _horizontal_spacing, _label_end_y, _metrics);

      // Temporary variable to store the y position after labels
      int _y_temp = _label_end_y;
      int _y_temp2;

      // Customer billing data
      std::vector<std::string> _lines_ = GetWrappedLines(_half_text_width, _customer.name, _metrics);
      _label_end_y = PutWrappedText(_image, _lines_, _margin, _label_end_y, _metrics);
      _label_end_y = PutWrappedText(_image, _customer.surname, _margin, _label_end_y, _metrics);
      _label_end_y = PutWrappedText(_image, _customer.phone, _margin, _label_end_y, _metrics);
      _label_end_y = PutWrappedText(_image, _customer.email, _margin, _label_end_y, _metrics);


      for (auto& _line : _customer.billing_addresses.Get().Lines()) {
        std::vector<std::string> _wrapped_data_lines = GetWrappedLines(_half_text_width, _line, _metrics);
        _label_end_y = PutWrappedText(_image, _line, _margin, _label_end_y, _metrics);
        _y_temp2 = _label_end_y;
      }

      // reset the y to the beginning of the billing & shipping lables
      _label_end_y = _y_temp;

      // Customer shipping data
      if (!_customer.billing_addresses.Equals(_customer.ship_addresses)) {
        std::vector<std::string> _lines = GetWrappedLines(_half_text_width + _horizontal_spacing, "Johny Bravo", _metrics);
        _label_end_y = PutWrappedText(_image, _lines, _half_text_width + _horizontal_spacing, _label_end_y, _metrics);
        _label_end_y = PutWrappedText(_image, _customer.surname, _half_text_width + _horizontal_spacing, _label_end_y, _metrics);
        _label_end_y = PutWrappedText(_image, _customer.phone, _half_text_width + _horizontal_spacing, _label_end_y, _metrics);
        _label_end_y = PutWrappedText(_image, _customer.email, _half_text_width + _horizontal_spacing, _label_end_y, _metrics);
        for (auto& _line : _customer.ship_addresses.Get().Lines()) {
          std::vector<std::string> _wrapped_data_lines = GetWrappedLines(_half_text_width + _horizontal_spacing, _line, _metrics);
          _label_end_y = PutWrappedText(_image, _line, _half_text_width + _horizontal_spacing, _label_end_y, _metrics);
        }
      }
      else {
        _label_end_y = PutWrappedText(_image, "Same as billing", _half_text_width + _horizontal_spacing, _label_end_y, _metrics);
      }

      // Determine new y based on what has been longer in lines billing or shipping
      _label_end_y = std::max(_label_end_y, _y_temp2) + 50;

      _metrics.font_scale = 0.35f;
      int _width = _image_width - (_margin * 2);

      std::vector<FieldData> _part_table_header = {
      {"Product Name", _width, 0.3}, 
      {"SKU", _width,  0.2},
      {"Quantity", _width, 0.1},
      {"Price ex. VAT", _width, 0.08},
      {"Price", _width, 0.08},
      {"VAT", _width, 0.08},
      {"Total ex. VAT", _width, 0.08},
      {"Total", _width, 0.08},
      };

      int _x_headers_start = _margin + _horizontal_spacing;
      int _table_top_y = 0;
      int _max_y_for_headers = 0;
      int _temp_y = 0;
      TableTopBorder(_table_top_y, _label_end_y, _metrics);
      cv::line(_image, cv::Point(_margin, _table_top_y), cv::Point(_image_width - _margin, _table_top_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
      for (auto& _header : _part_table_header) {
        _metrics.size = cv::getTextSize(_header.label, _metrics.font, _metrics.font_scale, 1, &_metrics.baseline);
        std::vector<std::string> _wrapped_label_lines = GetWrappedLines(_header.width, _header.label, _metrics);
        int _temp_y2 = PutWrappedText(_image, _wrapped_label_lines, _x_headers_start, _label_end_y, _metrics);
        if (_temp_y2 > _temp_y) {
          _temp_y = _temp_y2;
        }
        _x_headers_start += _header.width;
      }

      _label_end_y = _temp_y;
      _x_headers_start = 40;
      int _table_bottom_y = _label_end_y - (_metrics.size.height + (_metrics.spacing / 2) + _metrics.baseline);
      for (auto& _header : _part_table_header) {
        cv::line(_image, cv::Point(_x_headers_start - _horizontal_spacing , _table_top_y), cv::Point(_x_headers_start - _horizontal_spacing, _table_bottom_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
        _x_headers_start += _header.width;
      }
      cv::line(_image, cv::Point(_image_width - _margin, _table_top_y), cv::Point(_image_width - _margin, _table_bottom_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
      cv::line(_image, cv::Point(_margin, _table_bottom_y), cv::Point(_image_width - _margin, _table_bottom_y), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);

      // Part
      int _temp_part_y = _label_end_y;
      int _temp_part_y2 = 0;
      _x_headers_start = _margin + _horizontal_spacing;

      Part _part;
      _part.name = "iPhone 16 Screen Ultra Aftermarket";
      _part.own_sku = "SKU123456789";
      _part.quantity = 3;
      _part.sell_price = 100.0;
      _part.sell_price_ex_vat = 80;
      _part.vat = 20.0;
      float _total = _part.quantity * _part.sell_price;
      float _total_vat = _total * (_part.vat / 100);

      // continue adding later

      cv::imwrite("repair_ticket.jpg", _image); // Save the image

    }

  }

}