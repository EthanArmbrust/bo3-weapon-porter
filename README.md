# bo3-weapon-porter
Ports weapon gdt data from previous Call of Duty games to BO3


## Usage
`weapon-reader.exe old_weapon_file bo3_gdt_filename.gdt weapon_name [ignore_list.txt]`  

`old_weapon_file` is the data that you are porting over from the previous game.  These can be found here: http://denkirson.proboards.com/thread/4943/call-duty-weapon-attachment-stats  
Download the Weapon Files from the game you want, the unzip the specific weapon you want to port.  
  
`bo3_gdt_filename.gdt` is the filename of your weapon gdt. You must create this in APE before you start this process.  Your gdt should be located in the source_data folder of your Black Ops III install.  
  
`weapon_name` is the name of the bulletweapon asset in your GDT.  You must create this in APE before you start this process.  Ex. `ar_m14`  
  
`ignore_list.txt` is an optional text file you can include to ignore certain variables.  All variable names that contain a value from the ignored list will be ignored.  Put one ignore entry per newline.  Ex.  
  
example_ignore.txt  
```
Sound
Anim
```  
Any variables with names containing `Sound` or `Anim` will be ignored and ported over from the original weapon file.  
Ex. The values for `firstRaiseAnim` and `fireSound` from the original weapon file will not be copied over to the BO3 GDT.  
  
Example Usage:  
`weapon-reader.exe C:\ballista_zm "D:\Steam\steamapps\Call of Duty Black Ops III\source_data\custom_weapons.gdt" ballista_zm C:\example_ignore.txt`  

