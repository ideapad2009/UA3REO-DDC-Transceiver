# Transceiver "Wolf"
DDC-DUC SDR Tranceiver project https://ua3reo.ru/tag/transiver-ua3reo/

На русском https://github.com/XGudron/UA3REO-DDC-Transceiver/blob/master/README.ru-RU.md

## Principle of operation

The RF signal is digitized by a high-speed ADC chip and fed to an FPGA processor. <br>
It performs DDC / DUC conversion (digital frequency shift down or up the spectrum) - by analogy with a direct conversion receiver. <br>
The I and Q quadrature signals from the conversions are fed to the STM32 microprocessor. <br>
It filters, (de) modulates and outputs audio to an audio codec / USB. It also handles the entire user interface. <br>
When transmitting, the process occurs in the opposite order, only at the end of the chain there is a DAC, which converts the digital signal back to analog RF. <br>

## Specifications

* Receiving frequencies: 0 MHz - 750 MHz
* Transmission frequencies: 0 MHz - 160 MHz
* TX power: 6-7W (HF), 1W (VHF)
* Two antenna inputs
* Modulation types (TX / RX): CW, LSB, USB, AM, FM, WFM, DIGI
* LNA, Preamplifier
* Adjustable attenuator 0-31dB
* Two antenna inputs
* Band pass filters
* ADC dynamic range 100dB
* Supply voltage: 13.8V (overvoltage and polarity reversal protection)
* Consumption current when receiving: ~ 0.65A
* Current consumption during transmission: ~ 1.7A +

## Transceiver Features

* Panorama (spectrum + waterfall) 96 kHz wide
* Dual receiver (mixing A + B or A&B audio in stereo)
* Adjustable bandwidth: HPF from 0Hz to 500Hz, LPF from 300Hz to 20kHz
* Integrated SWR/power meter (HF)
* Automatic and manual Notch filter
* Switchable AGC (AGC) with adjustable attack rate
* Range map, with the ability to automatically switch modes
* Digital Noise Reduction (DNR), Pulse Noise Reduction (NB)
* CAT / PTT virtual COM ports (FT-450 emulation)
* USB operation (audio transmission, CAT, KEY, PTT)
* CW decoder, self-control
* Spectrum analyzer
* AGC takes into account the characteristics of human hearing (K-Weighting)
* TCXO frequency stabilization (it is possible to use an external clock source, such as GPS)
* WiFi operation: Time synchronization, virtual CAT interface (see Scheme/WIFI-CAT-instruction.txt)

### Sensitivity

At 10dB signal-to-noise ratio, LNA is on, ATT, LPF, BPF are off

Frequency, mHz | Sensitivity, dBm | Sensitivity
------------ | ------------- | -------------
1.9	| -131	| 63.0 nV
3.6	| -131	| 63.0 nV
7.1	| -131	| 63.0 nV
10  | -131	| 63.0 nV
14  | -131  | 63.0 nV
18  | -131  | 63.0 nV
21  | -131  | 63.0 nV
25  | -131  | 63.0 nV
28  | -131  | 63.0 nV
50	| -131	| 63.0 nV
100	| -131	| 63.0 nV
145	| -128	| 88.9 nV
435	| -121	| 0.2 uV

## Build

I ordered the boards in the Chinese service JLCPCB, they and their schemes are in the Scheme folder. <br>
After assembly, you need to flash FPGA and STM32 chips. <br>
STM32 firmware is performed via Keil or via USB lanyard in DFU Mode (by STM32 / FLASH.bat script). Or via ST-LINK v2. You must hold down the power button while flashing. <br>
FPGA firmware is performed through the Quartus program using a USB-Blaster. <br>
A properly assembled device does not require debugging, but if problems arise, the first thing to do is to check for clock signals: <br>
90 FGPA pin and ADC clock input - 122.88 MHz, PC9 STM32 pin - 12.288 MHz, PB10 STM32 pin - 48 kHz. <br>
If necessary, calibrate the transceiver through the appropriate menu <br>
WiFi module ESP-01 must have fresh firmware with SDK 3.0.4 and higher, and AT commands 1.7.4 and higher <br>

## Management

* **AF GAIN** - Volume
* **SHIFT / GAIN** - When the SHIFT function is active - smooth offset from the selected transceiver frequency. Inactive - IF gain control
* **ENC MAIN** - Main encoder for frequency control and menu settings
* **ENC 2** - Auxiliary encoder for menu operation. In normal mode, quickly switches the frequency
* **BAND -** - Switch to the band below
* **BAND +** - Switch to higher band
* **MODE -** - Mode group switching SSB-> CW-> DIGI-> FM-> AM
* **MODE +** - Switching subgroup mod LSB-> USB, CW_L-> CW_U, DIGI_U-> DIGI_L, NFM-> WFM, AM-> IQ-> LOOP
* **FAST** - Mode of fast x10 rewinding of the frequency by the main encoder (configurable)
* **FAST [clamp]** - Frequency step settings
* **PRE** - Turn on the preamplifier (LNA)
* **PRE [clamp]** - Turn on the driver and / or amplifier ADC
* **ATT** - Turn on the attenuator
* **MUTE** - Mute the sound
* **AGC** - Turn on AGC (automatic gain control)
* **AGC [clamp]** - AGC settings
* **A = B** - Setting the second bank of the receiver equal to the current
* **BW** - Switch to bandwidth selection menu (LPF)
* **BW [clamp]** - Switch to bandwidth selection menu (HPF)
* **TUNE** - Turn on the carrier for tuning the antenna
* **RF POWER** - Transmitter power selection
* **RF POWER [clamp]** - Squelch setting
* **A / B** - Switches between VFO-A / VFO-B receiver settings banks
* **A / B [clamp]** - Enable automatic mode change by bendmap
* **DOUBLE** - Turn on the dual receiver
* **DOUBLE [clamp]** - Switching between the modes of the dual receiver A&B (each channel of the headphones has its own path) or A + B (mixing signals of 2 receivers)
* **DNR** - Enable digital noise reduction
* **DNR [clamp]** - Turn on the impulse noise suppressor (NB)
* **NOTCH** - Turn on the automatic Notch filter to eliminate narrowband interference
* **NOTCH [clamp]** - Turn on the manual Notch filter to eliminate narrowband interference
* **CLAR** - Allows you to split transmission and reception to different VFO banks
* **CLAR [clamp]** - Enables SHIFT control from the front panel
* **WPM** - Switch to key speed selection menu (WPM)
* **WPM [clamp]** - Automatic key enable
* **MENU** - Go to the menu
* **MENU [clamp]** - Enable key lock LOCK
* **MENU [at power on]** - Reset transceiver settings

## Settings

### TRX Settings

* **RF Power** - Transmission power,%
* **Band Map** - Band Map, automatically switches mode depending on the frequency
* **AutoGainer** - Automatic ATT / PREAMP control depending on the signal level on the ADC
* **RF Filters** - Hardware filter management (LPF / HPF / BPF)
* **Two Signal tune** - Two-signal generator in TUNE mode (1 + 2kHz)
* **Shift Interval** - Offset range SHIFT (+ -)
* **Freq Step** - Frequency step by the main encoder
* **Freq Step FAST** - Frequency step by the main encoder in FAST mode
* **Freq Step ENC2** - Frequency tuning step by main add. encoder
* **Freq Step ENC2 FAST** - Frequency step by main add. encoder in FAST mode
* **Att step, dB** - Attenuator tuning step
* **DEBUG Console** - Output of debug and service information to USB / UART ports
* **MIC IN** - Select the microphone input
* **LINE IN** - Line input selection
* **USB IN** - Select USB audio input

### AUDIO Settings

* **IF Gain, dB** - IF gain
* **AGC Gain target, LKFS** - Maximum AGC gain (Maximum volume with AGC on)
* **Mic Gain** - Microphone gain
* **DNR xxx** - Digital squelch adjustment
* **SSB HPF Pass** - LPF cutoff frequency when operating in SSB
* **SSB LPF Pass** - HPF cutoff frequency when operating in SSB
* **CW LPF Pass** - LPF cutoff frequency when working in CW
* **FM LPF Pass** - LPF cutoff frequency when working in FM
* **FM Squelch** - FM squelch level
* **MIC EQ xxx** - Microphone equalizer levels
* **RX EQ xxx** - Receiver equalizer levels
* **RX AGC Speed** - AGC (automatic signal level control) response speed for reception (more-faster)
* **TX AGC Speed** - AGC / compressor response speed for transmission (more-faster)

### CW Settings

* **CW Key timeout** - Time before stopping the transmission mode after releasing the key
* **CW Generator shift** - Detuning the receive generator from the transmit frequency
* **CW Self Hear** - Self-control CW (key press is heard)
* **CW Keyer** - Automatic Key
* **CW Keyer WPM** - Key Speed, WPM
* **CW Decoder** - Software CW receive decoder

### SCREEN Settings

* **FFT Zoom** - FFT spectrum approximation (X1 - 96kHz, X2 - 48kHz, X4 - 24kHz, X8 - 12kHz, X16 - 6kHz)
* **FFT Speed** - FFT and waterfall speed
* **FFT Height** - FFT and waterfall proportional height
* **FFT Color** - FFT and waterfall colors: 1(blue -> yellow -> red), 2(black -> yellow -> red), 3(black -> yellow -> green), 4(black -> red), 5(black -> green), 6(black -> blue), 7(black -> white)
* **FFT Grid** - FFT and waterfall grids: 1(no grid), 2(fft grid), 3(fft+wtf grids), 4(wtf grid)
* **FFT Enabled** - Enable waterfall and FFT
* **FFT Averaging** - FFT burst averaging level
* **FFT Window** - Select FFT window (Hamming / Blackman-Harris / Hanning)

### ADC / DAC Settings

* **ADC Driver** - Turn on the preamplifier-ADC driver
* **ADC Preamp** - Turn on the preamplifier built into the ADC
* **ADC Dither** - Enable ADC dither for receiving weak signals
* **ADC Randomizer** - Enables ADC digital line encryption
* **ADC Shutdown** - Turn off the ADC

### WIFI Settings

* **WIFI Enabled** - Enable WiFi module
* **WIFI Select AP** - WiFi hotspot selection
* **WIFI Set AP Pass** - Set password for WiFi hotspot
* **WIFI Timezone** - Time zone (for updating the time via the Internet)
* **WIFI CAT Server** - Server for receiving CAT commands via WIFI

### Calibration [appears by long pressing the MENU button in the settings menu]

* **Encoder invert** - Invert the rotation of the main encoder
* **Encoder2 invert** - Invert the rotation of the additional encoder
* **Encoder debounce** - Time of debouncing contacts of the main encoder
* **Encoder2 debounce** - Time of debouncing contacts of the additional encoder
* **Encoder slow rate** - Deceleration rate of the main encoder
* **Encoder on falling** - The encoder is triggered only when level A falls
* **CIC Shift** - Bit shift after CIC decimator output
* **CICCOMP Shift** - Bit shift after CIC compensator
* **TX CICCOMP Shift** - Bit shift after TX CIC compensator
* **DAC Shift** - Bit shift of the output to the DAC
* **RF GAIN xx** - Calibration of the maximum output power for each range
* **S METER** - S-meter calibration
* **ADC OFFSET** - ADC offset calibration
* **LPF END** - LPF filter parameters
* **BPF x** - Bandpass filter parameters
* **HPF START** - HPF filter parameters
* **SWR TRANS RATE** - Adjustment of the transformation ratio of the SWR meter
* **VCXO Freq** - Frequency adjustment of the reference oscillator

### Set Clock Time

* Clock setting

### Flash update

* Launch STM32 firmware update

### Spectrum Analyzer

* **Spectrum START** - Start the spectrum analyzer
* **Begin, kHz** - Starting frequency of the analyzer with a step of 1 kHz
* **End, kHz** - End frequency of the analyzer in 1kHz steps
* **Top, dBm** - Upper threshold of the graph
* **Bottom, dBm** - Lower threshold of the graph
