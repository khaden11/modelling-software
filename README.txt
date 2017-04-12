COURSEWORK 5

TO RUN

Please run the commands qmake and make to run the executable. If not working try importing
into qtcreator.

This modelling software is still a work in progress.

The many features I tried to implement along with the deadline extension and
lack of time meant that I had to submit this program unfinished.

Whilst I have still got two advanced features, arcball rotation and collision detection,
I have implemented them in different ways than before.

MAIN WORKING FEATURES

Arcball - an altered arcball rotation is done by rotating the camera with the right mouse
button

Object selected - lets the user select any object drawn to screen and alter/see it's
properties. Done by rendering the objects in different colours/getting the colour of the
selected pixels.

Arcball - after selecting an object, use the right mouse button to rotate the object

Collision detection - Used to draw the objects onto the draw plane (viewed with the checkbox).

Load/Save Scene - Loads and saves scenes of objects.

UNFINISHED FEATURE

Object translation - a selected object does allow you to try and translate the object by
dragging the y axes but unfortunately, results in a bug that doesn't allow you to further
edit the object. This was done by an object picking method to recognise the clicked colour
of the axes.

FUTURE AIMS

I do aim to finish and improve this software over time. The rushing meant that some of the
code is still a little messy and unorganised. I would like to further implement some extra
features when I have time. An animation editor was planned for the bottom - to interpolate
transformations between key frames to produce them. More mouse controls for object manipulation
also need to be implemented. The importing of custom objects and user adding lighting would also
be useful.
