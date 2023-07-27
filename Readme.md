# Compiling
For solveing the radiance matrix equation, I introduced a library called Eigen which is not in the original environment. Before Compiling, please install the Eigen library first. The installation link is here: http://eigen.tuxfamily.org/index.php?title=Main_Page#Download. The package I downloaded is https://gitlab.com/libeigen/eigen/-/archive/3.3.9/eigen-3.3.9.tar.bz2. After downloading the zip file, there is a install instruction in the directory. Please follow the instructions to install the Eigen library.

Open this directory in the CS488 virtual machine environment, run 'premake4 gmake', then run 'make'

# Scenes
The scenes are hardcoded. No lua files are accepted. 

Scene1 is an empty Box. Scene2 is scene1 with a red cube laying on the ground. 

There are 3 light settings. Light1 is on the ceiling, light2 is on the left wall, light3 contains both light1 and light2.

# Program Parameters
The program can take up to 4 command line argument. The first argument should be either 0 or 1. 0 means using montecarlo and 1 means using hemicube for form factor calculation. The second argument is the square root of number of patches each surface will be divided into. Entering 4 means each surface will be divided into 4$\times$ 4 = 16 patches. The third argument is choosing the scene. The argument can be either 1 or 2. 1 means scene1 and 2 means scene2. Similarly, the 4th argument represents the choice of light sources. Valid values are 1, 2, 3 and each represent a light source described above in Scenes.

All these arguments are optional. The default situation uses scene1 and light source 1 with 4$\times$ 4 patches and montecarlo method.

The place to change monte carlo ray number is in the GeometryNode.cpp, in function GeometryNode::formfactor_montecarlo. By changein the ray_number, changes the number of rays simulated.

# Running Instructions
The command for running the program is "./Renderer argv{1} argv{2} argv{3} argv{4}". 

Running this prgram the first time for each scene patch number combination creates a new file in directory FormFactors. The new file is a csv file which stores the form factors of each patch. The file is named "{SceneName}_{patchNumber}_{FormFactorMethods}.csv". This reduces computation time for future uses. At the same time, the time cost for form factor computation will show on the command line prompt. 

In the end, a picture named "{FormFactorMethods}{SceneName}.png" will be generated in the same directory.