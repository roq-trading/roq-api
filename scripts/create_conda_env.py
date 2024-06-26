#!/usr/bin/env python3

"""                                                                                                                                                                
Copyright (c) 2017-2024, Hans Erik Thrane                                                                                                                       
                                                                                                                                                                
Installs Miniforge3 and creates a development environment
"""

import argparse
import os
import sys
import subprocess
import shutil
import urllib.request

RESET = "\033[0m"
ERROR = "\033[1;31m"
MESSAGE = "\033[1;34m"


def print_error(msg):
    """
    Print error message
    """
    print(ERROR, file=sys.stderr, end="")
    print("ERROR: ", file=sys.stderr, end="")
    print(msg, file=sys.stderr, end="")
    print(RESET, file=sys.stderr)


def print_message(msg):
    """
    Print message
    """
    print(MESSAGE, file=sys.stderr, end="")
    print(msg, file=sys.stderr, end="")
    print(RESET, file=sys.stderr)


def validate_env():
    """
    Validate the user's existing environment
    """
    # conda already activated?
    if "CONDA_PREFIX" in os.environ:
        print_error("Please deactivate your conda environment before using this script. Use 'conda deactivate'.")
        sys.exit(1)
    # have seen issues when anaconda was installed
    home = os.environ["HOME"]
    anaconda = f"{home}/.anaconda"
    if os.path.isdir(anaconda):
        print_error("Refusing to install with Anaconda in the environment. Please remove.")
        sys.exit(1)


def get_conda_sysname():
    """
    Get conda sysname
    """
    sysname = os.uname().sysname
    if sysname == "Linux":
        return sysname
    if sysname == "Darwin":
        return "MacOSX"
    print_error("Unknown kernel.")
    sys.exit(1)


def download_installer():
    """
    Download the Miniforge3 installer
    """
    print_message("Download installer...")
    machine = os.uname().machine
    sysname = get_conda_sysname()
    installer = f"Miniforge3-{sysname}-{machine}.sh"
    target = f"opt/{installer}"
    if not os.path.isfile(target):
        url = f"https://github.com/conda-forge/miniforge/releases/latest/download/{installer}"
        urllib.request.urlretrieve(url, target)
    return target


def install_conda(installer):
    """
    Execute the Miniforge3 installer
    """
    print_message("Install conda...")
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
    """
    Update the conda base environment
    """
    print_message("Update conda...")
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


def helper(item, name):
    """
    Helper when creating conda env
    """
    tmp = item.split()
    if len(tmp) > 0 and tmp[0] == name:
        return tmp[-1]
    return None


def create_conda_env(name):
    """
    Create conda environment
    """
    res = subprocess.run(["opt/conda/bin/conda", "env", "list"], stdout=subprocess.PIPE, check=True)
    lines = [item.decode("utf-8") for item in res.stdout.splitlines()]
    tmp = [helper(item, name) for item in lines]
    exists = False
    for item in tmp:
        if item is not None:
            print(item)
            exists = True
    if not exists:
        print_message("Create conda environment...")
        subprocess.check_call(
            [
                "opt/conda/bin/conda",
                "create",
                "--name",
                name,
                "--yes",
            ]
        )


def create_requirements():
    """
    Create a requirements.txt file (by parsing conda/meta.yaml)
    """
    print_message("Create requirements.txt (from conda/meta.yaml)...")
    cwd = os.path.dirname(os.path.realpath(__file__))
    script = f"{cwd}/_create_conda_env_helper.py"
    meta = "conda/meta.yaml"
    output = "opt/requirements.txt"
    # note! we need the helper script because we want to use the yaml parser from the conda environment
    subprocess.check_call(
        [
            "opt/conda/bin/python3",
            script,
            "--meta",
            meta,
            "--output",
            output,
        ]
    )
    return output


def install_conda_env(requirements, channel, name):
    """
    Create a conda environment from a requirements.txt file
    """
    print_message("Install dependencies...")
    roq_channel = f"https://roq-trading.com/conda/{channel}"
    subprocess.check_call(
        [
            "opt/conda/bin/conda",
            "install",
            "--name",
            name,
            "--yes",
            "--channel",
            roq_channel,
            "--channel",
            "conda-forge",
            "--file",
            requirements,
        ]
    )


def copy_activate_script(typ, name):
    """
    Create activate script for the conda environment
    """
    print_message("Copy activate script...")
    activate_dir = f"opt/conda/envs/{name}/etc/conda/activate.d"
    os.makedirs(activate_dir, exist_ok=True)
    cwd = os.path.dirname(os.path.realpath(__file__))
    arch = get_conda_sysname()
    source = f"{cwd}/conda/activate-{arch}-{typ}.sh"
    target = f"{activate_dir}/roq.sh"
    shutil.copy2(source, target)


def create_clion_helper():
    """
    LEGACY clion helper script
    """
    path = "opt/conda/bin/activate_dev"
    with open(path, "w", encoding="utf-8") as f:
        f.write("#!/usr/bin/env bash")
        f.write('SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )')
        f.write('source "$SCRIPT_DIR/activate" "dev" || return $?')


def main():
    """
    Main
    """
    parser = argparse.ArgumentParser(
        description="Miniforge3 installer script",
    )

    parser.add_argument("type", metavar="[debug|release]", type=str, nargs=1, help="type")
    parser.add_argument("channel", metavar="[unstable|stable]", type=str, nargs=1, help="channel")

    args = parser.parse_args()

    if args.type is None or args.channel is None:
        print_error("Expected two arguments.")
        sys.exit(1)

    typ = args.type[0]

    if not typ in {
        "debug",
        "release",
    }:
        print_error("Expected first argument to be type. Use 'debug' or 'release'.")
        sys.exit(1)

    channel = args.channel[0]

    if not channel in {
        "unstable",
        "stable",
    }:
        print_error("Expected second argument to be channel. Use 'unstable' or 'stable'.")
        sys.exit(1)

    validate_env()
    os.makedirs("opt/", exist_ok=True)
    installer = download_installer()
    if not os.path.isdir("opt/conda/"):
        install_conda(installer)
    update_conda()
    create_conda_env("dev")
    requirements = create_requirements()
    install_conda_env(requirements, channel, "dev")
    copy_activate_script(typ, "dev")
    create_clion_helper()
    print_message("Ready!")
    print_message("You can now activate your conda environment using 'source opt/conda/bin/activate dev'.")


if __name__ == "__main__":
    main()
