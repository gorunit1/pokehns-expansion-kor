#!/usr/bin/env bash
# usage: showtrace.sh <trace.txt> <test-id e.g. "K1-06"> : print that test's decoded trace block
awk -v id="$2" '/^## /{p = index($0, " " id " ") > 0} p' "$1"
