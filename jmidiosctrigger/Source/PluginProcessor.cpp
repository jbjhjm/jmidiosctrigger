/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JMidiOscTriggerAudioProcessor::JMidiOscTriggerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
     ),
	logger (StatusLog::getInstance())
#endif
{

}

JMidiOscTriggerAudioProcessor::~JMidiOscTriggerAudioProcessor()
{
	OSCHandler::getInstance().disconnect();
}

//==============================================================================
const juce::String JMidiOscTriggerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JMidiOscTriggerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JMidiOscTriggerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JMidiOscTriggerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JMidiOscTriggerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JMidiOscTriggerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JMidiOscTriggerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JMidiOscTriggerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JMidiOscTriggerAudioProcessor::getProgramName (int index)
{
    return {};
}

void JMidiOscTriggerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JMidiOscTriggerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JMidiOscTriggerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JMidiOscTriggerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    juce::ignoreUnused (layouts);
    return true;
}
#endif

void JMidiOscTriggerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int time;
    juce::MidiMessage m;
    juce::String searchString;
	bool isMidiMessageInWatchedRange = true; 
	bool midiMessageHasOscTarget = true; 
	pugi::xml_node xmlEntry;

	// auto numNotes = midiMessages.getNumEvents();
	// if(numNotes > 0) {
	// 	logger.log("Handle incoming midi events " + juce::String(numNotes));
	// }

    for (juce::MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
    {
		//logger.logMidiMessage(m, "processBlock");
        // processMidiInputMessage(m, midiOutput);

		// efficient pre-check: only allow noteon events & certain midi regions
		if(!m.isNoteOn(true)) continue;
		if(m.getNoteNumber() > maxNote && m.getChannel() != extraChannel) continue;
		
		if(isMidiMessageInWatchedRange) {

			midiMessageHasOscTarget = XMLReader::getInstance().parser->findEntryforMidiEvent(m, xmlEntry);

			if(midiMessageHasOscTarget) {
				logger.log("Found midiMessage with OSC target [channel=" + juce::String(m.getChannel()) +"]" + " [key=" + juce::String(m.getNoteNumber())+"]");
				
				midiMessages.clear(time,0);
				OSCHandler::getInstance().sendOSC(xmlEntry, m);
				continue;
			}
		}
		// logger.log("midiMessage has no OSC target [channel=" + juce::String(m.getChannel()) +"]" + " [key=" + juce::String(m.getNoteNumber())+"]");

    }

}



//==============================================================================
bool JMidiOscTriggerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JMidiOscTriggerAudioProcessor::createEditor()
{
    return new JMidiOscTriggerAudioProcessorEditor (*this);
}

//=== Load/Save persisted data ===========================================================================

void JMidiOscTriggerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto& configState = Store::getState(STATES::Config);
    //DBG(configState.toXmlString());
    //DBG(configState.getProperty(CONFIGPROPS::FilePath).toString());
    std::unique_ptr<juce::XmlElement> configStateAsXml (configState.createXml());
    DBG(configStateAsXml->toString());
    //configStateAsXml->writeToFile()
    //logger.log("persisting State Information.");
    copyXmlToBinary(*configStateAsXml, destData);
	refresh();
}

void JMidiOscTriggerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // getXmlFromBinary returns a unique_ptr. Need to convert it to a regular pointer to allow passing data to importToState.
    auto configStateAsXml = getXmlFromBinary(data, sizeInBytes);
    if (configStateAsXml.get() == nullptr) return;
    juce::XmlElement* regularXmlPointer = configStateAsXml.release();

    auto configState = Store::importToState(STATES::Config, regularXmlPointer);

    delete regularXmlPointer;
	
	initialized = true;
	logger.log("initialized State Information.");

	refresh();

}


//==============================================================================


bool JMidiOscTriggerAudioProcessor::readXmlFileAccordingToState()
{
    auto& configState = Store::getState(STATES::Config);
	auto filePath = configState.getProperty(CONFIGPROPS::FilePath);
	if(filePath.isString()) {
		return XMLReader::getInstance().loadXmlFile(filePath.toString());
    }
    else {
        // logger.log("loaded State Information - no config file path found.");
		return false;
    }
}

bool JMidiOscTriggerAudioProcessor::refresh()
{
	if(!initialized) return false;

	OSCHandler::getInstance().disconnect();
    readXmlFileAccordingToState();

	auto& configState = Store::getState(STATES::Config);

	maxNote = 127;
	if(configState.hasProperty(CONFIGPROPS::FilterMaxNote)) {
		auto* var = &configState.getProperty(CONFIGPROPS::FilterMaxNote);
		if(var->isInt()) maxNote = static_cast<int>(*var);
	}

	extraChannel = -1;
	if(configState.hasProperty(CONFIGPROPS::FilterExtraChannel)) {
		auto* var = &configState.getProperty(CONFIGPROPS::FilterExtraChannel);
		if(var->isInt()) extraChannel = static_cast<int>(*var);
	}

	OSCHandler::getInstance().connect();
	return true;
}






//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JMidiOscTriggerAudioProcessor();
}
