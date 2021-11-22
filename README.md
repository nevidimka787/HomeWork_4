# HomeWork_3

The repository collect my solutions of home tasks number 4. My task number is 11 (hard).

The code wrote on ubuntu 18.04.
The code wrote on c++ language.
 
# Program building
To build program:
1. Open console in program's folder.
2. Past command make <atribut>
Atributs list:
1. rebuild -- rebuild all program's files.
2. <file_name>.o -- rebuild current file only.
3. rm -- remove all bin files

Binarum file is in bin directory. To start program past in terminal "./Task3.elf"

# Program description
The program gets data from the console.
Format of the data:
point -- point
point -- point
...

After that program paint input graph and print it to new window.

Wisualisation of graph can be moved by mouse and scaled by keys on the keyboard.

Visualisation control:
1. Moving mouse whis active left mouse button -> move graph
2. Arrow up    -> scale up grahp
3. Arrow down  -> scale down graph


# Libraries
The program use OpenGL libraries and use "glad" for work with OpenGL's functions.

The program use stb_image for work with imajes (textures).
https://github.com/nothings/stb/blob/master/stb_image.h



