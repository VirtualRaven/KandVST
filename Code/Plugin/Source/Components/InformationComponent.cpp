#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"

InformationComponent::~InformationComponent()
{
}

InformationComponent::InformationComponent(){
    
	Font f;
	f.setHeight(20.0f);

	addAndMakeVisible(__version);
    __version.setFont(f);
    __version.setText("Version 0.1  2018", NotificationType::dontSendNotification);

    addAndMakeVisible(__authors);
    __authors.setFont(f);
    __authors.setText("Anton, Andreas, Klas \nLukas, Sarosh, Stas", NotificationType::dontSendNotification);
}

// variables
int x = 450; //starting x position
int y = 150; //starting y position
int midX = 640;
int midY = 225;
int w = 380; //width of component
int h = 200; //height of component

float startAlpha = 0.3f;
float endAlpha = 0.05f;
int offset = 10;

void InformationComponent::paint (Graphics& g){
    
    //Path to draw the rectangle
    Path myPath;
    Rectangle<int> infoWindow(x,y,w,h);
	myPath.addRectangle(infoWindow);
    g.setColour(Colour::fromRGB(43,68,78));
    g.fillPath(myPath);

    // == adding inner shadows using gradients == 
    //shadow for top
    ColourGradient shadowTop(Colours::black.withAlpha(startAlpha), midX, y,
                    Colours::black.withAlpha(endAlpha), midX, y+offset, false);
                    
    g.setGradientFill(shadowTop);
    g.fillPath(myPath);

    //shadow for bottom
    ColourGradient shadowBottom(Colours::black.withAlpha(startAlpha), midX, y+h,
                    Colours::black.withAlpha(endAlpha), midX, ((y+h)-offset) , false);
    g.setGradientFill(shadowBottom);
    g.fillPath(myPath);

    //shadow for left
    ColourGradient shadowLeft(Colours::black.withAlpha(startAlpha), x, midY,
                    Colours::black.withAlpha(endAlpha), x+offset, midY, false);
    g.setGradientFill(shadowLeft);
    g.fillPath(myPath);

    //shadow for right
    ColourGradient shadowRight(Colours::black.withAlpha(startAlpha), x+w, midY,
                    Colours::black.withAlpha(endAlpha), ((x+w)-offset), midY, false);
    g.setGradientFill(shadowRight);
    g.fillPath(myPath);


    g.setColour(Colours::black);
    g.strokePath(myPath,PathStrokeType (0.8f));

    // TEXT
    // == Title
    g.setColour(Colours::silver);
    g.setFont(30.0f);
    g.drawText("KANDVST",Rectangle<int>(x+10,y,w,50),Justification::centred,false);


}
void InformationComponent::resized(){
    //set the bounds of any child components here.
    Rectangle<int> r (x,y,w,h);
    r.removeFromTop(60);
    __version.setBounds(r);
    __version.setCentrePosition(midX,r.getY()+5.0f);
    r.removeFromTop(40);
    __authors.setBounds(r);
    __authors.setCentrePosition(midX,r.getY()+5.0f);
}
