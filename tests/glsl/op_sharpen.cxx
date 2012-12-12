#include "op_sharpen.h"

#include <djv_ui/float_value.h>
#include <djv_ui/group_box.h>
#include <djv_ui/layout_row.h>

namespace glsl {
namespace op_sharpen {

Op::Op() :
  _init(false)
{
  using namespace djv_ui;
  
  // Widgets.
  
  group_box::Box * value_group =
    new group_box::Box("Value");
  Float_Edit_Slider * value =
    new Float_Edit_Slider(0.0, 2.0);
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->add(value_group);
  Layout_V * layout_v = new Layout_V(value_group);
  layout_v->add(value);
  layout->add_stretch();
  
  // Initialize.
  
  value->set(_value.value);
  
  // Callbacks.

  value->signal.set(this, value_callback);
}

void Op::value_callback(double in)
{
  signal.emit((_value.value = in));
}

}
}
