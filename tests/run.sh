#!/bin/sh

TARGET=$1
DIR=tests

expect() {
	EXPECTED=$1
	TEST=$2
	if [ -z "$3" ]; then
		ACTUAL=`$TARGET $DIR/$TEST 2>&1`
	else
		INFILE=$3
		ACTUAL=`$TARGET $DIR/$TEST 2>&1 < $DIR/$INFILE`
	fi

	if [ "$ACTUAL" = "$EXPECTED" ]; then
		echo $TEST PASSED
	else
		echo $TEST FAILED
		echo "\t" got: $ACTUAL
		echo "\t" expected: $EXPECTED
	fi
}

expect "Hello World!" helloworld.bf
expect "151068" add.bf add.in
expect "~" t0.bf
expect "Compile Error: Unmatched ']'" t1.bf
expect "Compile Error: Unmatched '['" t2.bf
expect "Runtime Error: ptr fell off left end of tape" t3.bf
expect "Runtime Error: ptr fell off right end of tape" t4.bf
expect "Compile Error: Max stack size would be exceeded" t5.bf