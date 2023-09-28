# hackedorchestra_teensy

Documentation for teensy 4.0 examples for hacked orchestra workshops

Teensy basics as per [this workshop](https://www.pjrc.com/store/audio_tutorial_kit.html). 


<img src="./images/flyer8.png" alt="" width="400"/> 


## BOM 

* [Teensy 4.0 - without headers](https://www.tinytronics.nl/shop/en/development-boards/microcontroller-boards/teensy/teensy-4.0) € 27,50 via TinyTronics or Open Circuit
* [Teensy Audio Adapter Board for Teensy 4.0](https://www.tinytronics.nl/shop/en/development-boards/accessories/adapter-boards/teensy-audio-adapter-board-for-teensy-4.0) € 16,25 via TinyTronics or Open Circuit
* [Breakaway female headers row of 2, 3 and 4 positions](https://www.reichelt.nl/nl/nl/vrouwelijke-connector-2-54mm-1x20-scheidbaar-vertind-fis-bl1-20-z-p283794.html?&nbc=1) € 1,47 via Reichelt
* [Teensy header kit](https://opencircuit.shop/product/teensy-header-kit) € 2 via Open Circuit 
* [Condenser mic](https://www.tinytronics.nl/shop/en/sensors/sound/czn-15e-electret-condenser-microphone) € 0,50 via TinyTronics
* [32GB *class A1* SD card](https://www.tinytronics.nl/shop/en/data-storage/sd-cards/kingston-canvas-select-plus-32gb-class-10-uhs-i-a1-microsd-card-with-sd-kaart-adapter) € 7,00 via TinyTronics
* [Micro USB cable](https://www.tinytronics.nl/shop/en/cables-and-connectors/cables-and-adapters/usb/micro-usb/goobay-93918-micro-usb-cable-1m-black) € 2,25 via TinyTronics
* [Breadboard 830 points for prototyping](https://www.tinytronics.nl/shop/en/tools-and-mounting/prototyping-supplies/breadboards/breadboard-830-points) € 4,00 via TinyTronics
* 3x [Momentary switch](https://www.tinytronics.nl/shop/en/switches/manual-switches/push-buttons-and-switches/black-button-7mm) € 0,41 each via TinyTronics
* 3x [10k potentiometer, breadboard compatible](https://www.tinytronics.nl/shop/en/components/resistors/potentiometers/10k%CF%89-potmeter-standard) € 0,41 each via TinyTronics
* A pair of simple jack headphones and a speaker with a 3.5mm jack-jack cable and/or adapter. 


## Soldering the shield

[Tips, tricks pics]

## Install for Mac & Arduino 1.8.x

More info on other OS: [https://www.pjrc.com/teensy/td_download.html](https://www.pjrc.com/teensy/td_download.html)


### Install Teensyduino software (full package)

Looks like Arduino but works w Teensy without having to use additional teensyloader software, this is all included. 

Download Macintosh Complete installer for Arduino 1.8: [https://www.pjrc.com/teensy/td_158/Teensyduino_MacOS_Catalina.zip](https://www.pjrc.com/teensy/td_158/Teensyduino_MacOS_Catalina.zip)

Drag Teensyduino.app to applications. 

All programming should be done in Teensyduino software (not Arduino IDE)


### Board manager

Add link below to the JSON board manager URL in the preference panels: 

```
https://www.pjrc.com/teensy/package_teensy_index.json
```

Then install via > Tools > Boards > Boards Manager (search for "Teensy")

Installing separate Teensy Loader is not necessary, it comes with board install. 



### Choosing board & port

**Board**

Choose Teensy 4.0 from board menu

**Port**

When plugged in, a section called "Teensy Ports" shows up under > Tools > Port. On mac the port looks something like */dev/cu.usbmodemxxxxxxx Serial (Teensy 4.0)*

There might sometimes be some hassle with ports, then hit boot button and try again


## Example files

Upload simple blink sketch to check. The built-in LED is on pin 13 for teensy 4.0

All examples (incl the ones discussed [in video tutorial](https://www.pjrc.com/store/audio_tutorial_kit.html)) can be found under 
> Examples > Audio > Tutorial

Everything under "audio" is quite useful to take a look at first, additionally interesting example sketches for us: 

* Granular
* Guitar
* Recorder
* SimpleDrum


## Audio system design tool 

[https://www.pjrc.com/teensy/gui/index.html](https://www.pjrc.com/teensy/gui/index.html)

>> Open in **SAFARI**! In Firefox you can drag and drop but not delete things.

Lots of stuff, playing with mixers, filters, playback from SD card, or RAW samples included in Arduino code etc


## Note on recording and playback

With the record example you can make a recording either with the mix or with the line in on the board. It generates a .raw file that you can import into Audacity and convert into something else. It overwrites the file every time you record and recording time is 9-s	10 secs or so

## characters

The idea is that the participants in the workshop pick a sensor (pickup coil, piezo, solar cell, dynamo, mic(?)) connect it to th amp and experiment to create an 'instrument' with it. 
Then, when and if they have something that sounds reasonally nice, they pick one if 4/5 characters, these are filters created with the teensy (much like a guitar effect pedal). We call these *characters*.
Each *character* is basically a teensy filter with some buttons/knobs to control the parameters. Volume can be controlled on the speaker. 

1. Freeverb - input -> output. (heerko)
   controls: knob 1 - mix (unfiltered/filtered)
             knob 2 - roomsize
             knob 3 - damping.

2. Bitcrusher (heerko)
   controls: crushBits/samplerate. could be knob or buttons. tbd. 
3. Echo (loes)
   controls: delay
4. Flange (loes)
   controls: ? (on/off)? numVoices?
3. Pitchshift?





## Ideas

We can program this set up to add reverb/echo and trim/filter the sounds we get with the coils, motors and pickup mics. 

There's endless more variations to do with the pickup mics, running sounds through different trash materials. Or creating a feedback loop from piezo in to piezo out. 

We could also let the teensy create the sounds. The guitar examples can play chords for example which is quite cool. Other instruments also possible. Ordered a bunch of piezo films and disks to play with. 

Instead of a out-of-the-box consumer speaker we could also drive alternative/modified speakers like a corked speaker, pager motor, etc. to distort the sounds. 

