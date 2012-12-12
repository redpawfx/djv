#!/bin/csh

set file = $argv[1]
set build_path = $argv[2]

#echo $file
#echo $build_path

set libs = (`otool -L $file | egrep $build_path | awk '{print $1}'`)

#echo $libs

install_name_tool -id "@executable_path/../lib/"$file:t $file

foreach lib ($libs)
  install_name_tool -change $lib "@executable_path/../lib/"$lib:t $file
end
