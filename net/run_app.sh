#!/bin/bash

./CLIENT
find . | grep jpg | xargs eog
