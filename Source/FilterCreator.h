/*
  ==============================================================================

    FilterCreator.h
    Created: 14 Apr 2023 9:47:52pm
    Author:  JUAN CAMILO

  ==============================================================================
*/

#pragma once
#include <cmath>
#include <JuceHeader.h>
#include "Filtro.h"
class FilterCreator : public Filtro
{ 
public:
    //Constructor
    FilterCreator(float newFs, float newFc, float newQ, float Gain, int Slope, int numfilters) 
        : Fs(newFs), Fc(newFc), Q(newQ), V0(Gain), slope(Slope) 
    { 
        slope = 1;
        Bypass = false;
        filters.resize(numfilters);
        for (int i = 0; i < filters.size(); i++)
        {
            Filtro filter;
            filter.setFs(Fs);
            filter.setFc(Fc);
            filter.setQ(Q);
            filter.setGain(V0);
            filter.setFilterType(0);


            filters[i] = filter;
        }
    }

   

    void CreateFilter(int filterType, int numfilters);
    void updateSlope(int newSlope);
    float processSample(float sample);
    void updateParameters(float params, juce::String Id);
    void OnOffEq(bool State);

private:
    Filtro filter;
    std::vector<Filtro> filters;
    int slope;
    double Fs;
    double Fc;
    double Q;
    double V0;
    bool Bypass;

};