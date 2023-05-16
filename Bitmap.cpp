#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <vector>
#include "Bitmap.h"

using namespace std;
void Bitmap::LoadBMP(const char* Filename, vector<unsigned char>* image, unsigned int* w, unsigned int* h)
{
    BITMAPFILEHEADER    bmfh;  //stores information about the file format
    BITMAPINFOHEADER    bmih;  //stores information about the bitmap
    FILE* file; //stores file pointer
    UINT _Width, _Height;

    //open the file and read in the headers
    //file = Utility::CheckedFOpen(Filename.CString(), "rb");
  
    // open the file and read in the headers
    file = fopen(Filename, "r");
    if (file == NULL)
    {
        cout << "Failed to open file: " << Filename << endl;
        return;
    }


    fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&bmih, sizeof(BITMAPINFOHEADER), 1, file);

    _Width = bmih.biWidth;
    _Height = abs(bmih.biHeight);
    *w = _Width;
    *h = _Height;
    if (bmih.biBitCount == 32)
    {
        //Allocate space for the read operation
        /*Allocate(_Width, _Height);*/

        //save all header and bitmap information into file
        fread(image, sizeof(unsigned char) * 3, _Width * _Height, file);
    }
    else if (bmih.biBitCount == 24)
    {
        //Allocate space for the read operation
        /*Allocate(_Width, _Height);*/

        UINT Pitch = _Width * 3;
        UINT ExcessPitch = 0;
        while (double(Pitch / 4) != double(Pitch) / 4.0)
        {
            Pitch++;
            ExcessPitch++;
        }

        unsigned char* DataStore = new unsigned char[Pitch * _Height];
        fread(DataStore, 1, Pitch * _Height, file);

        UINT CurDataPos = 0;
        for (UINT i = 0; i < _Height; i++)
        {
            for (UINT i2 = 0; i2 < _Width; i2++)
            {
                //RGBColor CurColor;
                //CurColor.b = DataStore[CurDataPos++];
                //CurColor.g = DataStore[CurDataPos++];
                //CurColor.r = DataStore[CurDataPos++];
                //CurColor.a = 0;
                //_Data[i * _Width + i2] = CurColor;
                image->push_back(DataStore[CurDataPos++]);
                image->push_back(DataStore[CurDataPos++]);
                image->push_back(DataStore[CurDataPos++]);
            }
            CurDataPos += ExcessPitch;
        }

        delete[] DataStore;
    }
    else
    {

        cout << " invalid bit count " << Filename << endl;
    }
    fclose(file);
}