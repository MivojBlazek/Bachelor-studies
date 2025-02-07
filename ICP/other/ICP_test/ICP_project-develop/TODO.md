# TODO

## Main project tasks

- [ ] model has to be separated from GUI
- [X] generate documentation with doxygen
- [ ] usage of correct design patterns
- [X] toolkit Qt in version 5.5 for GUI
    > Qt is in version 5.15.3
- [X] Tested on Merlin
- [ ] Linux and Windows support
- [X] C++17 standard without Boost
- [X] Makefile (make, make run, make doxygen, make clean, make pack)
- [ ] class diagram in pdf
- [ ] bibliography in README.txt
- [ ] documentation of implemented and not implemented parts in README.txt

## Tasks for project submitting

- [X] xblaze38-xmicha94.zip / xblaze38-xmicha94-XX-XX.zip (XX = % of points)
- [X] source file should contain headers (author, content, ...)

## Tasks for individual parts

### GUI screen with:

* [X] Spawn obstacle button
    * [X] Movable
    * [X] Scalable
    * [X] Rotatable
    * [X] Scalable
* [X] Spawn robot button
    * [X] Movable
    * [X] Rotatable
* [X] Spawn player robot button
    * [X] Movable
    * [X] Rotatable
* [X] Player robot buttons
    * [X] Forward button
    * [X] Left button
    * [X] Right button
* [X] Save button
* [X] Load button
* [X] Start button
* [X] Pause button
* [X] Continue button
* [X] Reset button
* [X] End button
* [X] List of robots with parameters
    * [X] Detection distance
    * [X] Turning angle
    * [X] Turning direction
* [ ] Add delete option to context menu of objects

### Logic of robots and player:

* [X] Robot should move
* [X] Robot should turn left or right
* [X] Player should move
* [X] Player should turn left or right
* [X] Detecting obstacles in way
* [X] Detecting end of canvas
* [X] Add detection rectangle
* [X] Delete scene wont completely delete the player robot

### Final tasks:

* [X] Obstacles has texture or fill color
* [X] Something to reduce or increase speed of simulation
* [X] Generate randomly trash (after toggling generation via button)
* [ ] Setup detection distance to player robot
* [X] Spawn player robot under robots
* [ ] Each robot saves its settings to json and then loads it correctly
* [ ] CLEAN CODE (eg. delete commented code)
* [X] Window title (main.cpp)