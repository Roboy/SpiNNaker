**A plugin for Gazebo to communicate with SpiNNaker board.**

Add model of the arm to your Gazebo models.
```
#!bin/bash
cd ~/.gazebo/models
sudo ln -s /path/to/SpiNNaker/spinn_arm
```

Build:
```
#!bin/bash
echo 'export GAZEBO_PLUGIN_PATH=$/path/to/SpiNNaker/build:$GAZEBO_PLUGIN_PATH' >> /your/.bashrc
source /your/.bashrc
cd /path/to/SpiNNaker/build/
make clean #if not empty
cmake ../
make
```

Run:
```
#!bin/bash
cd /path/to/SpiNNaker
gzserver --verbose armworld.sdf
```
