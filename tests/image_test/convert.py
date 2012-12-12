#!/usr/bin/python

import sys
import os

plugin_list = [ "cineon", "dpx", "exr", "iff", "jpeg", "ppm", "png", "sgi",
  "tga", "tiff" ]
#plugin_list = [ "iff" ]

ext_list = {
  "cineon" : "cin",
  "dpx" : "dpx",
  "exr" : "exr",
  "iff" : "iff",
  "jpeg" : "jpeg",
  "ppm" : "ppm",
  "png" : "png",
  "sgi" : "sgi",
  "tga" : "tga",
  "tiff" : "tiff"
}

image_list = [ "grid.ppm", "grid_grey.ppm", "test01.ppm" ]

option_list = {
  "default" : [
    [ "-pixel", [
      "l u8", "la u8", "rgb u8", "rgba u8",
      "rgb u10",
      "l u16", "la u16", "rgb u16", "rgba u16",
      "l f16", "la f16", "rgb f16", "rgba f16",
      "l f32", "la f32", "rgb f32", "rgba f32" ] ] ],
  
  "cineon" :
    [ ],
  "dpx" : [
    [ "-save dpx endian", [ "msb", "lsb", "auto" ] ] ],
  "exr" : [
    [ "-save exr compression", [ "none", "piz", "zip", "rle" ] ] ],
  "iff" : [
    [ "-save iff compression", [ "none", "rle" ] ] ],
  "jpeg" : [
    [ "-save jpeg quality", [ "100", "25" ] ] ],
  "ppm" : [
    [ "-save ppm file type", [ "auto", "u1" ] ],
    [ "-save ppm file mode", [ "ascii", "binary" ] ] ],
  "png" :
    [ ],
  "sgi" : [
    [ "-save sgi compression", [ "none", "rle" ] ] ],
  "tga" : [
    [ "-save tga compression", [ "none", "rle" ] ] ],
  "tiff" : [
    [ "-save tiff compression", [ "none", "rle", "lzw" ] ] ]
}

def cmdln_option_build(data, out, i, tmp):

  if len(data[i]) == 0:
    out.append(tmp)
    return

  for j in data[i][1]:
    cmdln_option_build(data, out, i + 1, tmp + " " + data[i][0] + " " + j)

for plugin in plugin_list:

  i = 1
  
  for image in image_list:
    
    tmp_option_list = []
    tmp_option_list.extend(option_list["default"])
    tmp_option_list.extend(option_list[plugin])
    tmp_option_list.append([])

    cmdln_option_list = []
    cmdln_option_build(tmp_option_list, cmdln_option_list, 0, "")
    
    for j in cmdln_option_list:
      
      cmdln = "djv_convert" + " " + image + " out." + str(i) + "." + \
        ext_list[plugin]
      print cmdln + j
      
      result = os.system(cmdln + j)
      if result != 0:
        sys.exit(1);

      i = i + 1

