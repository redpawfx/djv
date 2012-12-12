#include "op_scale.h"

#include <djv_ui/choice.h>
#include <djv_ui/group_box.h>
#include <djv_ui/int_value.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>

namespace glsl {
namespace op_scale {

Op::Op()
{
  using namespace djv_ui;
  
  // Widgets.
  
  group_box::Box * size_group =
    new group_box::Box("Size");
  Int_Edit_Slider * width =
    new Int_Edit_Slider;
  Int_Edit_Slider * height =
    new Int_Edit_Slider;
  
  group_box::Box * type_group =
    new group_box::Box("Type");
  Choice * type =
    new Choice(label_type());
  
  group_box::Box * filter_default_group =
    new group_box::Box("Default Filter");
  Choice * filter_default_min =
    new Choice(label_filter_default());
  Choice * filter_default_mag =
    new Choice(label_filter_default());
  
  group_box::Box * filter_custom_group =
    new group_box::Box("Custom Filter");
  Choice * filter_custom_min =
    new Choice(label_filter_custom());
  Choice * filter_custom_mag =
    new Choice(label_filter_custom());
  
  // Layout.
  
  layout_item::group(List<Layout_Item *>() <<
    width <<
    height);
  
  Layout_V * layout = new Layout_V(this);
  layout->add(size_group);
  Layout_V * layout_v = new Layout_V(size_group);
  Layout_H * layout_h = new Layout_H(layout_v);
  layout_v->add(width);
  layout_v->add(height);
  
  layout->add(type_group);
  layout_v = new Layout_V(type_group);
  layout_v->add(type);
  
  layout->add(filter_default_group);
  layout_v = new Layout_V(filter_default_group);
  layout_h = new Layout_H(layout_v);
  layout_h->margin(0);
  layout_h->add(filter_default_min);
  layout_h->add(filter_default_mag);

  layout->add(filter_custom_group);
  layout_v = new Layout_V(filter_custom_group);
  layout_h = new Layout_H(layout_v);
  layout_h->margin(0);
  layout_h->add(filter_custom_min);
  layout_h->add(filter_custom_mag);
  
  layout->add_stretch();
  
  // Initialize.
  
  width->range(1, 2048);
  height->range(1, 2048);
  width->set(_value.size.x);
  height->set(_value.size.y);
  type->set(_value.type);
  filter_default_min->set(_value.default_min);
  filter_default_mag->set(_value.default_mag);
  filter_custom_min->set(_value.custom_min);
  filter_custom_mag->set(_value.custom_mag);
  
  // Callbacks.

  width->signal.set(this, width_callback);
  height->signal.set(this, height_callback);
  type->signal.set(this, type_callback);
  filter_default_min->signal.set(this, default_min_callback);
  filter_default_mag->signal.set(this, default_mag_callback);
  filter_custom_min->signal.set(this, custom_min_callback);
  filter_custom_mag->signal.set(this, custom_mag_callback);
}

void Op::width_callback(int in)
{
  signal.emit((_value.size.x = in));
}

void Op::height_callback(int in)
{
  signal.emit((_value.size.y = in));
}

void Op::type_callback(int in)
{
  signal.emit((_value.type = TYPE(in)));
}

void Op::default_min_callback(int in)
{
  signal.emit((_value.default_min = FILTER_DEFAULT(in)));
}

void Op::default_mag_callback(int in)
{
  signal.emit((_value.default_mag = FILTER_DEFAULT(in)));
}

void Op::custom_min_callback(int in)
{
  signal.emit((_value.custom_min = FILTER_CUSTOM(in)));
}

void Op::custom_mag_callback(int in)
{
  signal.emit((_value.custom_mag = FILTER_CUSTOM(in)));
}

}
}
