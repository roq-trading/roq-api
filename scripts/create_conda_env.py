#!/usr/bin/env python3

import os
import sys
import subprocess
import urllib.request

RESET = "\033[0m"
ERROR = "\033[1;31m"
MESSAGE = "\033[1;34m"


def print_error(*args, **kwargs):
    print(ERROR, file=sys.stderr, end="")
    print("ERROR: ", file=sys.stderr, end="")
    print(*args, file=sys.stderr, end="", **kwargs)
    print(RESET, file=sys.stderr)


def print_message(*args, **kwargs):
    print(MESSAGE, file=sys.stderr, end="")
    print(*args, file=sys.stderr, end="", **kwargs)
    print(RESET, file=sys.stderr)


def check_env():
    print_message("Checking...")
    if False and "CONDA_PREFIX" in os.environ:
        print_error("Please deactivate your conda environment before using this script. Use 'conda deactivate'.")
        sys.exit(1)
    home = os.environ["HOME"]
    anaconda = f"{home}/.anaconda"
    if os.path.isdir(anaconda):
        print_error("Refusing to install with Anaconda in the environment. Please remove.")
        sys.exit(1)


def get_conda_sysname():
    sysname = os.uname().sysname
    if sysname == "Linux":
        return sysname
    if sysname == "Darwin":
        return "MacOSX"
    print_error("Unknown kernel.")
    sys.exit(1)


def download_installer():
    print_message("Downloading installer...")
    machine = os.uname().machine
    sysname = get_conda_sysname()
    installer = f"Miniforge3-{sysname}-{machine}.sh"
    target = f"opt/{installer}"
    if not os.path.isfile(target):
        url = f"https://github.com/conda-forge/miniforge/releases/latest/download/{installer}"
        urllib.request.urlretrieve(url, target)
    return target


def install_conda(installer):
    print_message("Installing conda...")
    subprocess.check_call(
        [
            "bash",
            installer,
            "-b",
            "-p",
            "opt/conda/",
        ]
    )


def update_conda():
    print_message("Updating conda...")
    subprocess.check_call(
        [
            "opt/conda/bin/conda",
            "update",
            "--yes",
            "--name",
            "base",
            "--channel",
            "conda-forge",
            "conda",
        ]
    )


def helper(item):
    tmp = item.split()
    if len(tmp) > 0 and tmp[0] == "dev":
        return tmp[-1]
    return None


def create_env():
    print_message("Checking existing environments...")
    res = subprocess.run(["opt/conda/bin/conda", "env", "list"], stdout=subprocess.PIPE)
    if res.returncode != 0:
        print_error("Failed to fetch existing environments.")
        sys.exit(1)
    lines = [item.decode("utf-8") for item in res.stdout.splitlines()]
    tmp = [helper(item) for item in lines]
    exists = False
    for item in tmp:
        if item is not None:
            print(item)
            exists = True
    if exists:
        print_message("... exists")
    else:
        print_message("... does not exist")
        print_message("Creating dev environment...")
        subprocess.check_call(
            [
                "opt/conda/bin/conda",
                "create",
                "--name",
                "dev",
                "--yes",
            ]
        )


def create_requirements():
    print_message("Creating requirements file...")
    cwd = os.path.dirname(os.path.realpath(__file__))
    helper = f"{cwd}/_create_conda_env_helper.py"
    meta = "conda/meta.yaml"
    requirements = "opt/requirements.txt"
    # note! we need the helper because we want to use the yaml parser from the conda environment
    res = subprocess.check_call(
        [
            "opt/conda/bin/python3",
            helper,
            "--meta",
            meta,
            "--output",
            requirements,
        ]
    )
    res = subprocess.check_call(
        [
            "opt/conda/bin/conda",
            "install",
            "--name",
            "dev",
            "--yes",
            "--channel",
            "https://roq-trading.com/conda/unstable",
            "--channel",
            "conda-forge",
            "--file",
            requirements,
            "--quiet",
        ]
    )
    print(res)


def install_env():
    print_message("Installing env...")


def main():
    check_env()
    os.makedirs("opt/", exist_ok=True)
    installer = download_installer()
    if not os.path.isdir("opt/conda/"):
        install_conda(installer)
    update_conda()
    create_env()
    create_requirements()
    install_env()


if __name__ == "__main__":
    main()
