/**
 * @file echo_canceller_bzaec.h
 *
 * EchoCanceller class. Class that implement a B2ZAEC echo canceler.
 *
 * WARNING: this code is still INCOMPLETE and EXPERIMENTAL. Use at your own risk.
 *
 * @author Fabio Bigoni, Roberto Muzzì, Massimiliano Zucchelli, Ghilardi Gian Paolo
 */

#ifndef ECHO_CANCELLER_BZAEC_H
#define ECHO_CANCELLER_BZAEC_H

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>

#define MIC_BUFFER_SIZE                     8000    ///< Size of mic buffer (1 seconds)
#define SPK_BUFFER_SIZE                     8000    ///< Size of spk buffer (1 seconds)

// Debug mode
//#define ENABLE_DUMP_COEFFS                    // If defined sump ec coefficients on disk

// Echo canceler definitions
#define FIR_LENGTH                          220     //180
#define PERF_SEQ_LENGTH                     960     //960             ///< Fill with 0 to have multiple of 40ms
#define BETA_PERF_SEQ                       0x065B  //0x07F4

// Gain regulationd during cross correlation
#define SPEAKER_ATTENUATION_CROSS_CORRELATION   0x1000    ///< Speaker attenuation during cross correlation
#define STEP_RUN_VOLUME_INCREASE                500       ///< Increase step to reach maximum volume setted when echo canceler is working
#define STEP_RUN_VOLUME_INCREASE_MIC            100       ///< Increase step to reach maximum volume setted when echo canceler is working

typedef int16_t fract16;

///< Echo canceller status
enum EchoStatus
{
	ECHO_MIC_STARTUP_WAIT_STABILIZATION,
	ECHO_PERFECT_SEQUENCE,
	ECHO_CROSS_CORRELATION_SAVING_SAMPLES,
	ECHO_FIR_CALCULATING,
	ECHO_RUN_VOLUME_ADJ,
	ECHO_RUN
};

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
	unsigned int cclipLevel;
	unsigned int digSpkMult;
	unsigned int digMicMult;
	unsigned int gainSpkAttenuation;
	unsigned int gainMicAttenuation;
};

///< Echo presets map
typedef std::map<EchoPresetId, EchoPreset> EchoPresetMap;

/**
 * Class implementing B2ZAEC echo canceller
 *
 * Usage: the basic echo canceller processing loop
 *        involves just 3 APIs, to be called in strict order
 *        in a single thread that handled both micorophone and
 *        speaker audio.
 *
 *        1. addMicSamples: add microphone samples to the echo canceller
 *        2. addSpkSamples: add speaker samples for processing and obtain
 *                          the processed samples to be sent to the speaker
 *        3. getSamples:    get the processed samples that can be sent to the
 *                          remote network peer
 *
 * At the moment this code is only tested and works with 40 ms audio buffers.
 */
class EchoCanceller
{
	public:

		/**
		 * Default constructor
		 * @param[in] presets map of EchoCanceller parameters grouped into presets
		 */
		EchoCanceller(EchoPresetMap presets);

		/**
		 * Set the current echo canceller preset by an id
		 * @param[in] id identifier fot the preset to set as current
		 */
		void setEchoPresetId(EchoPresetId id);

		/**
		 * @Return the current preset in use
		 * @return the current preset in use
		 */
		EchoPreset &getEchoPreset();

		/**
		 * Set the volume level for the speaker
		 * @param[in] volume new volume to set, range [0,9]
		 * @param[in] enableSoftStart if enabled the volume is progressively increased from 0 to the specified value;
		 *            if disabled (default), the volume level is immediately set
		 */
		void setSpkVolume(int volume, bool enableSoftStart = false);

		/**
		 * Set the volume level for the microphone
		 * @param[in] volume new volume to set, range [0,9]
		 */
		void setMicVolume(int volume);

		/**
		 * Set an initial delay to waif for proper microphone stabilization
		 * @param[in] delayMs time to wait for proper microphone stabilization (in ms)
		 */
		void setStartupMicDelay(int delayMs);

		/**
		 * Set the echo delay in samples between transmitted or received signal
		 * @param newDelay delay in samples between transmitted or received signal
		 */
		void setEchoDelay(int newDelay);

		/**
		 * Set mic thresholds for auto gain modifier algorithm. If data power > activationTh open ch,
		 * if lower than micDeactivationTh close channel.
		 * @param activationTh
		 * @param deactivationTh
		 * @return 0 if success, -1 function not avaiable (depends on ENABLE_GAIN_MODIFIER)
		 */
		int setMicThresholds(fract16 activationTh, fract16 deactivationTh);

		/**
		 * Set speaker thresholds for auto gain modifier algorithm. If data power > activationTh open ch,
		 * if lower than deactivationTh close channel.
		 * @param activationTh
		 * @param deactivationTh
		 * @return 0 if success, -1 function not avaiable (depends on ENABLE_GAIN_MODIFIER)
		 */
		int setSpkThresholds(fract16 activationTh, fract16 deactivationTh);

		/**
		 * Set speaker saturation threshold to limit data after multiply volume.
		 * NOTE: to avoid echo ret on saturation (FIR response on saturation need dynamcs low than maximum)
		 *       set as 0x6777
		 * @param sat
		 */
		void setSpkSaturationThreshold(fract16 sat);

		/**
		 * Add the microphone samples to the echo canceller for processing
		 * @param[in] buffer input buffer
		 * @param[in] samples number of samples in the buffer
		 * @return true if the operation was successfully completed; false otherwise
		 */
		bool addMicSamples(fract16 *buffer, int samples);

		/**
		 * Add the speaker samples to the echo canceller for processing
		 * @param[in]  buffer input buffer
		 * @param[in]  samples number of samples in the buffer
		 * @param[out] processed samples that can be sent to the speaker. Caller
		 *             is not required to allocate memory and it must not free
		 *             this buffer after use
		 * @return true if the operation was successfully completed;
		 *         false otherwise (output buffer is not valid and cannot be used)
		 */
		bool addSpkSamples(fract16 *buffer, int samples, fract16 **output);

		/**
		 * Get processed samples from the echo canceller
		 * @param[in]  samples number of processed samples to obtain
		 * @param[out] output processed samples that can be sent to the network peer.
		 *                    Caller is not required to allocate memory and it
		 *                    must not free thi buffer after use.
		 * @return true if the operation was successfully completed;
		 *         false otherwise (output buffer is not valid and cannot be used)
		 */
		bool getSamples(int samples, fract16 **output);

		/**
		 * Reset internal echo canceller status for a new call.
		 */
		void reset();

		/**
		 * Notify the echo canceller the call is terminated
		 */
		void endCall();

		/**
		 * Return the current echo canceller status
		 */
		EchoStatus getEchoStatus();

		/**
		 * Return if the echo canceller is properly running or it's still initializing
		 * (e.g., it's calculating FIR)
		 * @return true if echo canceller is properly running; false if it's still initializing
		 */
		bool isRunning();

		/**
		 * Attenuate samples by a input attenuation level
		 * @param[in, out] buffer buffer with samples to operate on
		 * @param[in]      samples number of samples in the buffer
		 * @param[in]      volume attenuation level to be applied to the samples in the buffer
		 */
		void attenuateSample(fract16 *buffer, int samples, int volume);

		/**
		 * @brief setSpkGainModifier toggle audio gain modifier processing for speaker samples
		 * @param[in] enable enable/disable gain modifier usage while processing samples in addSpkSamples
		 */
		void setSpkGainModifier(bool enable);

		/**
		 * @brief setMicGainModifier toggle audio gain modifier processing for microphone samples
		 * @param[in] enable enable/disable gain modifier while processing samples in processMicSamples
		 */
		void setMicGainModifier(bool enable);

	private:

		void saveFirCoeffs();

		bool isFIRCalculating();

		void processMicSamples(fract16 *buffer, int samples);

		void gainMicModifier(fract16 micDatum, fract16 cancDatum, fract16 minMicGain);

		fract16 gainSpkModifier(fract16 spk_datum, fract16 minSpkGain);

		fract16 oneStepFIRComputation(fract16* inputs, int inputOffset, unsigned int speakerBufferSize);

		void lmsAlgorithm (int pointer, fract16 val, fract16* spkBuffer, int spkBufferSize);

		fract16 firLmsComputation(std::vector<fract16> &inputs, int inputOffset, fract16* micVal);

		inline void centerClip(fract16 *dataInout, fract16 dataPred);

		void attenuateSamples(fract16 *buffer, int samples, fract16 attenuation);

		/**
		 * @brief applyGainMultiplier applies to the samples in buffer the multiplier stored in mult
		 *        and saturates the result.
		 *
		 * @param buffer    audio samples
		 * @param samples   number of samples in buffer
		 * @param mult      multiplier
		 * @param SATMin    minimum saturation value
		 * @param SATMax    maximum saturation value
		 */
		void applyGainMultiplier(fract16 *buffer, int samples, unsigned mult, short SATMin, short SATMax);

	private:

		std::vector<fract16> micBuffer;                     ///< The buffer of microphone samples
		std::vector<fract16> spkBuffer;                     ///< The buffer of speaker samples

		int ptrEchoProcessing;                              ///< Read pointer for fir calculating

		EchoStatus echoStatus;                              ///< The status of echo canceler

		int delay;                                          ///< The delay result (calculated from cross correlation)

		// FIR management
		int echoFirCoeffs[FIR_LENGTH];                      ///< The values of coeffs for FIR echo canceler
		fract16 previousErrorValueForLMS;

		EchoPresetId  currPresetId;                         ///< Keeps track of the current Echo Preset
		EchoPresetMap echoPresets;                          ///< Variable to control echo (one for high, mid, low values, respectively)

		int startupMicSamplesDuration;                      ///< Variable need to setup the mic startup wait stabilization
		fract16  gainMic;                                   ///< Digital gain to be applied to processed samples

		fract16 currSpkAttenuation;                         ///< Current speaker attenuation value
		fract16 maxSpkAttenuation;                          ///< Final speaker attenuation value during soft start

		fract16 spkSaturationThreshold;                     ///< Saturation threshold for speaker data

		fract16 currMicAttenuation;                         ///< Current mic attenuation value

		bool enableSpkGainModifier;                         ///< Enable/disable gain modifier processing in addSpkSamples
		bool enableMicGainModifier;                         ///< Enable/disable gain modifier processing in processMicSamples

		fract16 spkActivationThreshold;                     ///< Activation threshold to open speaker ch
		fract16 spkDeActivationThreshold;                   ///< DeActivation threshold to close speaker ch
		fract16 micDeltaActivationThreshold;                ///< Activation threshold to open microphone ch
		fract16 micDeltaDeActivationThreshold;              ///< DeActivation threshold to close microphone ch

		#if defined(ENABLE_DUMP_COEFFS)
		// for debug
		FILE *echoMicSamples;
		FILE *echoSpkFile;
		FILE *echoSpkInFile;
		FILE *echoMicInFile;
		FILE *echoRunningSpeaker;
		FILE *echoMicOutFile;
		FILE *echoPredOutFile;
		FILE *echoCrossCorrFile;
		FILE *echoSpkGainFile;
		FILE *echoMicGainFile;
		#endif
};
#endif // ECHO_CANCELLER_BZAEC_H
