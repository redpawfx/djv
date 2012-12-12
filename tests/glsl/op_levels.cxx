#include "op_levels.h"

#include <djv_ui/float_value.h>
#include <djv_ui/group_box.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>

namespace glsl {
namespace op_levels {

Op::Op()
{
  using namespace djv_ui;
  
  // Widgets.
  
  group_box::Box * input_group = new group_box::Box("Input");
  Float_Edit_Slider * input0 = new Float_Edit_Slider(0.0, 1.0);
  Float_Edit_Slider * input1 = new Float_Edit_Slider(0.0, 1.0);
  group_box::Box * gamma_group = new group_box::Box("Gamma");
  Float_Edit_Slider * gamma = new Float_Edit_Slider(0.0, 4.0);
  group_box::Box * output_group = new group_box::Box("Output");
  Float_Edit_Slider * output0 = new Float_Edit_Slider(0.0, 1.0);
  Float_Edit_Slider * output1 = new Float_Edit_Slider(0.0, 1.0);
  group_box::Box * soft_clip_group = new group_box::Box("Soft Clip");
  Float_Edit_Slider * soft_clip = new Float_Edit_Slider(0.0, 1.0);
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->add(input_group);
  Layout_V * layout_v = new Layout_V(input_group);
  layout_v->add(input0);
  layout_v->add(input1);
  layout->add(gamma_group);
  layout_v = new Layout_V(gamma_group);
  layout_v->add(gamma);
  layout->add(output_group);
  layout_v = new Layout_V(output_group);
  layout_v->add(output0);
  layout_v->add(output1);
  layout->add(soft_clip_group);
  layout_v = new Layout_V(soft_clip_group);
  layout_v->add(soft_clip);
  layout->add_stretch();
  
  // Initialize.
  
  input0->set(_value.input0);
  input1->set(_value.input1);
  gamma->set(_value.gamma);
  output0->set(_value.output0);
  output1->set(_value.output1);
  soft_clip->set(_value.soft_clip);
  
  // Callbacks.
  
  input0->signal.set(this, input0_callback);
  input1->signal.set(this, input1_callback);
  gamma->signal.set(this, gamma_callback);
  output0->signal.set(this, output0_callback);
  output1->signal.set(this, output1_callback);
  soft_clip->signal.set(this, soft_clip_callback);
}

void Op::input0_callback(double in)
{
  signal.emit((_value.input0 = in));
}

void Op::input1_callback(double in)
{
  signal.emit((_value.input1 = in));
}

void Op::gamma_callback(double in)
{
  signal.emit((_value.gamma = in));
}

void Op::output0_callback(double in)
{
  signal.emit((_value.output0 = in));
}

void Op::output1_callback(double in)
{
  signal.emit((_value.output1 = in));
}

void Op::soft_clip_callback(double in)
{
  signal.emit((_value.soft_clip = in));
}

}
}
