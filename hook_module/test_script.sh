# !/bin/bash
sudo -i
cd /sys/kernel/debug/hooker_filter
/sys/kernel/debug/hooker_filter$ echo 223 > pid
/sys/kernel/debug/hooker_filter$ echo 222 > pid
/sys/kernel/debug/hooker_filter$ cat ./pid 