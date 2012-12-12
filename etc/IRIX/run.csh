#!/bin/csh

idb.py $HOME/tmp > djv.idb
gendist -root $HOME/tmp -idb djv.idb -spec djv.spec -dist /tmp -verbose
cd /tmp
tar cvf djv-0.8.2_irix.tardist djv djv.idb djv.sw

