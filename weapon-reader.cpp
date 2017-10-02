#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;


int main(int argc, char * argv[]){

  streampos size;
  char * fileContents;

  string filename = argv[1];
  string gdtFilename = argv[2];
  string weaponName = argv[3];
  string ignoreList;
  if(argc == 5){
    ignoreList = argv[4];
    }

  if(argc != 5 && argc != 4){
	cout << "Usage: weapon-reader bo2_data_filename bo3_gdt_filename.gdt weapon_name [ignoreList.txt]" << endl;
	cout << "argc: " << argc << endl;
	return 0;
  }

  int ignoreLineCount = 0;
  string * ignoreStringArray;

  //open ingnores list if it exists
  if(argc == 5){
    ifstream ignores;
    ignores.open(ignoreList);
    int x = 0;
    if(ignores.is_open()){
	string tempIgnore;
	while(getline(ignores, tempIgnore)){
	   x++;
	}
        ignoreLineCount = x;
	ignores.clear();
        ignores.seekg(0, ios::beg);
        ignoreStringArray = new string[ignoreLineCount];    

        int y = 0;

        cout << "temp: " << tempIgnore << endl;

        while((y < ignoreLineCount) && getline(ignores,ignoreStringArray[y])){
	   y++;
        }

        ignores.close();

    }
    else{
	cout << "Unable to read ignore list" << endl;
	return 0;
    }
   

  }  

  ifstream weaponFile(filename, ios::in|ios::binary|ios::ate); 
  size = weaponFile.tellg();
  
  fileContents = new char[size];
  
  weaponFile.seekg(0, ios::beg);
  weaponFile.read(fileContents, size);
  weaponFile.close();

  int slashCount = 0;

  char newContents[(int) size];

  for(int i = 0; i < (int) size; i++){
	if(fileContents[i] == '\\'){
	   newContents[i] = '\n';
	   slashCount++;
	}
	else{
	    newContents[i] = fileContents[i];
	}
  }

  
  //printf("%d", argc);
  //printf("%s",newContents);
  ofstream outFile(filename + "_readable.txt", ios::out|ios::binary);
  
  outFile.write(newContents,size);
  outFile.write("\n", 1);
  outFile.close();

  string bo2VariableRaw[slashCount + 2];
  string bo2VariableNames[(slashCount + 1) / 2];
  string bo2VariableValues[(slashCount + 1) / 2];

  ifstream readableFile;
  readableFile.open(filename + "_readable.txt");

  string plainString;

  if(readableFile.is_open()){
  int x = 0;

  while(getline(readableFile, bo2VariableRaw[x])){
	x++;
  }
  }

  readableFile.close();

  

  int rawCounter = 1;
  for(int z = 0; z < slashCount / 2; z++){
	bo2VariableNames[z] = bo2VariableRaw[rawCounter];
	rawCounter++;
	bo2VariableValues[z] = bo2VariableRaw[rawCounter];
	rawCounter++;
  }

/*

 for(int q = 0; q < slashCount / 2; q++){
	cout << bo2VariableNames[q] << endl;
	cout << "\t" << bo2VariableValues[q] << endl;
 } 
*/

  int gdtLines = 0;

  //cout << "making gdt file reader" << endl; 
  ifstream gdtFile(gdtFilename);
  string unused = "";

  while(getline(gdtFile, unused)){
	gdtLines++;
  }



  gdtFile.clear();
  gdtFile.seekg(0, ios::beg);

  //getline(gdtFile,unused);
  //cout << "First GDT Line:" << unused << endl;

  string gdtArray[gdtLines];
  for(int b = 0; b < gdtLines; b++){
	getline(gdtFile, gdtArray[b]);
  }

  //find weapon section of gdt

  int weaponSectionStart = 0;
  int weaponSectionEnd = 0;
  bool foundLine = false;
  bool foundEnd = false;

  string gdtWeaponLine = "\t\"" + weaponName + "\" ( \"bulletweapon.gdf\" )" + (char) 13;
  string gdtEndLine = "\t}\r";

  for(int n = 0; n < gdtLines; n++){
	if(gdtArray[n] == gdtWeaponLine){
		weaponSectionStart = n;
		foundLine = true;
	}
	if(foundLine && gdtArray[n] == gdtEndLine && !foundEnd){
		weaponSectionEnd = n;
		foundEnd = true;	
	}
  }

	/*for(int g = 0; g < gdtArray[3537].length(); g++){
		cout << (int) gdtArray[3537].at(g) << endl;
	}

	cout << "end line =" << gdtEndLine << endl;
	
	for(int g = 0; g < gdtEndLine.length(); g++){
		cout << (int) gdtEndLine.at(g) << endl;
	}

	cout << "final char" << (int) gdtArray[3537].at(gdtArray[3537].length() - 1) << endl; 
	cout << "weaponSectionEnd: " << weaponSectionEnd << endl;
	*/
  string newArray[gdtLines] = gdtArray;
  if(!foundLine){
	cout << "could not find weapon" << endl;
  }


  
  else{
    for(int y = 2; y < weaponSectionEnd - weaponSectionStart; y++){
	string varName = "";
	int endVarName = -1;
	int gdtIndex = weaponSectionStart + y;
	for(int h = 4; h < gdtArray[gdtIndex].length(); h++){
		if(gdtArray[gdtIndex].at(h) == '\"' && endVarName == -1){
			endVarName = h;
		}
	}
	//cout << "gdtIndex: " << gdtIndex << endl;
	//cout << gdtArray[gdtIndex].substr(3, endVarName - 3) << endl;
	varName = gdtArray[gdtIndex].substr(3, endVarName - 3);

 	   bool ignoreVar = false;
	   for(int ig = 0; ig < ignoreLineCount; ig++){
	      if(varName.find(ignoreStringArray[ig]) != std::string::npos){
		//cout << "ignoring " << ignoreStringArray[ig] << " in " << varName<< endl;
		ignoreVar = true;
	      }
 	   }

	bool foundBO2Version = false;
	for(int v = 0; v < (slashCount + 1) / 2; v++){

	    if(varName == bo2VariableNames[v] && !ignoreVar){
		//cout << "found bo2 version of: " << varName << endl;
		foundBO2Version = true;
		newArray[gdtIndex] = gdtArray[gdtIndex].substr(0, endVarName) + "\" \"" + bo2VariableValues[v] + "\"\r";	
		//cout << newArray[gdtIndex] << endl;
	
	    }	
	}
	if(!foundBO2Version && !ignoreVar){
	   cout << "Did not find BO2 version of " << varName << endl;
	}
	if(ignoreVar){
	   cout << "Ignoring BO2 version of " << varName << endl;
	} 
     }

     string newFilename = "test.gdt";
     ofstream newGDTFile;
     newGDTFile.open(gdtFilename);

     for(int m = 0; m < gdtLines; m++){
	newGDTFile << newArray[m] << "\n";
     }

     cout << "\nWrote to file \"" + gdtFilename + "\"" << endl;

  }

  return 0;
}
