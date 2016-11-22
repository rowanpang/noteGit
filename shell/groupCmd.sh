#!/bin/bash

echo "test1" || { echo "sadfddd" && exit -1; }

echo "finish"
