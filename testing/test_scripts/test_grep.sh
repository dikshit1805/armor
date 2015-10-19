###############################################################################

# This script tests the grep tool

#Usage:
# $ sh test_grep.sh 

################################################################################
#!/bin/bash
    
#grep
grep --version
status=$?
if test $status -eq 0
then
    echo "command grep --version passed"
else
    echo "command grep --version failed"
    exit
fi

echo " grep - search for a string in files"
tail /proc/misc | grep network_latency    
status=$?
if test $status -eq 0
then
    echo "command tail /proc/misc | grep network_latency passed"
else
    echo "command tail /proc/misc | grep network_latency failed"
exit
fi
exit


