#!/usr/bin/env bash

find . -name "*.json" -exec bash -c "python -mjson.tool {} > tmp" \; -exec mv tmp {} \;
