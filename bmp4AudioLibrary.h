//-------------------------------------------------------------------------------------------------------
// Filename     : bmp4AudioLibrary.h
// Created by   : BMP4 (Vincent Berthiaume)
// Description  : Audio library
//
// © 2014, Vincent Berthiaume, All Rights Reserved
//-------------------------------------------------------------------------------------------------------	
#ifndef __bmp4audiolibrary__
#define __bmp4audiolibrary__

#include "math.h"

//-------------------------------------------------------------------------------------------------------	
//PROTOTYPES
//template<class T> class bmp4;

template<class T> 
class bmp4{
public:
	bmp4();
	
	
	void overdriveMono(T &p_in, T &p_out, T &p_fGain);						//template function used to apply overdrive on single channels
	void overdriveStereo(T &p_in1, T &p_out1, T &p_in2, T &p_out2, float &p_fGain);
	
	void distortionMono(T &p_in, T &p_out, float &p_fDist, float &p_fGain); //template function used to apply distortion on single channels
	void distortionStereo(T &p_in1, T &p_out1, T &p_in2, T &p_out2, float &p_fDist, float &p_fGain);

	void delayMonoInput(T &p_in, T &p_out, T &p_out2);
		
	void bypassSingleChannel(T &p_in, T &p_out);										//template function used to bypass single channels, can be used for testing
	T sign(T &v);																	//template function, returns 1 for positive numbers and -1 for negative numbers
	
private:
	T* buffer;
	float fFeedBack;
	long delay;
	long cursor;

};

//-------------------------------------------------------------------------------------------------------	
//DEFINITIONS
template<class T>
bmp4<T>::bmp4(): cursor(0), delay(22000), fFeedBack(.5)
{
	int size = 44100;
	buffer = new T[size];
	memset (buffer, 0, size * sizeof (T));
	//buffer[44100] = {0};
}


//****************** overdrive as per p. 142 DAFX 2ed ******************
template<class T> 
void bmp4<T>::overdriveMono(T &p_in, T &p_out, T &p_fGain)
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

template<class T> 
void bmp4<T>::overdriveStereo(T &p_in1, T &p_out1, T &p_in2, T &p_out2, float &p_fGain) {
	overdriveMono(p_in1, p_out1, p_fDist, p_fGain);
	overdriveMono(p_in2, p_out2, p_fDist, p_fGain);
}

//****************** distortion as per p. 144 DAFX 2ed ******************
template<class T> 
void bmp4<T>::distortionMono(T &p_in, T &p_out, float &p_fDist, float &p_fGain) {
	
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

template<class T> 
void bmp4<T>::distortionStereo(T &p_in1, T &p_out1, T &p_in2, T &p_out2, float &p_fDist, float &p_fGain) {
	distortionMono(p_in1, p_out1, p_fDist, p_fGain);
	distortionMono(p_in2, p_out2, p_fDist, p_fGain);
}


template<class T> 
void bmp4<T>::delayMonoInput(T &p_in, T &p_out1, T &p_out2){

	T new_sample = p_in;
	T old_sample = buffer[cursor];

	buffer[cursor++] = new_sample + old_sample * fFeedBack;

	if (cursor >= delay){
		cursor = 0;
	}
	p_out1 = old_sample;
	p_out2 = old_sample;
}

template<class T> 
void bmp4<T>::bypassSingleChannel(T &input, T &output){
		output = input;
}

template<class T> 
T bmp4<T>::sign(T &v)
{
	return v > 0 ? 1.f : (v < 0 ? -1.f : 0.f);
}



#endif