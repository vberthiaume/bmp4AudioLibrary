//-------------------------------------------------------------------------------------------------------
// Filename     : bmp4AudioLibrary.h
// Created by   : BMP4 (Vincent Berthiaume)
// Description  : Audio library
//
// © 2014, Vincent Berthiaume, All Rights Reserved
//-------------------------------------------------------------------------------------------------------	


namespace bmp4
{
	class bmp4
	{
	public:
		template <class T> T sign(T &v);

		//template function used to apply overdrive on single channels
		template<class T> void overdriveSingleChannel(T &p_in, T &p_out);
		template<class T> void distortionSingleChannel(T &p_in, T &p_out);
	
		template<class T> void bypassSingleChannel(T &p_in, T &p_out);
	};
}