# Bugs

- [X] Rotation glitching
- [X] Scale glitching
- [X] Rotation and scale don't update after Load
- [X] Rotation and scale cannot be used both at a time
- [X] Rotation of obstacle by 90 degree glitching (incorrect bounding box probably)
- [X] RobotList has only and exactly 5 robots
- [X] RobotList don't have right settings
- [X] Player can only step with robot but cannot tell him to go
- [X] Detection rectangle in not moving with robot
- [X] After spamming Left, Right or Forward, Robot will make these operations faster
- [X] Saving to json doesn't work well
- [X] Maybe loading from json doesn't work well too

- [X] Obstacles with different size then default have wrong center to rotation and scale
- [ ] To refresh robotID or save position after rotation/scale you need to click on the object
- [X] Forward button is not disabled/enabled correctly after clicking on left or right
    > Just enabled it fulltime so it doesn't look meh
- [X] After loading scene, obstacles change size to default
- [ ] After loading robotList and its setting resets
- [X] Clearing in pause and starting a new run makes program crash
- [X] Sometimes it crashes
    > Fixed crash after spawn player -> start -> forward -> reset -> start
- [X] Rubbish change shape after pause and continue
- [X] Rubbish glitching