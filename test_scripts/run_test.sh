#!/usr/bin/env bash


echo ""
echo "running test: \"`basename ${1}`\""
ret_val=0
if [ "${2}" == "quietly" ]
then
    `dirname ${0}`/../programs/${1} > /dev/null 2>&1
    ret_val=$?
else
    echo ""
    `dirname ${0}`/../programs/${1}
    ret_val=$?
fi
if [ "${2}" != "quietly" ]
then
    echo ""
fi
if [ ${ret_val} -eq 0 ]
then
    echo "   ...Result: Ok"
    exit 0
else
    echo "   ...Result: Err"
fi

exit 1
