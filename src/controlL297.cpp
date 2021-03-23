/*
 * controlL297.cpp
 *
 *  Created on: Mar 16, 2021
 *      Author: 201723940
 */

#include "controlL297.h"

controlL297::controlL297(_L297_SELECT_ selection)
{
	config = new hardwareConfig();
	config->GPIO_Config(GPIOB, 0, OUTPUT);//enable de tout les moteurs
	config->GPIO_Config(GPIOB, 1, OUTPUT);//half full

	config->GPIO_Pin_Enable(GPIOB, 1);
	//config->GPIO_Pin_Disable(GPIOB,1);
	speed = 1;// vitesse en HZ
	lock = true;// actif bas
	chanel=0;

	_selection= selection;

	enabled = false;


	switch (selection) {
	case L297_1://master
		//init pinout
		chanel = 2;
		config->GPIO_Config(GPIOB, 3, ALTERNATE,1);//voir datasheet à table 11 pour les details de AFR
		config->GPIO_Config(GPIOA, 10, OUTPUT);// direction
		config->GPIO_Config(GPIOC, 9, OUTPUT);// lock

		timer = new Timer(TIM2,50000,false);
		timer->enablePWM(chanel,speed);


		break;
	case L297_2://master
		//init pinout
		chanel = 1;
		config->GPIO_Config(GPIOB, 6, ALTERNATE, 2);//voir datasheet à table 11 pour les details de AFR
		config->GPIO_Config(GPIOA, 11, OUTPUT);// direction
		config->GPIO_Config(GPIOC, 8, OUTPUT);// lock


		timer = new Timer(TIM4,50000,false);
		timer->enablePWM(chanel, speed);

		break;
	case L297_3: // master
		//init pinout
		chanel = 2;
		config->GPIO_Config(GPIOB, 5, ALTERNATE, 2);//voir datasheet à table 11 pour les details de AFR
		config->GPIO_Config(GPIOA, 12, OUTPUT);// direction
		config->GPIO_Config(GPIOC, 7, OUTPUT);// lock
		timer= new Timer(TIM3, 50000, false);
		timer->enablePWM(chanel, speed);

		break;
	case L297_4: // slave de L297_
		chanel = 3;
		// pas besoin de sa propre clk, mais suivre les mouvements de L297_3;
		//init pinout

		config->GPIO_Config(GPIOA, 13, OUTPUT);// direction
		config->GPIO_Config(GPIOB, 12, OUTPUT);// lock

		break;
	}
	//timer->start();

}

controlL297::~controlL297()
{
	if(config)
		delete config;
	if(timer)
		delete timer;
}
void controlL297::setSpeed(uint32_t speed)
{
	this->speed = speed;
	timer->enablePWM(chanel, speed);
	timer->start();
}
void controlL297::setDirection(_DIRECTION_ dir)
{
	switch (dir) {
	case CCW:
		if(_selection==0)
			config->GPIO_Pin_Enable(GPIOA, 10);
		if(_selection==1)
			config->GPIO_Pin_Enable(GPIOA, 11);
		if(_selection==2)
			config->GPIO_Pin_Enable(GPIOA, 12);
		if(_selection==3)
		{

		}
		break;

	case CW:
		if(_selection==0)
			config->GPIO_Pin_Disable(GPIOA, 10);
		if(_selection==1)
			config->GPIO_Pin_Disable(GPIOA, 11);
		if(_selection==2)
			config->GPIO_Pin_Disable(GPIOA, 12);
		if(_selection==3)
		{

		}
		break;
	}
}
void controlL297::setLockState(_STATE_ state)
{
	switch (state) {
	case UNLOCK :
		if(_selection==0)
			config->GPIO_Pin_Enable(GPIOC, 9);
		if(_selection==1)
			config->GPIO_Pin_Enable(GPIOC, 8);
		if(_selection==2)
			config->GPIO_Pin_Enable(GPIOC, 7);
		break;
	case LOCK:
		if(_selection==0)
			config->GPIO_Pin_Disable(GPIOC, 9);
		if(_selection==1)
			config->GPIO_Pin_Disable(GPIOC, 8);
		if(_selection==2)
			config->GPIO_Pin_Disable(GPIOC, 7);
		break;
	}

}
void controlL297::setEnable(bool state)
{
	switch (state) {
	case true:
		config->GPIO_Pin_Enable(GPIOB, 0);
		break;
	default:
		config->GPIO_Pin_Disable(GPIOB, 0);
		break;
	}

}
uint32_t controlL297::getSpeed(void)
{
	return speed;
}
bool controlL297::getDirection(void)
{
	return 0;
}
bool controlL297::getLockState(void)
{
	return lock;
}
bool controlL297::isEnables(void)
{
	return enabled;
}
