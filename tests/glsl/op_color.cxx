#include "op_color.h"

#include <djv_ui/float_value.h>
#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>

namespace glsl {
namespace op_color {

Op::Op() :
  _init(false)
{
  using namespace djv_ui;
  
  // Widgets.
  
  group_box::Box * brightness_group = new group_box::Box("Brightness");
  Float_Edit_Slider * brightness = new Float_Edit_Slider(0.0, 5.0);
  group_box::Box * contrast_group = new group_box::Box("Contrast");
  Float_Edit_Slider * contrast = new Float_Edit_Slider(0.0, 5.0);
  group_box::Box * saturation_group = new group_box::Box("Saturation");
  Float_Edit_Slider * saturation = new Float_Edit_Slider(0.0, 5.0);
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->add(brightness_group);
  Layout_V * layout_v = new Layout_V(brightness_group);
  layout_v->add(brightness);
  layout->add(contrast_group);
  layout_v = new Layout_V(contrast_group);
  layout_v->add(contrast);
  layout->add(saturation_group);
  layout_v = new Layout_V(saturation_group);
  layout_v->add(saturation);
  layout->add_stretch();
  
  // Initialize.
  
  brightness->set(_value.brightness);
  contrast->set(_value.contrast);
  saturation->set(_value.saturation);
  
  // Callbacks.
  
  brightness->signal.set(this, brightness_callback);
  contrast->signal.set(this, contrast_callback);
  saturation->signal.set(this, saturation_callback);
}

void Op::brightness_callback(double in)
{
  signal.emit((_value.brightness = in));
}

void Op::contrast_callback(double in)
{
  signal.emit((_value.contrast = in));
}

void Op::saturation_callback(double in)
{
  signal.emit((_value.saturation = in));
}

}
}
