#!/bin/bash

PROG="./RPN"

RED='\033[0;31m'
GREEN='\033[32m'
YELLOW='\033[1;33m'
NC='\033[0m'

NB_TOTAL=0
NB_PASSED=0
NB_FAILED=0

strip_ansi() {
  sed -E 's/\\[[0-9;]*[A-Za-z]//g'
}

run_test()
{
	local testName="$1"
	local input="$2"
	local expected="$3"

	NB_TOTAL=$((NB_TOTAL + 1))

	echo -e "${YELLOW}$testName${NC}"
	echo "Input: '$input'"
	echo "Expected: '$expected'"

	local actualRaw=$($PROG "$input" 2>&1)

	echo "Result: '$actualRaw'"

	if [ "$actualRaw" = "$expected" ]; then
		echo -e "${GREEN}✓ PASSED${NC}"
		NB_PASSED=$((NB_PASSED + 1))
	else
		echo -e "${RED}x FAILED${NC}"
		NB_FAILED=$((NB_FAILED + 1))
	fi
	echo "------------------------------"
}

run_test "simple add" "3 4 +" "7"
run_test "simple sub" "3 4 -" "-1"
run_test "simple mul" "3 4 *" "12"
run_test "simple div" "4 2 /" "2"

run_test "complex" "7 7 * 7 -" "42"

run_test "should just stack" "1 2 * 2 / 2 * 2 4 - +" "0"

run_test "invalid token ()" "(1 1 +)" "Error"
run_test "missing operator" "1 1 + 1" "Error"
run_test "missing operand" "1 1 + -" "Error"

echo "=============================="
echo "TOTAL"
echo "=============================="
echo -e "${GREEN} Passed ${NB_PASSED} ${NC}"
echo -e "${RED} Failed ${NB_FAILED} ${NC}"