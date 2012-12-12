/*------------------------------------------------------------------------------
 lib/djv_ui/frame_def.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_FRAME_DEF_H
#define DJV_UI_FRAME_DEF_H

namespace djv_ui {
namespace frame {

class Frame;
class Frame_Group_Base;
class Frame_Group_Horizontal;
class Frame_Group_Vertical;

}

typedef frame::Frame Frame;
typedef frame::Frame_Group_Horizontal Frame_Group_H;
typedef frame::Frame_Group_Vertical Frame_Group_V;

}

#endif

