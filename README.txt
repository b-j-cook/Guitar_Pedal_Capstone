Daisy Seed files and matlab simulation code can be found in the DaisySeed folder.

Analog distortion effect schematics can be found in the Analog Distortion Schematics folder. LTSpice and KiCad files are included.

Go into DaisySeed->DaisyExamples->MyProjects. The area we have been working in is the InitTest folder.

To edit, go to InitTest.cpp and edit. To upload changes cd into the InitTest folder.

You have to use git bash as your terminal otherwise it probably won't work.

Make clean and then make to remake the code. To flash, reset the daisy seed and enter the command
make program-dfu

Let me know if you have questions.
