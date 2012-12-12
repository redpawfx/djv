#include "op_exposure.h"

#include <djv_ui/float_value.h>
#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>

namespace glsl {
namespace op_exposure {

Op::Op() :
  _init(false)
{
  using namespace djv_ui;
  
  // Widgets.
  
  group_box::Box * value_group = new group_box::Box("Value");
  Float_Edit_Slider * value = new Float_Edit_Slider(-10.0, 10.0);
  group_box::Box * defog_group = new group_box::Box("Defog");
  Float_Edit_Slider * defog = new Float_Edit_Slider(0.0, 0.01);
  group_box::Box * knee_group = new group_box::Box("Knee");
  Float_Edit_Slider * knee_low = new Float_Edit_Slider(-3.0, 3.0);
  Float_Edit_Slider * knee_high = new Float_Edit_Slider(3.5, 7.5);
  group_box::Box * gamma_group = new group_box::Box("Gamma");
  Float_Edit_Slider * gamma = new Float_Edit_Slider(0.1, 4.0);
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->add(value_group);
  Layout_V * layout_v = new Layout_V(value_group);
  layout_v->add(value);
  layout->add(defog_group);
  layout_v = new Layout_V(defog_group);
  layout_v->add(defog);
  layout->add(knee_group);
  layout_v = new Layout_V(knee_group);
  layout_v->add(knee_low);
  layout_v->add(knee_high);
  layout->add(gamma_group);
  layout_v = new Layout_V(gamma_group);
  layout_v->add(gamma);
  layout->add_stretch();
  
  // Initialize.
  
  value->set(_value.value);
  defog->set(_value.defog);
  knee_low->set(_value.knee_low);
  knee_high->set(_value.knee_high);
  gamma->set(_value.gamma);
  
  // Callbacks.
  
  value->signal.set(this, value_callback);
  defog->signal.set(this, defog_callback);
  knee_low->signal.set(this, knee_low_callback);
  knee_high->signal.set(this, knee_high_callback);
  gamma->signal.set(this, gamma_callback);
}

void Op::value_callback(double in)
{
  signal.emit((_value.value = in));
}

void Op::defog_callback(double in)
{
  signal.emit((_value.defog = in));
}

void Op::knee_low_callback(double in)
{
  signal.emit((_value.knee_low = in));
}

void Op::knee_high_callback(double in)
{
  signal.emit((_value.knee_high = in));
}

void Op::gamma_callback(double in)
{
  signal.emit((_value.gamma = in));
}

}
}
