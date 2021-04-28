/*
 * @file    -> Encodeur.cpp
 * @author  -> Justin Bélanger
 * @version -> 0.01
 * @Created -> 23 mars 2021
 * @brief 	-> Classe qui crée et gere les interruption relier à en encodeur de roue
 */

#include "Encodeur.h"

/*
 * @name   -> Encodeur
 * @brief  -> constructeur de la classe Encodeur
 * @param  -> gpioPort: port sur lequel la pin de trigger sera
 * 			  noPin:	pin sur laquelle le trigger sera
 * 			  trigger:	type de trigger voulue (rissing ou falling edge)
 * @return -> None
 */
Encodeur::Encodeur(GPIO_TypeDef *gpioPort, uint8_t noPin, uint8_t trigger)
{
	// TODO Auto-generated constructor stub
	this->noPin = noPin;
	innitExternalInterupt(gpioPort,noPin, trigger);
}

/*
 * @name   -> innitExternalInterupt
 * @brief  -> innitialise l'interruption à trigger externe sur le port et al pin voulue avec le bon type de trigger
 * @param  -> gpioPort: port sur lequel la pin de trigger sera
 * 			  noPin:	pin sur laquelle le trigger sera
 * 			  trigger:	type de trigger voulue (rissing ou falling edge)
 * @return -> None
 */
void Encodeur::innitExternalInterupt(GPIO_TypeDef *gpioPort, uint8_t noPin, uint8_t trigger)
{

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	//set la pin voulue en alternated

	//set qu'elle pin doit attendre l'interrupt
	if(gpioPort == GPIOB)
	{
		GPIOB->MODER &= ~(3 << (noPin *2));
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOBEN); //activation de la clock sur GPIOB
		if(noPin < 4)
		{
			SYSCFG->EXTICR[0] |= SYSCFG_EXTI_PB << (noPin *4);
		}

		else if(noPin < 8)
		{
			SYSCFG->EXTICR[1] |= SYSCFG_EXTI_PB << ((noPin - 4) *4);
		}

		else if(noPin < 12)
		{
			SYSCFG->EXTICR[2] |= SYSCFG_EXTI_PB << ((noPin - 8) *4);

		}

		else if(noPin < 16)
		{
			SYSCFG->EXTICR[3] |= SYSCFG_EXTI_PB << ((noPin - 12) *4);
		}
	}

	if(gpioPort == GPIOC)
	{
		GPIOC->MODER &= ~(3 << (noPin *2));
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN); //activation de la clock sur GPIOB
		if(noPin < 4)
		{
			SYSCFG->EXTICR[0] |= SYSCFG_EXTI_PC << (noPin *4);
		}

		else if(noPin < 8)
		{
			SYSCFG->EXTICR[1] |= SYSCFG_EXTI_PC << ((noPin - 4) * 4);
		}

		else if(noPin < 12)
		{
			SYSCFG->EXTICR[2] |= SYSCFG_EXTI_PC << ((noPin - 8) * 4);

		}

		else if(noPin < 16)
		{
			SYSCFG->EXTICR[3] |= SYSCFG_EXTI_PC << ((noPin - 12) * 4);
		}
	}

	//edge trigger select
	switch(trigger)
	{
	case FALLING_EDGE_TRIGGER:
		EXTI->FTSR |= 1 << noPin;
		EXTI->RTSR &= ~(1 << noPin);
		break;

	case RISING_EDGE_TRIGGER:
		EXTI->RTSR |= 1 << noPin;
		EXTI->FTSR &= ~(1 << noPin);
		break;

	case RISIN_FALLING_EDGE_TRIGGER:
		EXTI->FTSR |= 1 << noPin;
		EXTI->RTSR |= 1 << noPin;
		break;
	}

	//enable interrupt sur le bon EXTI
	EXTI->IMR |= 1 << noPin;
	EXTI->EMR |= 1 << noPin;

	//enable interrupt sur le bon NVIC
	if(noPin < 5)
	{
		switch(noPin)
		{
		case 0:
			NVIC_EnableIRQ(EXTI0_IRQn);
			break;

		case 1:
			NVIC_EnableIRQ(EXTI1_IRQn);
			break;

		case 2:
			NVIC_EnableIRQ(EXTI2_IRQn);
			break;

		case 3:
			NVIC_EnableIRQ(EXTI3_IRQn);
			break;

		case 4:
			NVIC_EnableIRQ(EXTI4_IRQn);
			break;
		}
	}

	else if(noPin < 10)
	{
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	}

	else if(noPin < 16)
	{
		NVIC_EnableIRQ(EXTI15_10_IRQn);
	}
}

/*
 * @name   -> clearInterruptFlag
 * @brief  -> clear le flag du registre EXTI_PR (Necessaire après chaque interruption)
 * @param  -> None
 * @return -> None
 */
void Encodeur::clearInterruptFlag()
{
	EXTI->PR |= 1 << this->noPin;
}

/*
 * @name   -> ~Encodeur
 * @brief  -> destructeur de la classe
 * @param  -> None
 * @return -> None
 */
Encodeur::~Encodeur()
{
	// TODO Auto-generated destructor stub
}

