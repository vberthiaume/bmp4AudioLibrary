//-------------------------------------------------------------------------------------------------------
// Filename     : bmp4AudioLibrary.h
// Created by   : BMP4 (Vincent Berthiaume)
// Description  : Audio library
//
// © 2014, Vincent Berthiaume, All Rights Reserved
//-------------------------------------------------------------------------------------------------------	



class bmp4
{
public:

	//template function, returns 1 for positive numbers and -1 for negative numbers
	template <class T> static T sign(T &v);

	//template function used to apply overdrive on single channels
	template<class T> static void overdriveSingleChannel(T &p_in, T &p_out);

	//template function used to apply distortion on single channels
	template<class T> static void distortionSingleChannel(T &p_in, T &p_out);
	
	//template function used to bypass single channels, can be used for testing
	template<class T> static void bypassSingleChannel(T &p_in, T &p_out);
};
