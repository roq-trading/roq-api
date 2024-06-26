#!/usr/bin/env python3

"""                                                                                                                                                                
Copyright (c) 2017-2024, Hans Erik Thrane                                                                                                                       
                                                                                                                                                                
Helper script to create requirements.txt
"""

import argparse
import os
import sys
import ruamel.yaml

RESET = "\033[0m"
ERROR = "\033[1;31m"
MESSAGE = "\033[1;34m"


def print_error(*args, **kwargs):
    """
    Print error message
    """
    print(ERROR, file=sys.stderr, end="")
    print("ERROR: ", file=sys.stderr, end="")
    print(*args, file=sys.stderr, end="", **kwargs)
    print(RESET, file=sys.stderr)


def get_conda_pkg_ext():
    """
    Get conda package extension
    """
    machine = os.uname().machine
    if machine == "x86_64":
        return "64"
    if machine == "aarch64":
        return machine
    if machine == "arm64":
        return machine
    print_error("Unknown architecture.")
    sys.exit(1)


def get_conda_compiler():
    """
    Get conda compiler package name
    """
    pkg_ext = get_conda_pkg_ext()
    sysname = os.uname().sysname
    if sysname == "Linux":
        return f"gxx_linux-{pkg_ext}"
    if sysname == "Darwin":
        return f"clang_osx-{pkg_ext}"
    print_error("Unknown kernel.")
    sys.exit(1)


def helper(x):
    """
    Helper parsing requirements
    """
    if isinstance(x, str):
        return x
    for item in x:
        if "compiler('cxx')" in item:
            return get_conda_compiler()
    print_error(f"{x}")
    sys.exit(1)


def read_meta(meta, output):
    """
    Parse conda/meta.yaml to find list of dependencies
    """
    with open(meta, encoding="utf-8") as fd:
        yaml = ruamel.yaml.YAML(typ="rt")
        doc = yaml.load(fd.read())
        requirements = doc["requirements"]
        x = set()
        for _, values in requirements.items():
            x.update([helper(item) for item in values])
        with open(output, "w", encoding="utf-8") as f:
            for line in sorted(x):
                f.write(f"{line}\n")


def main():
    """
    Main
    """
    parser = argparse.ArgumentParser(description="Helper script")
    parser.add_argument("--meta")
    parser.add_argument("--output")
    args = parser.parse_args()
    read_meta(args.meta, args.output)


if __name__ == "__main__":
    main()
