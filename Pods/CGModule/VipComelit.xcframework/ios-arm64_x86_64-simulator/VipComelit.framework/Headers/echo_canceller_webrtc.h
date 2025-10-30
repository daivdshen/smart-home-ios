/**
 * @file echo_canceller_webrtc.h
 *
 * EchoCanceller class. Class that implement a Webrtc echo canceler.
 *
 * @author Dario Corna
 */

#ifndef ECHO_CANCELLER_WEBRTC_H
#define ECHO_CANCELLER_WEBRTC_H

#include <webrtc/modules/audio_processing/include/audio_processing.h>
#include <webrtc/modules/interface/module_common_types.h>

#define FIR_LENGTH (840)
#define PERF_SEQ_LENGTH (FIR_LENGTH + 180*2 + 100) ///< webrtc fir

typedef int16_t fract16;

///< Echo preset identifiers
enum EchoPresetId
{
	ECHO_PRESET_HIGH = 1, ///< Maximum echo canceling: like half duplex
	ECHO_PRESET_MID  = 2, ///< Medium echo canceling: little half duplex
	ECHO_PRESET_LOW  = 3  ///< Low echo canceling: almost full duplex
};

///< Echo preset values
struct EchoPreset
{
	bool         nsEnable; // Enable noise suppression
	webrtc::NoiseSuppression::Level nsLevel; // Noise suppression level
	bool         vadEnable; // voice detection
	webrtc::VoiceDetection::Likelihood vadLevel; // voice detection level
	bool         aecEnable; // Enable Echo cancellation
	webrtc::EchoControlMobile::RoutingMode aecMode;   // AEC routing mode
	bool         resetEnable; // Enable engine reset on call
	bool         hfFilterEnable; // Enable High Frequency filter
	float        micMult; // Digital mic multiplier
	float        spkMult; // Digital spk Multiplier
};

///< Echo presets map
typedef std::map<EchoPresetId, EchoPreset> EchoPresetMap;

class EchoCanceller
{
public:
	/**
	 * Default constructor
	 * @param[in] presets map of EchoCanceller parameters grouped into presets
	 */
	EchoCanceller(EchoPresetMap presets);

	/**
	 * @brief echoControlParameterSet
	 * @param id
	 */
	void echoControlParameterSet(EchoPresetId id);
	int  getProcessedGainCaptureVolume();
	void setCaptureVolume(int vol);
	void endCall();
	bool isRunning() { return isEchoReady; }

	/**
	 * @brief setDelay
	 * @param delay
	 */
	void setEchoDelay(int _delay) { delay = _delay; }

	void addMicSamples(fract16* micPointer, int numSamples);
	void addSpkSamples(fract16 * packet, int size);

	float getDigitalMicMultiplier() { return echoPresets.at(currPresetId).micMult; }
	float getDigitalSpkMultiplier() { return echoPresets.at(currPresetId).spkMult; }

private:
	EchoPresetId  currPresetId;                         ///< Keeps track of the current Echo Preset
	EchoPresetMap echoPresets;                          ///< Variable to control echo (one for high, mid, low values, respectively)
	webrtc::AudioProcessing *apm;
	webrtc::AudioFrame      micFrame;
	webrtc::AudioFrame      spkFrame;
	bool                    isEchoReady;
	int16_t*                echoPath;
	int                     captureVolume;
	int                     perfSectProgress;
	static const int        STARTUP_DELAY = 20; // number of 10ms webrtc packets before to start echo canc
	int                     delay;///< The delay result (calculated from cross correlation)
	unsigned int            micPktId;
	unsigned int            spkPktId;
	bool                    nsEnabled;
	bool                    resetEnabled;

private:

	/**
	 * Reset internal echo canceller engine
	 */
	void reset();
};

#endif // ECHO_CANCELLER_WEBRTC_H
