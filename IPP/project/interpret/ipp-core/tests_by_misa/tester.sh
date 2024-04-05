
#
# @file tester.sh
#
# Tests for IPP interpret
#
# @author <xblaze38> Michal Blažek
#

#!/usr/bin/env bash
#!/bin/bash
#!/usr/bin/bash

print_result()
{
    if [ $1 -eq 0 ]; then
        echo -e "Out: \e[32mOK\e[0m"
        ((OK_COUNT++))
    else
        echo -e "Out: \e[31mFAIL\e[0m"
        ((FAIL_COUNT++))
    fi
}

print_result_rc()
{
    if [ $1 -eq 0 ]; then
        echo -e "Rc:  \e[32mOK\e[0m"
        ((OK_COUNT++))
        OK=1
    else
        echo -e "Rc:  \e[31mFAIL\e[0m"
        ((FAIL_COUNT++))
    fi
}

OK=0
OK_COUNT=0
FAIL_COUNT=0

PHP="php8.3 ../interpret.php"
PATH_TO_TESTS="./tests/"

for TEST in $PATH_TO_TESTS*.src
do
    INPUT_FILE="${TEST%.src}.in"
    if [ -f "$INPUT_FILE" ]; then
        $PHP --source="$TEST" --input="$INPUT_FILE" > "${TEST%.src}.your_out" 2>/dev/null
    else
        $PHP --source="$TEST" > "${TEST%.src}.your_out" 2>/dev/null
    fi
    echo $? > "${TEST%.src}.your_rc"

    echo -e "\e[34m$(basename ${TEST%.src})\e[0m"

    EXPECTED_RC="${TEST%.src}.rc"
    diff -Z -u "${EXPECTED_RC}" "${TEST%.src}.your_rc" > "${TEST%.src}.rc_diff"
    print_result_rc $?

    if [ "$(cat "$EXPECTED_RC")" -eq 0 ]; then
        if [ $OK -eq 1 ]; then
            ((OK_COUNT--))
        else
            ((FAIL_COUNT--))
        fi
        OK=0
        EXPECTED_OUT="${TEST%.src}.out"
        diff -u "${EXPECTED_OUT}" "${TEST%.src}.your_out" > "${TEST%.src}.out_diff"
        print_result $?
    fi

    echo -e ""
done

echo -e "\e[32mPASSED: \e[0m$OK_COUNT"
echo -e "\e[31mFAILED: \e[0m$FAIL_COUNT"
