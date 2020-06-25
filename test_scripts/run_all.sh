#!/usr/bin/env bash


pushd `dirname ${0}` > /dev/null 2>&1

echo -e "\nRunning all tests:"
for i in ../programs/*
do
    ./run_test.sh $i ${1}
done

if [[ "${1}" == "again_quietly" ]]
then
    echo -e "\n\n\n\n\nRunning all tests again quietly:"
    for i in ../programs/*
    do
        ./run_test.sh $i "quietly"
    done
fi

popd > /dev/null 2>&1

exit 0
