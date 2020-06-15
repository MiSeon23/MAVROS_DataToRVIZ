# MAVROS_DataToRVIZ

Recieved topic
   - /mavros/local_position/pose
   
Recieved Data
   - position ( x, y, z )
   - orientation(;quaternion) ( x, y, z, w )
   
Created Data
   - EulerAngles ( roll, pitch, yaw )(rad)


### rviz setting

Frame name
   - position
   
---

#### HOW TO BUILD

```
git clone https://github.com/MiSeon23/MAVROS_DataToRviz

ship to <your_workspace>

catkin_make
```


#### HOW TO RUN

```
roscore

rosrun debug_drone getPosition
```
