/*
 * hc_sr04.h
 *
 *  Created on: Mar 21, 2025
 *      Author: mateu
 */

#ifndef INC_HC_SR04_H_
#define INC_HC_SR04_H_

#include "main.h"

// Timer Callback Function:
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

// Sensor Class:
class HC_SR04{
public:
	HC_SR04(TIM_HandleTypeDef* _htim);
	// Delay Timer Function:
	void delay_us(uint16_t us);

	void Read(void);

	float distance;
private:
	TIM_HandleTypeDef* timer_echo;
	uint32_t ic_val1 = 0;
	uint32_t ic_val2 = 0;
	uint8_t is_first_captured = 0;
	uint32_t diff;
};



#endif /* INC_HC_SR04_H_ */
