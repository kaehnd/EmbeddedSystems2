/*
 * speaker.h
 *
 *  Created on: Dec 23, 2019
 *     Author: Daniel Kaehn
 *     Class: CE 2812-021
 *     Assignment: Lab 3: LCD and Keypad API
 *
 *     Declarations for speaker peripheral API
 */

#ifndef SPEAKER_H_
#define SPEAKER_H_



/*
 * Initialize Timer3 and GPIOB for use with speaker peripheral
 * MUST BE CALLED BEFORE ANY OTHER FUNCTION IN THIS FILE
 * Arguments: none
 * Returns: none
 */
void speaker_init(void);

/*
 * Plays a continuous square wave tone at the specified frequency
 * Arguments: int frequency, frequency in Hz of tone to be played
 * Returns: none
 */
void speaker_play_tone(int frequency);

/*
 * Stops any playing tone
 * Arguments: none
 * Returns: none
 */
void speaker_stop(void);

/*
 * Plays tones at specified frequency and blocks until input is recieved from the keypad
 * Arguments: int frequency, frequency in Hz of tone to be played
 * Returns: none
 */
void speaker_alarm(int frequency);

#endif /* SPEAKER_H_ */
