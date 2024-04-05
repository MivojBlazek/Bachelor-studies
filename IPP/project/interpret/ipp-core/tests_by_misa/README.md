# Tests for IPP interpret 2024

Author: <xblaze38> Michal Blažek

Software is provided 'AS IS' so it can contains bugs and errors.These tests can make incorrect results. Use it with carefully.
NOONE is able to share these tests.

## Execution

To run these tests it is required to have functional `php8.3 interpret.php` command and `interpret.php` file should be in parent directory of these files.

I recommend you to make a new directory in same place as `interpret.php` and download it inside.

* command `make` runs testing program

* command `make run` is the same as `make`

* command `make clean` clears files that were created from running your program

## Description of files

* `TEST_NAME.src` is source file with XML code

* `TEST_NAME.in` is input file of certain test

* `TEST_NAME.rc` is expected return code of certain test

* `TEST_NAME.out` is expected output of certain test

* `TEST_NAME.your_rc` is your return code from running your program

* `TEST_NAME.your_out` is your output of certain test from running your program

* `TEST_NAME.rc_diff` is difference of your return code with expected

* `TEST_NAME.out_diff` is difference of your output with expected output
