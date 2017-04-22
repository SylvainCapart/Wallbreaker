Name : Wallbreaker (Quadrabreaker)
Author : Sylvain Capart
Date : 01/2017

INSTALLATION

1. Ensure that you have SDL2 and SDL2_image libraries on your computer.
If not, they can be found at : https://www.libsdl.org/download-2.0.php
Download the development libraries corresponding to your platform.
For more information about SDL2 installation for your plateform : https://wiki.libsdl.org/Installation

SDL_ttf 2.0 library is also needed : https://www.libsdl.org/projects/SDL_ttf/ 
If you need to compile the sources (./configure, make, make install) to get the library, you will also need the FreeType library : https://www.freetype.org/download.html#stable

2. Check that you have cmake on your platform. You can open a terminal and type "cmake -h" to check.
If not it can be found on cmake.org.

NB : if cmake -h does not find anything but you know cmake is present, check that cmake binaries path is in your environment path.

3. Generate the makefile 
	- first possibility : open the graphical user interface Cmake
						  specify you source folder (where you have your Wallbreaker folder)
						  specify the build folder (for example xxx/Wallbreaker/build)
						  click on Configure
						  if asked if you want to create "build", answer yes
						  if proposed to choose the compiler, keep "use default native compiler", and choose your compiler in the list
						  click on Generate

	- second possibility : if you do not have the CMake GUI, open a terminal
						   go in the project folder by typing : cd xxx/Wallbreaker
						   type : mkdir build
						   type : cd build
						   if you are on a Windows plateform, open your visual studio environment 
						   ON UNIX : type : cmake -G "Unix Makefiles" ..
						   ON WINDOWS with VS 2010 : type : cmake -G "Visual Studio 10" ..
						   ON WINDOWS with VS 2005 : type : cmake -G "Visual Studio 8" ..
						   (complete list of makefile generator can be found at : https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html)
						   IF NOT WINDOWS type : make ( on windows a visual studio solution is generated )

	NB : it is possible also to generate the makefile from your development environment.

4. Open your favorite development environment
5. Create a new project with existing sources : select project folder
					          if asked, select use makefile
					          if the option is present, accept to run configure scripts in subfolder
						   
							
6. Build and run the project