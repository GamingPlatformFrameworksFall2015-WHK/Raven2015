# Raven2015
a 2D, C++ game engine developed jointly by Will Nations, Hailee Ammons, and Kevin Wang 

# Developers
## Installation

This installation guide will assume a build for Visual Studio 2015, built for a Windows x64 platform. As a team, we wished to develop for VS, and the only edition of the VC++ compiler that would work with the EntityX library was that of VS2015. Both of the main libraries used for this game engine are usable for Windows, Mac OSX, and Linux platforms, but as we only have explicit experience in installing for Windows' Visual Studio, that is the only one we will be creating a direct guide for. If you wish to develop on a different platform or using a different IDE, I suggest you visit the documentation for the two libraries listed below.

1. Ensure that Visual Studio 2015 is installed on your computer. Any edition of the actual IDE will be fine.

2. Acquire a VS2015-enabled version of the Simple and Fast Multimedia Library (SFML) version 2.3+. You can build one yourself or go to www.nightlybuilds.ch/project/show/1/SFML/. The file near the bottom entitled "Visual Studio (2015RC) x64" will provide you with an archive you can extract the libraries from.

3. Create your Visual Studio 2015 project. Make sure that it is listed as a C++ Win32 Application.

4. Right click your project title, then go to Properties. At the top, make sure "All Configurations" is selected, then go to Configuration Properties -> VC++ Directories. Under "Include Directories", go to your SFML extraction location and select the "include" folder. Then hit Apply. After that, under "Library Directories", go to the same location and select the "lib" folder. Then hit Apply. Open your File Explorer, go to the "bin" folder of the SFML extraction location and copy all of the .dll files there into your project files (next to where you would put your main.cpp and the like).

5. Back in your Project Properties, go to Linker->System and set the Subsystem property to be "Console". Hit Apply.

6. <p>Go to Linker->Input. Ensure that you NOW set your Configuration (located in the top-left corner of the window) to "Debug". Then click on the "Additional Dependencies" property. Type in the following...</p>
    <code>sfml-graphics-d.lib;
    sfml-window-d.lib;
    sfml-audio-d.lib;
    sfml-network-d.lib;
    sfml-main-d.lib;
    sfml-system-d.lib;</code>
<p>Note: Each item must be separated by either a newline or a semi-colon. "system" must be at the bottom, "main" must be above it, and "graphics" must be at the top. The others shouldn't have any dependencies with each other.</p>

7. Afterwards, switch the configuration to "Release" and do the exact same thing as the previous step except omit the "-d" portion of each of the statements. So instead of using "sfml-graphics-d.lib", you'll use "sfml-graphics.lib".

8. Go to Linker->Advanced and ensure that the "Target Machine" property is set to an x64 machine.

9. After setting your solution configuration to x64 (at the top of your main editor window), build the project and attempt to run it. It -should- compile and link just fine.
