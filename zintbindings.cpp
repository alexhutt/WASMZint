#include <emscripten/bind.h>

#include <zint.h>

using namespace emscripten;

class ZintWrapper {
public:
  ZintWrapper(
    int symbology,
    std::string data,
    bool transparent,
    bool show_text,
    int option_1,
    int option_2,
    int option_3,
    int output_options
  )
    : barcode_info(ZBarcode_Create()),
    data(data)
  {
    barcode_info->symbology = symbology;
    if (transparent) {
      strcpy(barcode_info->bgcolour, "00000000"); // Enable Alpha Channel
    }
    barcode_info->show_hrt = show_text;
    barcode_info->option_1 = option_1;
    barcode_info->option_2 = option_2;
    barcode_info->option_3 = option_3;
    barcode_info->output_options = output_options;
  }

  int render(float height, float scale) {
    barcode_info->height = height;
    barcode_info->scale = scale;
    return ZBarcode_Encode_and_Buffer(barcode_info, reinterpret_cast<const unsigned char*>(data.c_str()), 0, 0);
  }

  int getWidth() const {
    return barcode_info->bitmap_width;
  }
  int getHeight() const {
    return barcode_info->bitmap_height;
  }

  val getBitmap() const {
    if (barcode_info->bitmap != nullptr) {
      const auto length = barcode_info->bitmap_width * barcode_info->bitmap_height * 3;
      return val(typed_memory_view(length, barcode_info->bitmap));
    }
    return val::undefined();
  }

  val getBitmapAlpha() const {
    if (barcode_info->alphamap != nullptr) {
      const auto length = barcode_info->bitmap_width * barcode_info->bitmap_height;
      return val(typed_memory_view(length, barcode_info->alphamap));
    }
    return val::undefined();
  }

private:
  zint_symbol* barcode_info;
  int type;
  std::string data;
};

// Binding code
EMSCRIPTEN_BINDINGS(my_class_example) {
  class_<ZintWrapper>("ZintWrapper")
    .constructor<int, std::string, bool, bool, int, int, int, int>()
    .function("render", &ZintWrapper::render)
    .function("getBitmap", &ZintWrapper::getBitmap)
    .function("getBitmapAlpha", &ZintWrapper::getBitmapAlpha)
    .property("width", &ZintWrapper::getWidth)
    .property("height", &ZintWrapper::getHeight)
    ;
}
