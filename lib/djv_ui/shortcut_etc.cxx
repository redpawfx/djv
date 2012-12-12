/*------------------------------------------------------------------------------
 lib/djv_ui/shortcut_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "shortcut.h"

namespace djv_ui {
namespace shortcut {

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Shortcut & a, const Shortcut & b)
{
  return
    a.name == b.name &&
    a.value == b.value;
}

bool operator != (const Shortcut & a, const Shortcut & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// key()
//------------------------------------------------------------------------------

namespace {

enum _MOD
{
  _SHIFT,
  _ALT,
  _CTRL,
  _COMMAND,
  
  _MOD_SIZE
};

#if defined(DJV_WINDOWS)
#undef _UP // XXX
#endif

enum _KEY
{
  _SPACEBAR,
  _BACKSPACE,
  _ESCAPE,
  _INSERT,
  _DELETE,
  _HOME,
  _END,
  _PAGE_UP,
  _PAGE_DOWN,
  _LEFT,
  _RIGHT,
  _UP,
  _DOWN,
  _F1,
  _F2,
  _F3,
  _F4,
  _F5,
  _F6,
  _F7,
  _F8,
  _F9,
  _F10,
  _F11,
  _F12,
  _KEYPAD_1,
  _KEYPAD_2,
  _KEYPAD_3,
  _KEYPAD_4,
  _KEYPAD_5,
  _KEYPAD_6,
  _KEYPAD_7,
  _KEYPAD_8,
  _KEYPAD_9,
  
  _KEY_SIZE
};

int _mod(_MOD in)
{
  switch (in)
  {
    case _SHIFT: return FL_SHIFT;
    case _ALT: return FL_ALT;
    case _CTRL: return FL_CTRL;
    case _COMMAND: return FL_COMMAND;
  }
  return 0;
}

int _key(_KEY in)
{
  switch (in)
  {
    case _SPACEBAR: return ' ';
    case _BACKSPACE: return FL_BackSpace;
    case _ESCAPE: return FL_Escape;
    case _INSERT: return FL_Insert;
    case _DELETE: return FL_Delete;
    case _HOME: return FL_Home;
    case _END: return FL_End;
    case _PAGE_UP: return FL_Page_Up;
    case _PAGE_DOWN: return FL_Page_Down;
    case _LEFT: return FL_Left;
    case _RIGHT: return FL_Right;
    case _UP: return FL_Up;
    case _DOWN: return FL_Down;
    case _F1: return FL_F + 1;
    case _F2: return FL_F + 2;
    case _F3: return FL_F + 3;
    case _F4: return FL_F + 4;
    case _F5: return FL_F + 5;
    case _F6: return FL_F + 6;
    case _F7: return FL_F + 7;
    case _F8: return FL_F + 8;
    case _F9: return FL_F + 9;
    case _F10: return FL_F + 10;
    case _F11: return FL_F + 11;
    case _F12: return FL_F + 12;
    case _KEYPAD_1: return FL_KP + 1;
    case _KEYPAD_2: return FL_KP + 2;
    case _KEYPAD_3: return FL_KP + 3;
    case _KEYPAD_4: return FL_KP + 4;
    case _KEYPAD_5: return FL_KP + 5;
    case _KEYPAD_6: return FL_KP + 6;
    case _KEYPAD_7: return FL_KP + 7;
    case _KEYPAD_8: return FL_KP + 8;
    case _KEYPAD_9: return FL_KP + 9;
  }
  
  return 0;
}
     
String key(int in)
{
  _KEY tmp = _KEY(0);
  
  switch (in)
  {
    case ' ': tmp = _SPACEBAR; break;
    case FL_BackSpace: tmp = _BACKSPACE; break;
    case FL_Escape: tmp = _ESCAPE; break;
    case FL_Insert: tmp = _INSERT; break;
    case FL_Delete: tmp = _DELETE; break;
    case FL_Home: tmp = _HOME; break;
    case FL_End: tmp = _END; break;
    case FL_Page_Up: tmp = _PAGE_UP; break;
    case FL_Page_Down: tmp = _PAGE_DOWN; break;
    case FL_Left: tmp = _LEFT; break;
    case FL_Right: tmp = _RIGHT; break;
    case FL_Up: tmp = _UP; break;
    case FL_Down: tmp = _DOWN; break;
    case FL_F + 1: tmp = _F1; break;
    case FL_F + 2: tmp = _F2; break;
    case FL_F + 3: tmp = _F3; break;
    case FL_F + 4: tmp = _F4; break;
    case FL_F + 5: tmp = _F5; break;
    case FL_F + 6: tmp = _F6; break;
    case FL_F + 7: tmp = _F7; break;
    case FL_F + 8: tmp = _F8; break;
    case FL_F + 9: tmp = _F9; break;
    case FL_F + 10: tmp = _F10; break;
    case FL_F + 11: tmp = _F11; break;
    case FL_F + 12: tmp = _F12; break;
    case FL_KP + 1: tmp = _KEYPAD_1; break;
    case FL_KP + 2: tmp = _KEYPAD_2; break;
    case FL_KP + 3: tmp = _KEYPAD_3; break;
    case FL_KP + 4: tmp = _KEYPAD_4; break;
    case FL_KP + 5: tmp = _KEYPAD_5; break;
    case FL_KP + 6: tmp = _KEYPAD_6; break;
    case FL_KP + 7: tmp = _KEYPAD_7; break;
    case FL_KP + 8: tmp = _KEYPAD_8; break;
    case FL_KP + 9: tmp = _KEYPAD_9; break;
    default:
      return String(in, 1);
  }
  
  return string::serialize(tmp, label_key());
}

}

bool serialize(String * in, int * out)
{
  *out = 0;

  while (in->size())
  {
    _MOD mod = static_cast<_MOD>(0);
    
    if (string::serialize(in, &mod, label_mod()))
      *out |= _mod(mod);
    else
      break;
  }
  
  _KEY key = static_cast<_KEY>(0);
  
  if (string::serialize(in, &key, label_key()))
  {
    *out |= _key(key);
  }
  else
  {
    String tmp;
    string::serialize(in, &tmp);
    int c = tmp[0];
    *out |= c;
  }

  return true;
}

//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------

String serialize(int in)
{
  List<String> out;
  
  if (FL_SHIFT & in)
  {
    in -= FL_SHIFT;
    out += string::serialize(_SHIFT, label_mod());
  }
  if (FL_ALT & in)
  {
    in -= FL_ALT;
    out += string::serialize(_ALT, label_mod());
  }
  if (FL_CTRL & in)
  {
    in -= FL_CTRL;
    out += string::serialize(_CTRL, label_mod());
  }
  if (FL_COMMAND & in)
  {
    in -= FL_COMMAND;
    out += string::serialize(_COMMAND, label_mod());
  }
  
  out += key(in);
  
  return string::join(out, ' ');
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

String label(int in)
{
  List<String> out;
  
  if (FL_SHIFT & in)
  {
    in -= FL_SHIFT;
    out += string::serialize(_SHIFT, label_mod());
  }
  if (FL_ALT & in)
  {
    in -= FL_ALT;
    out += string::serialize(_ALT, label_mod());
  }
  if (FL_CTRL & in)
  {
    in -= FL_CTRL;
    out += string::serialize(_CTRL, label_mod());
  }
  if (FL_COMMAND & in)
  {
    in -= FL_COMMAND;
    out += string::serialize(_COMMAND, label_mod());
  }
  
  out += key((in >= 'a' && in <= 'z') ? (in - 32) : in);
  
  return string::join(out, '+');
}

const List<String> & label_key()
{
  static const List<String> data = List<String>() <<
    "Spacebar" <<
    "Backspace" <<
    "Escape" <<
    "Insert" <<
    "Delete" <<
    "Home" <<
    "End" <<
    "Page up" <<
    "Page down" <<
    "Left" <<
    "Right" <<
    "Up" <<
    "Down" <<
    "F1" <<
    "F2" <<
    "F3" <<
    "F4" <<
    "F5" <<
    "F6" <<
    "F7" <<
    "F8" <<
    "F9" <<
    "F10" <<
    "F11" <<
    "F12" <<
    "Keypad 1" <<
    "Keypad 2" <<
    "Keypad 3" <<
    "Keypad 4" <<
    "Keypad 5" <<
    "Keypad 6" <<
    "Keypad 7" <<
    "Keypad 8" <<
    "Keypad 9";
  
  DJV_ASSERT(data.size() == _KEY_SIZE);
  
  return data;
}

const List<String> & label_mod()
{
  static const List<String> data = List<String>() <<
    "Shift" <<
    "Alt" <<
    "Ctrl" <<
    "Command";
  
  DJV_ASSERT(data.size() == _MOD_SIZE);
  
  return data;
}

extern const String
  tooltip_button = "Click, then type shortcut";

extern const String
  prefs_title = "Shortcuts";

//------------------------------------------------------------------------------

}
}

