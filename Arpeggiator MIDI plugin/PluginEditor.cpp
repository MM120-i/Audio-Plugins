
/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

A1AudioProcessorEditor::A1AudioProcessorEditor(A1AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set the window size
    setSize(460, 405);
    juce::Colour buttonColour = juce::Colour::fromRGB(13, 195, 199); // cyan

    // Setup GUI components
    setupAdditionalModesToggle();
    setupNoteDisplayLabel();
    setupArpSlider();
    setupAscendingButton(buttonColour);
    setupDescendingButton(buttonColour);
    setupRandomButton(buttonColour);
    setupAlternativeButton(buttonColour);
    setupRotarySlider();
    setupOctaveComboBox();
    startTimer(100);
}

void A1AudioProcessorEditor::setupAdditionalModesToggle()
{
    additionalModesToggle.setButtonText("");

    additionalModesToggle.onClick = [this] {

        bool isChecked = additionalModesToggle.getToggleState();

        random.setEnabled(isChecked);
        alternative.setEnabled(isChecked);

        if (!isChecked) {
            audioProcessor.isAlternative = false;
            audioProcessor.isRandom = false;
            random.setToggleState(false, juce::dontSendNotification);
            alternative.setToggleState(false, juce::dontSendNotification);
        }
    };

    addAndMakeVisible(additionalModesToggle);
}


void A1AudioProcessorEditor::setupNoteDisplayLabel()
{
    noteDisplayLabel.setFont(juce::Font(20.0f));
    noteDisplayLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    noteDisplayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(noteDisplayLabel);
}

void A1AudioProcessorEditor::setupArpSlider()
{
    arpSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    arpSlider.setRange(0.0, 1.0, 0.05);
    arpSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(0, 255, 255));
    arpSlider.setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(0, 200, 255));
    arpSlider.setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGB(50, 50, 50));
    arpSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    arpSlider.setPopupDisplayEnabled(true, false, this);
    arpSlider.setTextValueSuffix(" Speed");
    arpSlider.setValue(0.5);
    arpSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    arpSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(30, 30, 30));
    addAndMakeVisible(&arpSlider);
    arpSlider.addListener(this);
}

void A1AudioProcessorEditor::setupAscendingButton(juce::Colour buttonColour)
{
    ascendingButton.onClick = [this] {
        isAscendingSelected = !isAscendingSelected;

        if (isAscendingSelected) {
            ascendingButton.setEnabled(false);
            descendingButton.setEnabled(true);

            audioProcessor.isAscending = true;
            audioProcessor.isDescending = false;
        }
        else {
            ascendingButton.setEnabled(true);
            audioProcessor.isAscending = false;
        }

        repaint();
    };

    ascendingButton.setColour(juce::TextButton::buttonColourId, buttonColour);
    addAndMakeVisible(&ascendingButton);
    ascendingButton.addListener(this);
}


void A1AudioProcessorEditor::setupDescendingButton(juce::Colour buttonColour)
{
    descendingButton.onClick = [this] {
        isDescendingSelected = !isDescendingSelected;

        if (isDescendingSelected) {
            descendingButton.setEnabled(false);
            ascendingButton.setEnabled(true);
            audioProcessor.isDescending = true;
            audioProcessor.isAscending = false;
        }
        else {
            descendingButton.setEnabled(true);
            audioProcessor.isDescending = false;
        }

        repaint();
    };

    descendingButton.setColour(juce::TextButton::buttonColourId, buttonColour);
    addAndMakeVisible(&descendingButton);
    descendingButton.addListener(this);
}


void A1AudioProcessorEditor::setupRandomButton(juce::Colour buttonColour)
{
    random.onClick = [this] {
        isRandomSelected = !isRandomSelected;

        if (isRandomSelected) {
            random.setEnabled(false);
            ascendingButton.setEnabled(true);
            descendingButton.setEnabled(true);
            alternative.setEnabled(true);
            audioProcessor.isRandom = true;
            audioProcessor.isAscending = false;
            audioProcessor.isDescending = false;
            audioProcessor.isAlternative = false;
        }
        else {
            random.setEnabled(true);
            audioProcessor.isRandom = false;
        }

        repaint();
    };

    random.setEnabled(false);
    random.setColour(juce::TextButton::buttonColourId, buttonColour);
    addAndMakeVisible(random);
}

void A1AudioProcessorEditor::setupAlternativeButton(juce::Colour buttonColour)
{
    alternative.onClick = [this] {
        isAlternativeSelected = !isAlternativeSelected;

        if (isAlternativeSelected) {
            alternative.setEnabled(false);
            random.setEnabled(true);
            ascendingButton.setEnabled(true);
            descendingButton.setEnabled(true);
            audioProcessor.isAlternative = true;
            audioProcessor.isRandom = false;
            audioProcessor.isAscending = false;
            audioProcessor.isDescending = false;
        }
        else {
            alternative.setEnabled(true);
            audioProcessor.isAlternative = false;
        }

        repaint();

    };

    alternative.setEnabled(false);
    alternative.setColour(juce::TextButton::buttonColourId, buttonColour);
    addAndMakeVisible(alternative);
}

void A1AudioProcessorEditor::setupRotarySlider()
{
    // rotary Slider 1:
    rotarySlider.setSliderStyle(juce::Slider::Rotary);
    rotarySlider.setRange(1.0, 5.0, 0.50);
    rotarySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rotarySlider.setPopupDisplayEnabled(false, false, this);
    rotarySlider.setValue(1.0);
    rotarySlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB(0, 255, 255));
    rotarySlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour::fromRGB(0, 150, 150));
    rotarySlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(0, 255, 255));
    rotarySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    rotarySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(30, 30, 30));
    addAndMakeVisible(rotarySlider);
    rotarySlider.addListener(this);
    juce::Colour neonGreen = juce::Colour::fromRGB(57, 255, 20); // Neon green

    // rotary slider 2
    rotaryValueLabel2.setText("Initial Burst Time: ", juce::dontSendNotification);
    rotaryValueLabel2.setColour(juce::Label::textColourId, neonGreen);
    rotaryValueLabel2.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rotaryValueLabel2);
    rotarySlider2.setSliderStyle(juce::Slider::Rotary);
    rotarySlider2.setRange(1.0, 5.0, 0.50);
    rotarySlider2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rotarySlider2.setPopupDisplayEnabled(false, false, this);
    rotarySlider2.setValue(1.0);
    rotarySlider2.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB(0, 255, 255));
    rotarySlider2.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour::fromRGB(0, 150, 150));
    rotarySlider2.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(0, 255, 255));
    rotarySlider2.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    rotarySlider2.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(30, 30, 30));
    addAndMakeVisible(rotarySlider2);
    rotarySlider2.addListener(this);

    // rotary slider 3
    rotaryValueLabel3.setText("Final Note Duration: ", juce::dontSendNotification);
    rotaryValueLabel3.setColour(juce::Label::textColourId, neonGreen);
    rotaryValueLabel3.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rotaryValueLabel3);

    rotarySlider3.setSliderStyle(juce::Slider::Rotary);
    rotarySlider3.setRange(1.0, 5.0, 0.50);
    rotarySlider3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rotarySlider3.setPopupDisplayEnabled(false, false, this);
    rotarySlider3.setValue(1.0);
    rotarySlider3.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB(0, 255, 255));
    rotarySlider3.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour::fromRGB(0, 150, 150));
    rotarySlider3.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(0, 255, 255));
    rotarySlider3.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    rotarySlider3.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(30, 30, 30));
    addAndMakeVisible(rotarySlider3);
    rotarySlider3.addListener(this);
}

void A1AudioProcessorEditor::setupOctaveComboBox()
{
    for (size_t i = 0; i <= NUMBER_OF_OCTAVES; i++) 
        octaveComboBox.addItem(juce::String(i), i);

    octaveComboBox.setColour(juce::ComboBox::backgroundColourId, juce::Colour::fromRGB(0, 30, 30));
    octaveComboBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
    octaveComboBox.setColour(juce::ComboBox::outlineColourId, juce::Colour::fromRGB(0, 200, 200));

    addAndMakeVisible(octaveComboBox);

    octaveComboBox.onChange = [this] {
        audioProcessor.numOctaves = octaveComboBox.getSelectedId();
    };
}

// Destructor
A1AudioProcessorEditor::~A1AudioProcessorEditor()
{
    ascendingButton.removeListener(this);
    descendingButton.removeListener(this);
    random.removeListener(this);
    alternative.removeListener(this);

    stopTimer();
}

// Paint function for drawing
void A1AudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::ColourGradient gradient(juce::Colours::transparentBlack, 0, 0, juce::Colours::darkcyan, getWidth(), getHeight(), false);
    g.setGradientFill(gradient);
    g.fillAll();

    juce::Colour neonColor = juce::Colour::fromRGB(57, 255, 20); // Neon green

    // Arpeggiator label:
    g.setColour(neonColor);
    g.setFont(14.5f);
    g.drawFittedText("Arpeggiator", 10, 0, getWidth(), 30, juce::Justification::centredLeft, 1);

    // Octaves label:
    g.setColour(neonColor);
    g.setFont(15.0f);
    g.drawFittedText("Octaves:", 195, 25, 100, 30, juce::Justification::centred, 1);

    // Additional Modes Label
    g.setColour(neonColor);
    g.setFont(15.0f);
    g.drawFittedText("Additional Modes:", 80, 243, 100, 30, juce::Justification::centred, 1);

    // Modes Label:
    g.setColour(neonColor);
    g.setFont(15.0f);
    g.drawFittedText("Modes:", 80, 25, 100, 30, juce::Justification::centred, 1);

    // Duration Label:
    g.setColour(neonColor);
    g.setFont(15.0f);
    g.drawFittedText("Long Duration:", 220, 200, 120, 120, juce::Justification::centred, 1);
}

// Resize and arrange components
void A1AudioProcessorEditor::resized()
{
    // Position slider
    arpSlider.setBounds(40, 30, 20, getHeight() - 60);

    // Position the buttons
    ascendingButton.setBounds(80, 50, 100, 30);
    descendingButton.setBounds(80, 100, 100, 30);
    random.setBounds(80, 272, 100, 30);
    alternative.setBounds(80, 317, 100, 30);

    // Position combobox
    octaveComboBox.setBounds(195, 50, 100, 30);

    // Position the note display label
    noteDisplayLabel.setBounds(40, 200, 220, 40);

    // Position the Rotary sliders:
    rotarySlider.setBounds(220, 265, 120, 120);  // 1st rotary slider
    rotarySlider2.setBounds(330, 265, 120, 120); // 2nd rotary slider
    rotarySlider3.setBounds(330, 50, 120, 120);  // 3rd rotary slider (upper slider)

    // Position the rotary value label below the rotary slider
    rotaryValueLabel.setBounds(220, 360, 120, 30);
    rotaryValueLabel2.setBounds(330, 250, 120, 20);
    rotaryValueLabel3.setBounds(330, 32, 120, 20);

    // Position the checkbox 
    additionalModesToggle.setBounds(182, 244, 150, 30);
}

// Handle slider value changes
void A1AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.arpSpeed = arpSlider.getValue();
    audioProcessor.longDuration = rotarySlider.getValue();
    audioProcessor.shortDuration = rotarySlider2.getValue();
    audioProcessor.duration3 = rotarySlider3.getValue();
}

// Handle button clicks
void A1AudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &ascendingButton) 
        repaint();
    else if (button == &descendingButton) {
        repaint();
    else if (button == &random) {
        repaint();
    else if (button == &alternative) {
        repaint();
}
