#!/usr/bin/env bash

# This file should be sourced, not executed:
#   source dev_env.sh

# Find the directory where this script lives.
# This should be the Cantera repository root.
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Move to the repository root.
cd "$REPO_ROOT" || return 1

# Activate the Python virtual environment.
source ct-env/bin/activate

# Tell Python where to find the locally built Cantera Python package.
export PYTHONPATH="$REPO_ROOT/build/python:$PYTHONPATH"

# Tell Linux where to find the locally built Cantera shared library.
export LD_LIBRARY_PATH="$REPO_ROOT/build/lib:$LD_LIBRARY_PATH"

# Tell Cantera where to find mechanism/data files like gri30.yaml.
export CANTERA_DATA="$REPO_ROOT/data"

echo "Cantera dev environment loaded."
echo "Repo: $REPO_ROOT"
echo "Python: $(which python)"
