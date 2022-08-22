# Graphical-Programs
Author: Michael Koslowski

The following is a wavefront.obj converter. It is a basic converter, that I use to create raw model files which are directly used by my OpenGL program. 
Its goal is to combine Vertex-Coordinates, Texture-Coordinates and Normals and to delete any duplicates. The correct order is saved by the indices. Also any redundant text, newLines and leading zero's are deleted.

It converts wavefront.obj files, as they are used in for example Blender to a new file. That new file can be split into 4 segments; each seperated by one single "newLine"('\n'):

I Name of the specified Vertex, Fragment and optional Geometry Shader

II First number represents how many Textures are used by the model. Right now, it is Hardcoded to 1. 
II Second number is the count of unique Full Vertices
II Third number is the count of indices.

III Full Vertices that are unique combinations of Vertex-Coordinates, Texture-Coordinates and Normals

IV the indices needed to draw the model.

There is an example file called "backpack.obj". "backpack.k_model" is the converted version. I am not the creator nor do I own that file. 
The creater Berk Gedik provides a free version for everyone to use at https://sketchfab.com/3d-models/survival-guitar-backpack-799f8c4511f84fab8c3f12887f7e6b36
If you want the textures and materials to that model, you have to download it from there!

The converter is written with the c++20 standard in mind, but can easily be rewritten to be downwards compatible. Feel free to use and modify the program as you need. 
As an amateur, I can not guarantee file safety. Any damages or loss of data are the users obligation to avoid.
