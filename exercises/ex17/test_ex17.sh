#!/usr/bin/env bash

set -euo pipefail

DB="test-ex17.dat"

rm -f "$DB"

echo "[1] Create database"
./ex17 "$DB" c

echo "[2] Add records"
./ex17 "$DB" s 1 zed zed@example.com
./ex17 "$DB" s 2 frank frank@example.com
./ex17 "$DB" s 3 joe joe@example.com

echo "[3] List records"
./ex17 "$DB" l | tee logs/test-list.log

grep -q "^1 zed zed@example.com$" \
    logs/test-list.log

grep -q "^2 frank frank@example.com$" \
    logs/test-list.log

grep -q "^3 joe joe@example.com$" \
    logs/test-list.log

echo "[4] Get record"
./ex17 "$DB" g 2 | tee logs/test-get.log

grep -q "^2 frank frank@example.com$" \
    logs/test-get.log

echo "[5] Delete record"
./ex17 "$DB" d 3

./ex17 "$DB" l | tee logs/test-after-delete.log

if grep -q "^3 " logs/test-after-delete.log
then
    echo "Deleted record still exists."
    exit 1
fi

echo "[6] Reject invalid ID"

if ./ex17 "$DB" g -1
then
    echo "Negative ID was incorrectly accepted."
    exit 1
fi

rm -f "$DB"

echo "All tests passed."
