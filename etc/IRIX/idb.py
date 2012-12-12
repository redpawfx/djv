#!/usr/bin/python

#-------------------------------------------------------------------------------
# idb.py
#
# Utility for creating an IRIX package IDB file.
#
# Input: A directory tree with the files you want to package.
# Output: The generated IDB file printed to stdout.
#-------------------------------------------------------------------------------

import os, sys, stat

# Command line.

if len(sys.argv) != 2:
  print 'Usage: idb.py (package root)'
  sys.exit(1)

# Walk the directory tree accumulating a list of items.

item_list = []

arg = os.path.join(sys.argv[1], '')
path_sys = [ '', 'usr', 'usr/local', 'opt' ]
for root, dirs, files in os.walk(arg):
  
  path = root.replace(arg, '', 1)
  if path in path_sys:
    continue
  item_list.append(path)
  
  for i in files:
    item_list.append(os.path.join(path, i))

# Sort list for gendist.

item_list.sort()

# Print.

for i in item_list:

  file_name_real = os.path.join(arg, i)
  stat_data = os.lstat(file_name_real)

  # Directory item.

  if stat.S_ISDIR(stat_data.st_mode):
    print 'd 0755 root sys ' + i + ' ' + i + ' djv.sw.base'

  # Symbolic link item.
    
  elif stat.S_ISLNK(stat_data.st_mode):
    file_name_real = os.readlink(file_name_real)
    print 'l 0755 root sys ' + i + ' ' + i + ' djv.sw.base ' + \
      'symval(' + file_name_real + ')'
        
  # Executable item.
    
  elif stat_data.st_mode & stat.S_IXOTH:
    print 'f 0755 root sys ' + i + ' ' + i + ' djv.sw.base'
    
  # Regular file item.

  else:
    print 'f 0644 root sys ' + i + ' ' + i + ' djv.sw.base'

