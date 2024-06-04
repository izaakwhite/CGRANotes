# How to Install -- WSL
A guide on how to install Standford's AHA CGRA 
## Pre Reqs

* Python 3.6 or <
* CMAKE
* GCC, C++ 11 or <, GNU
* Docker

## Installation

```bash
# Clone the AHA repository
git clone https://github.com/StanfordAHA/aha aha
cd aha

# Initialize and update submodules
git submodule update --init --recursive

```

# How to Start -- WSL
### In ~./aha
```bash
# Set up docker daemon
sudo nohup dockerd &

# Build the Docker image -- This may take a while & a lot of computing power the first time
docker build . -t aha_image

# Run a Docker container from the built image
docker run -it --name aha_container aha_image bash
```
--- 
**Note: This took personally took me 3 hours**
### If Already Built A container:
```bash
sudo docker rm -f aha_container
sudo docker run -it --name aha_container aha_image bash
```

# Now In Container:
```bash
source /aha/bin/activate

cd /aha/garnet
```
## Generate a 4x2 CGRA
```bash
python garnet.py --width 4 --height 2 --verilog
```
## Tid bits
**Note: All in ~./aha/garnet** 
### Help
```bash
python garnet.py --help
```
### Test Installation
```bash
pytest
```
###  Make file
running `make` outputs:
```
To test your installation simply do:
  pytest

For help do this:
  python garnet.py --help

Example build:
  python garnet.py --width 2 --height 2

To clean up after building a design (moves the entire design to a subdirectory):
  make clean
```
### Verify Functionality -- via [Standford AHA garnet README](https://github.com/StanfordAHA/garnet/blob/a93d3e6a35d385bcd9b25bfdaf17702f81d5c773/README.md)
```bash
cd /aha; ./garnet/.github/scripts/run_pytest.sh
```

# To Do
I have been poking around the files and the github to figure out how to run a simple test program on this cgra so for now my to do list kinda looks like
- [ ] run a program on the cgra
- [ ] reasearch other cgra simulators
 
## Branches - Master
### What I'm doing ATM
Copy of standford files + my simple 'hello world' program & shell script that I'm trying to get to run on the simulated CGRA