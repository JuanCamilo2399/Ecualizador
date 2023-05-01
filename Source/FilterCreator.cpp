/*
  ==============================================================================

    FilterCreator.cpp
    Created: 14 Apr 2023 9:47:52pm
    Author:  JUAN CAMILO

  ==============================================================================
*/

#include "FilterCreator.h"

void FilterCreator::CreateFilter(int filterType, int numfilters)
{ 
    filters.resize(numfilters);
    for (int i = 0; i < filters.size(); i++)
    {
        Filtro filter;
        filter.setFs(Fs);
        filter.setFc(Fc);
        filter.setQ(Q);
        filter.setGain(V0);
        filter.setFilterType(filterType);


        filters[i] = filter;
    }
    
}

void FilterCreator::updateSlope(int newSlope)
{
    slope = newSlope;
}

float FilterCreator::processSample(float sample)
{
    float Nsample = sample;
    
    if (Bypass == true)
    {
        switch (slope)
        {
        case 0:
            for (int i = 0; i < 1; i++)
            {
                Nsample = filters[i].procesSample(Nsample);
            }
            break;
        case 1:
            for (int i = 0; i < 2; i++)
            {
                Nsample = filters[i].procesSample(Nsample);
            }
            break;
        case 2:
            for (int i = 0; i < 3; i++)
            {
                Nsample = filters[i].procesSample(Nsample);
            }
            break;
        default:
            break;
        }
    }
    else if (Bypass == false) 
    {
        return Nsample;
    }


    
}

void FilterCreator::updateParameters(float params, juce::String Id)
{
    if (Id == "FC" || Id == "Fc" || Id == "fc")
    {
        for (int i = 0; i < filters.size(); i++)
        {
            filters[i].setFc(params);
        }
    }
    else if (Id == "Fs" || Id == "FS" || Id == "fs")
    {
        for (int i = 0; i < filters.size(); i++)
        {
            filters[i].setFs(params);
        }
    }
    else if (Id == "V0" || Id == "v0")
    {

        for (int i = 0; i < filters.size(); i++)
        {
            filters[i].setGain(params);
        }
    }
    else if (Id == "Q" || Id == "q")
    {
        for (int i = 0; i < filters.size(); i++)
        {
            filters[i].setQ(params);
        }
    }
}

void FilterCreator::OnOffEq(bool State)
{
    Bypass = State;
}

