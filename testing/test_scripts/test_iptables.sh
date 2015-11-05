###############################################################################

# This script tests the iptablest tool

#Usage:
# $ sh test_iptables.sh 

################################################################################
#!/bin/bash
    
#iptables
# Testing iptables
iptables -V
status=$?
if test $status -eq 0
then
    echo "command iptables -V passed"
else
    echo "command iptables -V failed"
    exit
fi

echo "iptables -L"
iptables -L
status=$?
if test $status -eq 0
then
    echo "command iptables -L passed"
else
    echo "command iptables -L failed"
    exit
fi
exit

