#!/usr/bin/robot -dout
*** Comment ***
comment section
comment section
comment section

*** Settings ***
Resource    ./configuration.robot
Resource    ./myKeywords.robot
# this is a comment

*** Test Cases ***
Display the list of configuration files
    Show files
