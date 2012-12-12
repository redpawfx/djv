#!/usr/sfw/bin/python

#-------------------------------------------------------------------------------
# prototype.py
#
# Utility for creating a Solaris package prototype file.
#
# Input: A directory tree with the files you want to package.
# Output: The generated prototype file printed to stdout.
#-------------------------------------------------------------------------------

import os, sys, stat

# Command line.

if len(sys.argv) != 2:
  print 'Usage: prototype.py (package root)'
  sys.exit(1)

# Walk the directory tree accumulating a list of items.

item_list = []

arg = os.path.join(sys.argv[1], '')
path_sys = [ '/', '/usr', '/usr/local', '/opt' ]
for root, dirs, files in os.walk(arg):

  # Directory item.
  
  path = root.replace(arg, '/', 1)
  if path in path_sys:
    continue
  item_list.append('d none ' + path + ' 0755 root bin')
  
  for i in files:

    file_name = os.path.join(path, i)
    file_name_real = os.path.join(root, i)
    stat_data = os.lstat(file_name_real)

    # Symbolic link item.
    
    if stat.S_ISLNK(stat_data.st_mode):
      item_list.append( \
        's none ' + file_name + '=' + os.readlink(file_name_real) + '')
        
    # Executable item.
    
    elif stat_data.st_mode & stat.S_IXOTH:
      item_list.append('f none ' + file_name + ' 0755 root bin')
    
    # Regular file item.

    else:
      item_list.append('f none ' + file_name + ' 0644 root bin')

item_list.sort()

# Print.

print 'i pkginfo'
for i in item_list:
  print i

