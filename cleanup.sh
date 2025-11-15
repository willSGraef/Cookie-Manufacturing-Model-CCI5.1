#!/usr/bin/env bash

# This scripts helps with liquidswrds's OCD by cleaning up unwanted files.

find . -type d -name "__pycache__" -exec rm -rf {} +

find . -type f -name '~$*' -exec rm -f {} +
