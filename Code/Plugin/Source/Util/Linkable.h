#ifndef LINKABLE_H
#define LINKABLE_H
#include "JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "PresetManager.h"
class Linkable : private MouseListener, private ChangeListener
{
private:
	int __linkedTo;
	std::vector<AudioProcessorParameter *> __linkParameters;
	virtual void mouseUp(const MouseEvent& event) override;
public:
	Linkable(AudioProcessorParameter* param);
	Linkable(std::vector<AudioProcessorParameter *> params);
	~Linkable();

	bool isLinked();
	void Link(int id);
	void Unlink();
	virtual void LinkCouldHaveChanged() = 0;

	virtual void changeListenerCallback(ChangeBroadcaster * source) override;

};
#endif // !LINKABLE_H
