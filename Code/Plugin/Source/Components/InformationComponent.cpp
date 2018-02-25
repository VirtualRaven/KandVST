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
    __version.setText("Version X 2018", NotificationType::dontSendNotification);

    addAndMakeVisible(__authors);
    __authors.setFont(f);
    __authors.setText("Anton, Andreas, Klas, Lukas, Sarosh, Stas", NotificationType::dontSendNotification);

	// variables
	x = 310; //starting x position
	y = 150; //starting y position
	midX = 500;
	midY = 225;
	w = 380; //width of component
	h = 200; //height of component

	// shadow stuff
	startAlpha = 0.3f;
	endAlpha = 0.05f;
	offset = 10;
}


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

}
void InformationComponent::resized(){
    //set the bounds of any child components here.
    Rectangle<int> r (x,y,w,h);
	r.removeFromTop(60);
    __version.setBounds(r);
	__version.setCentrePosition(midX + 10.0f, r.getY());
    r.removeFromTop(40);
    __authors.setBounds(r);
    __authors.setCentrePosition(midX + 10.0f, r.getY());
}
