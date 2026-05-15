# Stretched Thickened-Flame Extension
# This fork is based on Cantera 3.2.0

## First, a virtual environment ct-env needs to be created using
python3 -m venv ct-env
source ct-env/bin/activate
## Then packages and dependencies need to be installed
python -m pip install --upgrade pip setuptools wheel
python -m pip install scons cython numpy ruamel.yaml pytest jinja2
python -m pip install "typing_extensions>=4.13.0"
## make sure boost and doxygen is installed (ubuntu below adapt as needed)
sudo apt update
sudo apt install build-essential g++ git python3 python3-venv python3-dev doxygen libboost-dev
## If Python.h not found error appears
#sudo apt install python3.12-dev
## Then build using scons (8 cores, adjust as per machine)
scons clean
scons build -j8
