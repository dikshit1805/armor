###############################################################################

# This script tests the setpci tool

#Usage:
# $ sh test_setpci.sh 

################################################################################
#!/bin/bash
    
#setpci
setpci --version
status=$?
if test $status -eq 0
then
    echo "command setpci --version passed"
else
    echo "command setpci --version failed"
    exit
fi

echo " setpci -  configure PCI devices"
echo " setpci -s 0 device_id vendor_id - lists ID’s of devices in slot 0 in all buses"
setpci -s 0 device_id vendor_id
status=$?
if test $status -eq 0
then
    echo "command setpci -s 0 device_id vendor_id passed"
else
    echo "command setpci -s 0 device_id vendor_id failed"
exit
fi
exit
