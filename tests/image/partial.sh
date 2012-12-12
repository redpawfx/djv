#!/bin/sh

in=grid.ppm

for plugin in tga sgi ppm dpx cin exr tiff jpeg png; do
#for plugin in tga; do

  ~/build/djv/djv_convert $in tmp.$plugin
  if [ $? -ne 0 ]; then exit; fi

  i=1
  for size in 512 2048 16384; do
  
    dd if=tmp.$plugin of=out.$i.$plugin bs=1 count=$size
    if [ $? -ne 0 ]; then exit; fi
    
    i=`expr $i + 1`
  done
  
  rm tmp.$plugin  
done

