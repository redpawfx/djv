#!/bin/csh

# Clean-up temporary files.

foreach i ('*~' '*.bak' core '*.core' so_locations gmon.out '*.gmon')
  find . -name "$i" -type f | xargs rm -f
end

set dir = (bin doc etc icon lib plugin tests util)
find $dir -name ".*" -type f | xargs rm -f

find . -maxdepth 1 -name ".*" -type f | xargs rm -f

# Clean-up Windows build files.

foreach i (ncb suo ilk manifest DEF)
  foreach j (`find third_party -name "*.$i"`)
    rm -f $j
  end
end

foreach i (BuildLog.htm)
  foreach j (`find third_party -name $i`)
    rm -f $j
  end
end

# Clean-up after Windows file-system conversion.

foreach i ('*.csh' '*.sh' '*.pl' configure install-sh ltconfig)
  find . -name "$i" -type f | xargs chmod +x
end

