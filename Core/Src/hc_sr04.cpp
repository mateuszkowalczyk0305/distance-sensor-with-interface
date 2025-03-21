/*
 * hc_sr04.cpp
 *
 *  Created on: Mar 21, 2025
 *      Author: mateu
 */

#include "hc_sr04.h"
#include "tim.h"

uint32_t ic_val1 = 0;
uint32_t ic_val2 = 0;
uint8_t is_first_captured = 0;
uint32_t diff;
float distance;

HC_SR04* HC_SR04::instance = nullptr;

HC_SR04::HC_SR04(TIM_HandleTypeDef* _htim){
	timer_echo = _htim;
	instance = this;
}

void HC_SR04::delay_us(uint16_t us){
	  __HAL_TIM_SET_COUNTER(timer_echo, 0);  			// reset counter
	  while (__HAL_TIM_GET_COUNTER(timer_echo) < us);
};

void HC_SR04::Read(void){
	  HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
	  delay_us(2);
	  HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
	  delay_us(10);
	  HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);

	  __HAL_TIM_ENABLE_IT(timer_echo, TIM_IT_CC1);
};

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	  {
	    if (HC_SR04::instance->is_first_captured == 0)
	    {
	      HC_SR04::instance->ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
	      HC_SR04::instance->is_first_captured = 1;
	    }
	    else
	    {
	    	HC_SR04::instance->ic_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
	      __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);

	      if (ic_val2 > ic_val1)
	    	  HC_SR04::instance->diff = ic_val2 - ic_val1;
	      else
	        diff = (0xFFFF - ic_val1) + ic_val2;

	      HC_SR04::instance->distance = (diff * 0.0343) / 2;
	      HC_SR04::instance->is_first_captured = 0;
	    }
	  }
};
