/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#pragma GCC push_options    /*It turns on certain optimization flags for GCC*/
#pragma GCC optimize ("Os")


#include "fonts.h"
#include "ssd1306.h"
#include "test.h"
#include "math.h"
#include "OLED_GUI.h"
#include "5D_joystick.h"
#include "thermister_10k.h"
#include "thermocycler.h"
#include "PID.h"
#include "thermistor.h"
#pragma GCC pop_options

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BLOCK_DUTY_HIGHEST        100
#define BLOCK_HIGHEST_TEMP_DIFF   20
#define BLOCK_PID_P               (float)((float)BLOCK_DUTY_HIGHEST/(float)BLOCK_HIGHEST_TEMP_DIFF)
#define ABS(x)                    ((x) > 0? (x) :-(x) )
#define BLOCK_TEMPERATURE_PIN     ADC_CHANNEL_1
#define LID_TEMPERATURE_PIN       ADC_CHANNEL_0
#define EXTRA_TEMPERATURE_PIN_1     ADC_CHANNEL_2
#define EXTRA_TEMPERATURE_PIN_2    ADC_CHANNEL_3

/* Controller parameters */
/*
#define PID_KP  0.0f
#define PID_KI  0.0f      //0.5f
#define PID_KD  0.0f      //0.25f
*/


#define LID_PID_KP       		40.0f
#define LID_PID_KI       		1.3333f      //0.5f
#define LID_PID_KD       		300.0f       //0.25f
#define LID_PID_TAU      		0.02f	     //36
#define LID_PID_LIM_MIN  		0.0f
#define LID_PID_LIM_MAX  		100.0f
#define LID_PID_LIM_MIN_INT  	0.0f
#define LID_PID_LIM_MAX_INT  	500.0f
#define LID_SAMPLE_TIME_S 		0.1f
#define LID_PID_BETA 		        0.0f

#define LID_PWM_MAX  		999.0f


#define PELTIER_PID_KP   		  5.0f //40
#define PELTIER_PID_KI   		  0.0f      //0.5f
#define PELTIER_PID_KD   		  0.0f      //0.25f
#define PELTIER_PID_TAU 		  15.0f
#define PELTIER_PID_LIM_MAX  	  999
#define PELTIER_PID_LIM_MIN  	  (-999)
#define PELTIER_PID_LIM_MIN_INT   (-450)
#define PELTIER_PID_LIM_MAX_INT   (450)
#define PELTIER_SAMPLE_TIME_S 	  0.1f
#define PELTIER_PID_BETA 	       0.0f
#define PELTIER_NO_OF_SAMPLES_AVG  5


/*
 *
#define PCR1

#ifdef PCR1

Defines for PCR 1

#endif /* PCR1*/

/*
#ifdef PCR2

Defines for PCR 2

#endif /* PCR2*/





//PCR1
#define BLOCK_THERMISTOR_R0			(float)10261//11194//10698//9750
#define BLOCK_THERMISTOR_RS			(float)998//990
#define BLOCK_THERMISTOR_ADC        &hadc1
#define BLOCK_THERMISTOR_CHANNEL    ADC_CHANNEL_1
#define BLOCK_THERMISTOR_BETA       (float)4047//3609//3495//3182

#define EXTRA_1_THERMISTOR_R0		  (float)5097
#define EXTRA_1_THERMISTOR_RS		  (float)975.0
#define EXTRA_1_THERMISTOR_ADC        &hadc1
#define EXTRA_1_THERMISTOR_CHANNEL    ADC_CHANNEL_2
#define EXTRA_1_THERMISTOR_BETA       (float)3838


#define EXTRA_2_THERMISTOR_R0		  (float)5097
#define EXTRA_2_THERMISTOR_RS		  (float)977.0
#define EXTRA_2_THERMISTOR_ADC        &hadc1
#define EXTRA_2_THERMISTOR_CHANNEL    ADC_CHANNEL_3
#define EXTRA_2_THERMISTOR_BETA       (float)3838




/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

char num_string[10];
float block_cur_temp,IIR_block_cur_temp,block_cur_error,block_pre_out;
float block_set_temp = 35;
int block_PWM ;
volatile uint8_t OLED_lock = 0;
uint8_t joystick_out;
PCR_settings_t* PCR_settings;
float set_point;
parameter* annealing_temp;
float lid_cur_temp,Lid_setpoint = 100;
int lid_pwm_count;

float dummy_p,dummy_I,dummy_D,Peltier_PWM_beta;
float PELTIER_PID_WINDOW = 2.0f;
float PELTIER_PID_SMALL_WINDOW = 2.0f;
float PELTIER_TEMP_IIR_ALFA  =    0.9;
float extra_temperature_2      = 0;
float extra_temperature_1      = 0;
uint8_t peltier_PID_state = 0;
int16_t x_pos;
int16_t y_pos;
volatile uint8_t update_live = 0;

int current_PWM = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
/* USER CODE BEGIN PFP */

//float get_heat_block_temperature_LM35_A0(void);
//float get_heat_block_temperature_10k_thermistor_A1(void);
void Display_temperature(float num);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  /* USER CODE BEGIN 2 */

  ILI9341_Init();
  ILI9341_FillScreen(BLACK);
  ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
  //ILI9341_DrawText("HELLO WORLD", FONT4, 0, 200, YELLOW, BLACK);
  //ILI9341_DrawPixel(160,120,RED);

  float temp =0.5;
  float pre_block_cur_temp;

  HAL_Delay(100);
  //SSD1306_Init();  // initialise

  TIM1->CCR1 = 0;
  TIM1->CCR2 = 0;


  Device_start_buzzer();
  display_big_message("WELPCR", "----tagline----",big_font,2);
  //OLED_PCR_config_GUI_init();

  /*SSD1306_GotoXY (0,0);
  SSD1306_Puts ("WELPCR", &Font_11x18, 1);
  SSD1306_UpdateScreen(); //display*/
  //HAL_Delay(1000);
  //SSD1306_Clear();
  get_PCR_settings(&PCR_settings);
  annealing_temp = (*PCR_settings).annealing_temp;










  block_cur_temp = get_thermister_temp(&hadc1, BLOCK_TEMPERATURE_PIN);

  //start_thermocycler();


  PIDController LID_pid = { LID_PID_KP, LID_PID_KI, LID_PID_KD,
	                    LID_PID_TAU,
	                    LID_PID_LIM_MIN, LID_PID_LIM_MAX,
	                    LID_PID_LIM_MIN_INT,
	                    LID_PID_LIM_MAX_INT,
	                    LID_SAMPLE_TIME_S,
						LID_PID_BETA
  	  	  	  	  	  };

   PIDController_Init(&LID_pid);

   PIDController peltier_pid = { PELTIER_PID_KP, PELTIER_PID_KI, PELTIER_PID_KD,
		   	   	   	    PELTIER_PID_TAU,
						PELTIER_PID_LIM_MIN, PELTIER_PID_LIM_MAX,
						PELTIER_PID_LIM_MIN_INT,
						PELTIER_PID_LIM_MAX_INT,
						PELTIER_SAMPLE_TIME_S,
						PELTIER_PID_BETA
   	  	  	  	  	  };

    PIDController_Init(&peltier_pid);


    thermistor_t block_thermistor ={
       					BLOCK_THERMISTOR_R0,
   						BLOCK_THERMISTOR_RS,
   						BLOCK_THERMISTOR_BETA,
   						BLOCK_THERMISTOR_ADC,
   						BLOCK_THERMISTOR_CHANNEL
       					};

    thermistor_t EXTRA_thermistor_1 ={
       						EXTRA_1_THERMISTOR_R0,
   							EXTRA_1_THERMISTOR_RS,
   							EXTRA_1_THERMISTOR_BETA,
   							EXTRA_1_THERMISTOR_ADC,
   							EXTRA_1_THERMISTOR_CHANNEL
           					};

     thermistor_t EXTRA_thermistor_2 ={
       		                EXTRA_2_THERMISTOR_R0,
   							EXTRA_2_THERMISTOR_RS,
   							EXTRA_2_THERMISTOR_BETA,
   							EXTRA_2_THERMISTOR_ADC,
   							EXTRA_2_THERMISTOR_CHANNEL
           					};

     thermistor_t LID_thermistor ={
               					BLOCK_THERMISTOR_R0,
           						BLOCK_THERMISTOR_RS,
           						BLOCK_THERMISTOR_BETA,
           						BLOCK_THERMISTOR_ADC,
   								LID_TEMPERATURE_PIN
               					};






   TIM1->CCR3 =lid_pwm_count;



   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);





  HAL_TIM_Base_Start_IT(&htim2);










  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  /*-------------Extra temperature------------*/


	  get_thermistor_temp_(&EXTRA_thermistor_1);
	   extra_temperature_1 =EXTRA_thermistor_1.temperature;
	  get_thermistor_temp_(&EXTRA_thermistor_2);
	  extra_temperature_2 =EXTRA_thermistor_2.temperature;

	//----------------------------------------------------------

	  update_GUI();

	  check_touch();

	  /*---calulate temperature and add IIR filter*/
	  //pre_block_cur_temp = block_cur_temp;
	  float avg_block_cur_temp_1 = 10;
	  float avg_block_cur_temp_2 = 0;
	  float small_window_kp;
	  while (ABS(avg_block_cur_temp_1 - avg_block_cur_temp_2) > 0.5)
	  {
		  avg_block_cur_temp_1 = 0;
		  avg_block_cur_temp_2 = 0;
		  for (int i=0; i< PELTIER_NO_OF_SAMPLES_AVG; i++)
		  	  {
			  	  	  get_thermistor_temp_(&block_thermistor);
			  		  avg_block_cur_temp_1 +=  block_thermistor.temperature/ PELTIER_NO_OF_SAMPLES_AVG;
		  	  }
		  for (int i=0; i< PELTIER_NO_OF_SAMPLES_AVG; i++)
		  	  {
			  	  	  get_thermistor_temp_(&block_thermistor);
			  		  avg_block_cur_temp_2 += block_thermistor.temperature/ PELTIER_NO_OF_SAMPLES_AVG;
		  	  }

	  }

	/*  if(ABS(pre_block_cur_temp- avg_block_cur_temp) >0)
	  {

		  block_cur_temp = pre_block_cur_temp;
	  }
	  else
	  {
		  block_cur_temp =avg_block_cur_temp;
	  }
*/
	  block_cur_temp =avg_block_cur_temp_1;
	  IIR_block_cur_temp = (1- PELTIER_TEMP_IIR_ALFA)* block_cur_temp + PELTIER_TEMP_IIR_ALFA*IIR_block_cur_temp;


	  if(update_live)
	  {
		  update_live_par();
		  update_live=0;
	  }

	  /*--------------------------------------------*/


	       get_thermistor_temp_(&LID_thermistor);
	  	  lid_cur_temp   =  LID_thermistor.temperature;
	  	  block_cur_error    = block_set_temp -  IIR_block_cur_temp;


	  set_point = get_block_set_point();
	  block_set_temp = set_point;         // Comment this line to set temp manually


	  if(Lid_setpoint-lid_cur_temp < 40 && Lid_setpoint-lid_cur_temp > 35)
	  	  {
	  		  //PIDController_Update(&LID_pid, Lid_setpoint, lid_cur_temp);

		      TIM1->CCR3    = 0;
		      lid_pwm_count = 0;
	  	  }

	  else if(Lid_setpoint-lid_cur_temp > 2)
	  {
		  //PIDController_Update(&LID_pid, Lid_setpoint, lid_cur_temp);

		  TIM1->CCR3 =LID_PWM_MAX;
		  lid_pwm_count =LID_PWM_MAX;
	  }
	  else

	  {
		  TIM1->CCR3    = 0;
		  lid_pwm_count = 0;
	  }
	  


	  /************************Peltier PID*******************************************/
/*
	  TIM1->CCR1 = block_PWM;
	  TIM1->CCR2 = block_PWM;
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

*/

	  if(block_set_temp < 65)
	  {
		  Peltier_PWM_beta = 8.2 * block_set_temp - 300;//282;//276; //9.6959 * block_set_temp - 252.48;
	  	  PELTIER_PID_WINDOW = 10;
	  	  peltier_pid.Kp = 75;
	  	  peltier_pid.Ki = 0;
	  	  peltier_pid.Kd = 0;//150
	  	  PELTIER_PID_SMALL_WINDOW = 3;
	  	  small_window_kp = 50;
	  }
	  else if(block_set_temp < 85)
	  {
		  Peltier_PWM_beta = 8.4522 * block_set_temp - 292;//11.285 * block_set_temp - 366.95;// 11.285 366.95
		  PELTIER_PID_WINDOW = 10;
		  peltier_pid.Kp = 10;
		  peltier_pid.Ki = 0;
		  peltier_pid.Kd = 200;//200;
		  PELTIER_PID_SMALL_WINDOW = 0.5;
		  small_window_kp = 75;
	  }
	  else if(block_set_temp < 150)
	  {
	  	  Peltier_PWM_beta = 9.1637 * block_set_temp - 345;
	  	  PELTIER_PID_WINDOW = 3;
	  	  peltier_pid.Kp = 15;
	  	  peltier_pid.Ki = 2;//2
	  	  peltier_pid.Kd =100;// 125;//125;
	  	  PELTIER_PID_SMALL_WINDOW = 0.5;
	  	  small_window_kp = 100;//100;
	  }

	 if(ABS(block_set_temp - IIR_block_cur_temp) <  PELTIER_PID_WINDOW)
	  	  {

		 	 if (ABS(block_set_temp - IIR_block_cur_temp) <  PELTIER_PID_WINDOW)
		 	 {
		 		peltier_pid.Kp = small_window_kp;
		 	 }


		      if (peltier_PID_state != 3 )
		     		  {
		     			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
		     			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		     			  HAL_Delay(100);
		     			 peltier_PID_state = 3;
		     		  }


		  	  peltier_pid.beta = Peltier_PWM_beta;
			  PIDController_Update(&peltier_pid, block_set_temp, IIR_block_cur_temp);
			  dummy_p = peltier_pid.proportional_;
			  dummy_I = peltier_pid.integrator;
			  dummy_D = peltier_pid.differentiator;


			  if ( (block_pre_out>0 && peltier_pid.out < 0) || (block_pre_out<0 && peltier_pid.out > 0) )
			  {
				  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
				  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
				  HAL_Delay(5);
			  }

		  	  //block_PWM = (int) peltier_pid.out;
			   block_PWM = 0.8*(int) peltier_pid.out + 0.2 * block_PWM;
			  if (peltier_pid.out > 0)
			  {
				  TIM1->CCR1 = ABS(peltier_pid.out);
				  TIM1->CCR2 = ABS(peltier_pid.out);
				  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
				  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			  }
			  else if (peltier_pid.out < 0)
			  {
				  TIM1->CCR1 = ABS(peltier_pid.out);
				  TIM1->CCR2 = ABS(peltier_pid.out);
				  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
				  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			  }

			  block_pre_out =peltier_pid.out;
			  current_PWM = peltier_pid.out;

	  	  }
	  else if((block_set_temp-IIR_block_cur_temp) > PELTIER_PID_WINDOW)
	  {


		  if (peltier_PID_state != 1 )
		  {
			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			  HAL_Delay(100);
			  peltier_PID_state = 1;
		  }




		  if (current_PWM < PELTIER_PID_LIM_MAX)
		  {
			  current_PWM = current_PWM+50;
			  if(current_PWM > PELTIER_PID_LIM_MAX)
			  {
				  current_PWM = PELTIER_PID_LIM_MAX;
			  }
		  }




		  TIM1->CCR1 = current_PWM;
		  TIM1->CCR2 = current_PWM;

		  if(current_PWM> 0)
		  {
			  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
			  HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_1);
		  }
		  else
		  {
			  HAL_TIM_PWM_Stop (&htim1,  TIM_CHANNEL_2);
			  HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_1);

		  }
		  PIDController_Init(&peltier_pid);

		  block_PWM = current_PWM; //(int) PELTIER_PID_LIM_MAX;
		  //block_PWM = 0.5*(int) PELTIER_PID_LIM_MAX + 0.5 * block_PWM;
	  }
	  else if((block_set_temp-IIR_block_cur_temp)< (-PELTIER_PID_WINDOW))
	  	  {
		  /*if (peltier_PID_state != 2 )
		  		  {
		  			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
		  			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  			  HAL_Delay(100);
		  			  peltier_PID_state = 2;
		  		  }
          */
		  if (current_PWM > PELTIER_PID_LIM_MIN)
		  		  {
		  			  current_PWM = current_PWM-50;
		  			  if(current_PWM < PELTIER_PID_LIM_MIN)
		  			  {
		  				  current_PWM = PELTIER_PID_LIM_MIN;
		  			  }
		  		  }






	  		  TIM1->CCR1 = ABS(current_PWM);//ABS(PELTIER_PID_LIM_MIN);
	  		  TIM1->CCR2 = ABS(current_PWM);//ABS(PELTIER_PID_LIM_MIN);

	  		if(current_PWM> 0)
	  				  {
	  					  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	  					  HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_1);
	  				  }
	  				  else
	  				  {
	  					  HAL_TIM_PWM_Stop (&htim1,  TIM_CHANNEL_2);
	  					  HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_1);

	  				  }

	  		  PIDController_Init(&peltier_pid);

	  		  block_PWM = current_PWM; //(int) PELTIER_PID_LIM_MIN;
	  		//block_PWM = 0.5*(int) PELTIER_PID_LIM_MIN + 0.5 * block_PWM;
	  	  }
	  else
	  {
		  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
		  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

		  PIDController_Init(&peltier_pid);

		  block_PWM = 0;
		  //block_PWM =  0.5 * block_PWM;

	  }


	  HAL_Delay(100);



/*
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	  HAL_Delay(100);*/
/*
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	  HAL_Delay(6000);*/


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 140;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 0;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 32000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2250;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_DC_Pin|LCD_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TOUCH_CS_Pin|STATUS_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BUZZER_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : J_UP_Pin J_DOWN_Pin J_LEFT_Pin J_RIGHT_Pin
                           J_MID_Pin J_SET_Pin J_RST_Pin */
  GPIO_InitStruct.Pin = J_UP_Pin|J_DOWN_Pin|J_LEFT_Pin|J_RIGHT_Pin
                          |J_MID_Pin|J_SET_Pin|J_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DC_Pin TOUCH_CS_Pin STATUS_LED_Pin LCD_RESET_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin|TOUCH_CS_Pin|STATUS_LED_Pin|LCD_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : TOUCH_IRQ_Pin */
  GPIO_InitStruct.Pin = TOUCH_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TOUCH_IRQ_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	update_live = 1;



}

void update_live_par(void)
{
	char num_string2[10];
	static float block_previous_temp  = 25;
	float ramp_rate;

	ramp_rate = (IIR_block_cur_temp - block_previous_temp)/1;
	//sprintf(num_string2,"Rate: %0.1f",ramp_rate);
	update_parameter_value(RAMP_RATE,  ramp_rate);
	update_parameter_value(BLOCK_TEAMPERATURE,  IIR_block_cur_temp);
	update_parameter_value(SET_POINT,  set_point);
	update_parameter_value(LID_TEMP,  lid_cur_temp);
	update_parameter_value(TEMP_1,  extra_temperature_1);
	update_parameter_value(TEMP_2,  extra_temperature_2);
	update_parameter_value(TIME_REMAINING,  0);


	ILI9341_DrawLine(250, 0, 245, 15, WHITE);//slash between cycles
	block_previous_temp = IIR_block_cur_temp;

	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
}

void Display_temperature(float num)
{
	//sprintf(num_string,"Temp:%0.1f",num);

	//SSD1306_Clear();
	//HAL_TIM_Base_Stop_IT(&htim2);
	update_parameter_value(BLOCK_TEAMPERATURE,  num);
	if(OLED_lock == 0)
		{

		/*OLED_lock =1;
			SSD1306_GotoXY (0,0);
			SSD1306_Puts (num_string, &Font_11x18, 1);
			SSD1306_UpdateScreen(); //display

			OLED_lock =0;*/
		}

	//HAL_TIM_Base_Start_IT(&htim2);
}

void oled_print_int(int num)
{
	sprintf(num_string,"%d",num);

		SSD1306_Clear();
		SSD1306_GotoXY (0,0);
		SSD1306_Puts (num_string, &Font_16x26, 1);
		SSD1306_UpdateScreen(); //display
}

void check_touch(void)
{
	if (!HAL_GPIO_ReadPin(touch_IRQ_port,touch_IRQ_pin ))
	{
		ili9341_touch_coordinate(&x_pos, &y_pos);
	}
	else
	{
		x_pos = -1;
		y_pos = -1;
	}
		//HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	//HAL_Delay(1000);
}

void test_start_buzzer(void)
{
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);


}



void Device_start_buzzer(void)
{
	for(int i = 0; i<3;i++ )
	{
		HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_SET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_RESET);
		HAL_Delay(50);
	}

	for(int i = 0; i<3;i++ )
		{
			HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
		}
	for(int i = 0; i<2;i++ )
			{
				HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_SET);
				HAL_Delay(200);
				HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_RESET);
				HAL_Delay(200);
			}


}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
