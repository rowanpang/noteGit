#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

// Read unsigned char (byte)
unsigned char readByte(ifstream &thisFile){
    char buffer;
    thisFile.get(buffer);
    return (unsigned char)buffer;
}

#define COLUMN_SEPARATE " >>||<<"
#define DATA_PER_LINE 8
int main(int argc, char** argv)
{
    if (argc <3)
    {
	cout << "error!";
	return 1;
    }
    ifstream binFileA(argv[1]);
    ifstream binFileB(argv[2]);

    int offset = 0;
    unsigned char byteA;
    unsigned char byteB;
    unsigned char aEof;
    unsigned char bEof;

    cout << hex;

    aEof = binFileA.eof();
    bEof = binFileB.eof();
    //while(!binFileA.eof() && !binFileB.eof())
    while((!aEof) || (!bEof))
    {
	// Display file A
	for(int i=0; i< DATA_PER_LINE; i++)
	{
	    if(!aEof)
	    {
		byteA = readByte(binFileA);
		cout << setw(3) << (int)byteA << " ";
	    }else
	    {
		cout << " __ ";
	    }
	}
	cout << COLUMN_SEPARATE;

	// Display both files with "__" as difference
	if(!aEof && !bEof)
	{
	    binFileA.seekg(-DATA_PER_LINE, ios::cur);
	}
	for(int i=0; i<DATA_PER_LINE; i++)
	{
	    if(aEof || bEof)
	    {
		cout << " __ ";
	    }else
	    {
		byteA = readByte(binFileA);
		byteB = readByte(binFileB);
		if(byteA != byteB){
		    cout << " __ ";
		}
		else
		{
		    cout << setw(3) << (int)byteA << " ";
		}
	    }
	}
	cout << COLUMN_SEPARATE;
    
	// Display file B
	if(!aEof && !bEof)
	{
	    binFileB.seekg(-DATA_PER_LINE, ios::cur);
	}
	for(int i=0; i<DATA_PER_LINE; i++)
	{
	    if(!bEof)
	    {
		byteB = readByte(binFileB);
		cout << setw(3) << (int)byteB << " ";
	    }
	    else
	    {
		cout << " __ ";
	    }
	}
	cout << endl;

	aEof = binFileA.eof();
	bEof = binFileB.eof();
    }

    binFileB.close();
    binFileB.close();
}
