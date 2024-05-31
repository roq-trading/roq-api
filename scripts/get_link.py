#!/usr/bin/env python

"""
Copyright (c) 2017-2024, Hans Erik Thrane

Command-line tool to get import link from json.
"""

import json
import sys

def main():
    path = sys.argv[1]
    with open(path) as file:
        doc = json.load(file)
        print('{}'.format(doc['link']), end='')


if __name__ == "__main__":
    main()
