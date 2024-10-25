# CS2 External radar cheat

This project is an external radar cheat created for CS2, utilizing C++ and SFML.
**<span style="color:red">This project was developed solely for educational purposes, and I do not endorse the idea of cheating.</span>**

# Usage demonstration

This cheat is designed for use on a secondary monitor or alongside the game in windowed mode, with the cheat's window positioned to the side.

https://github.com/user-attachments/assets/6e02a544-42a1-43f3-a218-20a1759f3736

# Building the project

1. Clone the repo and create a build folder

   ```bash
   git clone https://github.com/bordeanu05/cs2cheat.git
   cd cs2cheat
   mkdir build
   ```

2. Add SFML to the project

   - [Download SFML 2.6.1](https://www.sfml-dev.org/download/sfml/2.6.1/) (make sure you download the one with the same architecture as your C++ compiler)

   - Inside the src folder create another folder called "sfml" and paste the bin, include and lib folders from the downloaded SFML in there

   ```bash
   cd src
   mkdir sfml
   <copy-paste the bin, include and lib folders here ...>
   ```

   - Inside the build folder you should paste all the .dll files from the SFML bin folder. This is required for the program to start

3. Add radar images (I will not provide the images here, but you can find them online)
   Create a folder called "radar_imgs" inside the build folder and paste all radar images in there. Make sure they are named accordingly (eg. de_mirage.png, de_ancient.png)

4. Build the project

   ```bash
   cd build
   cmake ..
   cmake --build ./ -j
   ```

**In case of a CS2 update, chances are the cheat will stop working. In case that happens you can go to [this repo](https://github.com/a2x/cs2-dumper) and find the updated offsets. After you find the new offsets, open offsets.hpp and change them, then you can build the project again and the cheat should work.**

# Safety/Ban concerns

This cheat is designed to be difficult for VAC (Valve Anti-Cheat) to detect because it only reads the game's process memory without modifying any in-game data. However, its undetectability largely depends on it not becoming widely known. It is important to note that this cheat should NOT be used on FACEIT servers.
