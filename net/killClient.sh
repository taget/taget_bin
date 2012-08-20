#!/bin/bash
ps aux | grep CLIENT | grep -v grep  |awk '{print $2}'  |xargs kill -9

