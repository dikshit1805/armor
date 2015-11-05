###############################################################################

# This script tests the dmidecode tool

#Usage:
# $ sh test_dmidecode.sh 

################################################################################
#!/bin/bash
    
#dmidecode
/usr/local/sbin/dmidecode -V
status=$?
if test $status -eq 0
then
    echo "command /usr/local/sbin/dmidecode -V passed"
else
    echo "command /usr/local/sbin/dmidecode -V failed"
    exit
fi

echo "dmidecode command reads the system DMI table to display hardware and BIOS information of the server"
echo "/usr/local/sbin/dmidecode -t 4"
/usr/local/sbin/dmidecode -t 4
status=$?
if test $status -eq 0
then
    echo "command /usr/local/sbin/dmidecode -t 4 passed"
else
    echo "command /usr/local/sbin/dmidecode -t 4 failed"
    exit
fi
exit

