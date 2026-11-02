#!/bin/bash

echo "Starting configuring cli..."
cmake -S ../src/frontend -B ../build
echo "cli has configured successfuly!"

echo "Starting building cli..."
cmake --build ../build
echo "cli has built successfuly!"