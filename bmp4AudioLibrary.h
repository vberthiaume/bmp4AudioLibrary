//-------------------------------------------------------------------------------------------------------
// Filename     : bmp4AudioLibrary.h
// Created by   : BMP4 (Vincent Berthiaume)
// Description  : Audio library
//
// © 2014, Vincent Berthiaume, All Rights Reserved
//-------------------------------------------------------------------------------------------------------	
#include "math.h"

//-------------------------------------------------------------------------------------------------------	
//PROTOTYPES

//template function, returns 1 for positive numbers and -1 for negative numbers
template <class T> T sign(T &v);

//template function used to apply overdrive on single channels
template<class T> void overdriveSingleChannel(T &p_in, T &p_out, T &p_fGain);

//template function used to apply distortion on single channels
template<class T> void distortionSingleChannel(T &p_in, T &p_out, T &p_fDist, T &p_fGain);
//static void distortionSingleChannel(double &p_in, double &p_out, double &p_fDist, double &p_fGain);
	
//template function used to bypass single channels, can be used for testing
template<class T> void bypassSingleChannel(T &p_in, T &p_out);

//-------------------------------------------------------------------------------------------------------	
//DEFINITIONS

//****************** overdrive as per p. 142 DAFX 2ed ******************
template<class T> void overdriveSingleChannel(T &p_in, T &p_out, T &p_fGain)
{
	//safely cast parameters to avoid problems	
	T gain = static_cast<T> (p_fGain);
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
template<class T> void distortionSingleChannel(T &p_in, T &p_out, T &p_fDist, T &p_fGain) {
	
	//clip limit
	T clipLimit = .999f;
		
	//mix is simply the distortion param 
	T mix = static_cast<T> (p_fDist);	

	//amplified original sample 
	T q	= p_in * static_cast<T> (p_fGain);

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

//****************** distortion as per p. 144 DAFX 2ed ******************
//void distortionSingleChannel(double p_in, double p_out, double p_fDist, double p_fGain) {
//	
//	//clip limit
//	double clipLimit = .999f;
//		
//	//mix is simply the distortion param 
//	double mix = static_cast<double> (p_fDist);	
//
//	//amplified original sample 
//	double q	= p_in * static_cast<double> (p_fGain);
//
//	//distorted sample
//	double distortedSample = sign(q)*(1-exp(-abs(q)));
//
//	//output is combination of distorted and original samples
//	p_out = mix * distortedSample + (1-mix) * q;
//
//	//so this works, DAWs do not clip, but the problem is that we're handling some of the distortion here too... 
//	//so lowering the gain removes some of the distortion instead of just making the volume softer. 
//	//what we'd need is do something like detect the clip then proportionally reduce the gain of the two elements above
//	if (abs(p_out) > clipLimit){
//		if (p_out > 0) p_out = clipLimit; 
//		if (p_out < 0) p_out = clipLimit;
//	}				
//}

//template<class T1, class T2> void distortionSingleChannel(T1 &p_in, T1 &p_out, T2 &p_fDist, T2 &p_fGain) {
//	
//	//clip limit
//	T1 clipLimit = .999f;
//		
//	//mix is simply the distortion param 
//	T1 mix = static_cast<T1> (m_fDist);	
//
//	//amplified original sample 
//	T1 q	= p_in * static_cast<T1> (p_fGain);
//
//	//distorted sample
//	T1 distortedSample = sign(q)*(1-exp(-abs(q)));
//
//	//output is combination of distorted and original samples
//	p_out = mix * distortedSample + (1-mix) * q;
//
//	//so this works, DAWs do not clip, but the problem is that we're handling some of the distortion here too... 
//	//so lowering the gain removes some of the distortion instead of just making the volume softer. 
//	//what we'd need is do something like detect the clip then proportionally reduce the gain of the two elements above
//	if (abs(p_out) > clipLimit){
//		if (p_out > 0) p_out = clipLimit; 
//		if (p_out < 0) p_out = clipLimit;
//	}				
//}

template<class T> void bypassSingleChannel(T &input, T &output){
		output = input;
}

template<class T> T sign(T &v)
{
	return v > 0 ? 1.f : (v < 0 ? -1.f : 0.f);
}

