//-------------------------------------------------------------------------------------------------------
// Filename     : bmp4AudioLibrary.cpp
// Created by   : BMP4 (Vincent Berthiaume)
// Description  : Audio library
//
// © 2014, Vincent Berthiaume, All Rights Reserved
//-------------------------------------------------------------------------------------------------------	

#include "bmp4AudioLibrary.h"

//****************** overdrive as per p. 142 DAFX 2ed ******************
template<class T> void bmp4::overdriveSingleChannel(T &p_in, T &p_out)
{
	//safely cast parameters to avoid problems	
	T gain = static_cast<T> (m_fGain);
	T dist = 1.0;//static_cast<T> (m_fDist);

	//mix goes with the gain param for now, but need to implement its own parameter
	//T mix = gain;
	
	//save original frame value
	T originalFrame = p_in * gain;
		      
	// UNDER THRESHOLD
	if (abs(originalFrame) < dist){			
		p_out = gain * 2 * (originalFrame);
	}
		
	// BIGGER THAN 2X TRESHOLD 
	else if (abs(originalFrame) > 2*dist){	
		if (originalFrame > 0) p_out = gain * 1;
		if (originalFrame < 0) p_out = gain * -1;
	}

	//BIGGER THAN THRESHOLD, SMALLER THAN 2X THRESHOLD
	else {							
		if (originalFrame > 0) p_out = gain *  ( 3- pow((2-originalFrame*3), 2) )/3;
		if (originalFrame < 0) p_out = gain * -( 3- pow((2-abs(originalFrame)*3), 2) )/3;
	}

	//p_out = mix * p_out + (1-mix)* originalFrame;
}

//****************** distortion as per p. 144 DAFX 2ed ******************
template<class T> void bmp4::distortionSingleChannel(T &p_in, T &p_out) {
	
	//clip limit
	T clipLimit = .999f;
		
	//mix is simply the distortion param 
	T mix = static_cast<T> (m_fDist);	

	//amplified original sample 
	T q	= p_in * static_cast<T> (m_fGain);

	//distorted sample
	T distortedSample = sign(q)*(1-exp(-abs(q)));

	//output is combination of distorted and original samples
	p_out = mix * distortedSample + (1-mix) * q;


	//so this works, DAWs do not clip, but the problem is that we're handling some of the distortion here too... 
	//so lowering the gain removes some of the distortion instead of just making the volume softer. 
	//what we'd need is do something like detect the clip then proportionally reduce the gain of the two elements above
	if (abs(p_out) > clipLimit){
		if (p_out > 0) p_out = clipLimit; 
		if (p_out < 0) p_out = clipLimit;
	}
				
}

template<class T> void bmp4::bypassSingleChannel(T &input, T &output){
		output = input;
}

template<class T> T bmp4::sign(T &v)
{
	return v > 0 ? 1.f : (v < 0 ? -1.f : 0.f);
}
