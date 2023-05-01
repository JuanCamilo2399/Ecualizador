/*
  ==============================================================================

    Filtro.h
    Created: 30 Mar 2023 4:43:12pm
    Author:  Wateo

  ==============================================================================
*/

#pragma once
#include <cmath>
#include <JuceHeader.h>
class Filtro
{
public:
    Filtro()
    {
        b0 = 0; b1 = 0; b2 = 0; a1 = 0; a2 = 0;
        xn1 = 0; xn2 = 0; yn1 = 0; yn2 = 0;
        filterType = 0; // Pasabajos
        Fs = 44.1e3;
        Fc = 1e3;
        Q = 0.707;
        V0 = 1.1220;
        G = 1.0f;
    }
    void setFs(float newFs) { 
        Fs = newFs; 
        updateFilterCoeff();
    }

    void setGain(float Gain)
    {
        G = Gain;
        float linear = Gain / 20;
        V0 = std::pow(10, linear);
        updateFilterCoeff();
    }


    void setFc(float newFc) { Fc = newFc; 
    updateFilterCoeff();
    }
    void setQ(float nQ) { Q = nQ; 
    updateFilterCoeff();
    }
    void setFilterType(int newFilterType) { filterType = newFilterType; updateFilterCoeff();
    }

    void updateFilterCoeff()
    {
        float thetac = (2 * juce::double_Pi * Fc) / Fs;
        float d = 1 / Q;
        float sinen = (d / 2) * sin(thetac);
        float Beta = 0.5 * ((1 - sinen) / (1 + sinen));
        float gamma = (0.5 + Beta) * cos(thetac);

        float Kbell = std::tanf((juce::double_Pi * Fc) / Fs);
        float d0 = 1 + ((1 / Q) * Kbell) + std::pow(Kbell,2);
        float e0 = 1 + ((1 / (Q * V0)) * Kbell) + std::pow(Kbell, 2);


        float alphabell = 1 + (V0/Q * Kbell) + std::pow(Kbell, 2);
        float Betabell = 2 * ((std::pow(Kbell, 2)) - 1);
        float gammabell = 1 - (V0 / Q) * Kbell + std::pow(Kbell, 2);
        float deltabell = 1 - (1 / Q) * Kbell + std::pow(Kbell, 2);
        float etabell = 1 - (1 / (V0 * Q) * Kbell) + std::pow(Kbell, 2);

        float Kshelf = std::tanf((juce::double_Pi * Fc) / Fs);
        float root2 = 1 / Q;

        switch (filterType)
        {
        case 0: // Pasabajos
            b0 = (0.5+Beta-gamma)/2.0;
            b1 = (0.5+Beta-gamma);
            b2 = (0.5+Beta-gamma)/2.0;
            a1 = -2*gamma;
            a2 = 2*Beta;
            break;
        case 1: // Pasa Altos
            b0 = (0.5 + Beta + gamma) / 2.0f;
            b1 = -(0.5 + Beta + gamma);
            b2 = (0.5 + Beta + gamma) / 2.0f;
            a1 = -2 * gamma;
            a2 = 2 * Beta;
            break;
        case 2: // Bell

            if (G > 0)
            {
                b0 = alphabell / d0;
                b1 = Betabell / d0;
                b2 = gammabell / d0;
                a1 = b1;
                a2 = deltabell / d0;
            }

            else if (G < 0)
            {
                b0 = d0 / e0;
                b1 = Betabell / e0;
                b2 = deltabell / e0;
                a1 = Betabell / e0;
                a2 = etabell / e0;
            }

    
            break;
        default:
            break;
        }
    }
    float procesSample(float xn)
    {
        
        float yn = b0 * xn + b1 * xn1 + b2 * xn2 - a1 * yn1 - a2 * yn2;
        xn2 = xn1;
        xn1 = xn;
        yn2 = yn1;
        yn1 = yn;

        return yn;
    }


private:
    int filterType;
    float Fs;
    float Fc;
    float Q,V0, G;
    float b0, b1, b2, a1, a2;
    float xn1, xn2, yn1, yn2;
}; 