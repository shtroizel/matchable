#!/usr/bin/env bash


pushd `dirname ${0}` > /dev/null 2>&1

failed=()

echo -e "\nRunning all tests:"
for i in ../programs/*
do
    if ! ./run_test.sh $i ${1}
    then
        failed+=("`basename ${i}`")
    fi
done

if [[ "${1}" == "again_quietly" ]]
then
    echo -e "\n\n\n\n\nRunning all tests again quietly:"
    for i in ../programs/*
    do
        if ! ./run_test.sh $i "quietly"
        then
            failed+=("`basename ${i}` (quietly)")
        fi
    done
fi

popd > /dev/null 2>&1

if [ ${#failed[@]} -ne 0 ]
then
    echo -e "\n${#failed[@]} test run(s) failed:"
    for f in "${failed[@]}"
    do
        echo "    ${f}"
    done
    exit 1
fi

echo -e "\nAll tests passed"
exit 0
