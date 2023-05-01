/*
  ==============================================================================

    FrequencySpectrumClass.h
    Created: 26 Mar 2020 5:31:28pm
    Author:  WATEO

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FrequencySpectrumClass    : public Component
{
public:
    FrequencySpectrumClass()
    {

    }

    ~FrequencySpectrumClass()
    {
    }

    void paint (Graphics& g) override
    {
		DBG("H");
		g.fillAll(Colour(Colours::white));

		Path wavePath;
		wavePath.startNewSubPath(0.0f, getHeight()/2);
		if (init)
		{
			for (auto i = 0; i < getWidth(); ++i)
			{
				wavePath.lineTo (i, getHeight() / 2);
			}
		}
		else
		{
			for (auto i = 0; i < getWidth(); ++i)
			{
				wavePath.lineTo (i, (getHeight() / 2) - (data[i] * getHeight()/2));
			}
		}
		g.setColour(Colours::black);
		g.strokePath(wavePath, PathStrokeType(1.0));
    }

    void resized() override
    {

    }

	void receiveArray(float* waveForm [], int size)
	{	
		data = *waveForm;
		arraySize = size;
		init = false;
		repaint();
	}

private:
	float* data;
	int arraySize;
	bool init = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencySpectrumClass)
};
