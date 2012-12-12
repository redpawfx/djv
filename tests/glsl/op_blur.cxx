#include "op_blur.h"

#include <djv_ui/choice.h>
#include <djv_ui/group_box.h>
#include <djv_ui/int_value.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>

namespace glsl {
namespace op_blur {

Op::Op()
{
  using namespace djv_ui;
  
  // Widgets.
  
  group_box::Box * radius_group =
    new group_box::Box("Radius");
  Int_Edit_Slider * radius =
    new Int_Edit_Slider(0, 127);
  
  group_box::Box * filter_group =
    new group_box::Box("Filter");
  Choice * filter =
    new Choice(label_filter());
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->add(radius_group);
  Layout_V * layout_v = new Layout_V(radius_group);
  layout_v->add(radius);
  layout->add(filter_group);
  layout_v = new Layout_V(filter_group);
  layout_v->add(filter);
  layout->add_stretch();
  
  // Initialize.
  
  radius->set(_value.radius);
  filter->set(_value.filter);
  
  // Callbacks.

  radius->signal.set(this, radius_callback);
  filter->signal.set(this, filter_callback);
}

void Op::radius_callback(int in)
{
  signal.emit((_value.radius = in));
}

void Op::filter_callback(int in)
{
  signal.emit((_value.filter = FILTER(in)));
}

}
}
