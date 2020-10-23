#include "stm32h7xx_hal.h"
#include "main.h"
#include "front_unit.h"
#include "lcd.h"
#include "trx_manager.h"
#include "settings.h"
#include "system_menu.h"
#include "functions.h"
#include "audio_filters.h"
#include "agc.h"

static void FRONTPANEL_ENCODER_Rotated(int8_t direction);
static void FRONTPANEL_ENCODER2_Rotated(int8_t direction);
static uint16_t FRONTPANEL_ReadMCP3008_Value(uint8_t channel, GPIO_TypeDef *CS_PORT, uint16_t CS_PIN);
static void FRONTPANEL_ENCODER2_Rotated(int8_t direction);

static void FRONTPANEL_BUTTONHANDLER_DOUBLE(void);
static void FRONTPANEL_BUTTONHANDLER_DOUBLEMODE(void);
static void FRONTPANEL_BUTTONHANDLER_AsB(void);
static void FRONTPANEL_BUTTONHANDLER_ArB(void);
static void FRONTPANEL_BUTTONHANDLER_TUNE(void);
static void FRONTPANEL_BUTTONHANDLER_PRE(void);
static void FRONTPANEL_BUTTONHANDLER_ATT(void);
static void FRONTPANEL_BUTTONHANDLER_ATTHOLD(void);
static void FRONTPANEL_BUTTONHANDLER_ANT(void);
static void FRONTPANEL_BUTTONHANDLER_PGA(void);
static void FRONTPANEL_BUTTONHANDLER_FAST(void);
static void FRONTPANEL_BUTTONHANDLER_MODE_P(void);
static void FRONTPANEL_BUTTONHANDLER_MODE_N(void);
static void FRONTPANEL_BUTTONHANDLER_BAND_P(void);
static void FRONTPANEL_BUTTONHANDLER_BAND_N(void);
static void FRONTPANEL_BUTTONHANDLER_RF_POWER(void);
static void FRONTPANEL_BUTTONHANDLER_SQUELCH(void);
static void FRONTPANEL_BUTTONHANDLER_AGC(void);
static void FRONTPANEL_BUTTONHANDLER_AGC_SPEED(void);
static void FRONTPANEL_BUTTONHANDLER_WPM(void);
static void FRONTPANEL_BUTTONHANDLER_KEYER(void);
static void FRONTPANEL_BUTTONHANDLER_DNR(void);
static void FRONTPANEL_BUTTONHANDLER_NB(void);
static void FRONTPANEL_BUTTONHANDLER_BW(void);
static void FRONTPANEL_BUTTONHANDLER_HPF(void);
static void FRONTPANEL_BUTTONHANDLER_NOTCH(void);
static void FRONTPANEL_BUTTONHANDLER_NOTCH_MANUAL(void);
static void FRONTPANEL_BUTTONHANDLER_SHIFT(void);
static void FRONTPANEL_BUTTONHANDLER_CLAR(void);
static void FRONTPANEL_BUTTONHANDLER_LOCK(void);
static void FRONTPANEL_BUTTONHANDLER_SERVICES(void);
static void FRONTPANEL_BUTTONHANDLER_MENU(void);
static void FRONTPANEL_BUTTONHANDLER_MUTE(void);
static void FRONTPANEL_BUTTONHANDLER_STEP(void);
static void FRONTPANEL_BUTTONHANDLER_BANDMAP(void);

static bool FRONTPanel_MCP3008_1_Enabled = true;
static bool FRONTPanel_MCP3008_2_Enabled = true;
static bool FRONTPanel_MCP3008_3_Enabled = true;

static int32_t ENCODER_slowler = 0;
static uint32_t ENCODER_AValDeb = 0;
static uint32_t ENCODER2_AValDeb = 0;
static bool ENCODER2_SWLast = true;

PERIPH_FrontPanel_Button PERIPH_FrontPanel_Buttons[] = {
	{.port = 1, .channel = 7, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_PRE, .holdHandler = FRONTPANEL_BUTTONHANDLER_PGA},		  //PRE-PGA
	{.port = 1, .channel = 6, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_ATT, .holdHandler = FRONTPANEL_BUTTONHANDLER_ATTHOLD},	  //ATT-ATTHOLD
	{.port = 1, .channel = 5, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_MUTE, .holdHandler = NULL},								  //MUTE-SCAN
	{.port = 1, .channel = 4, .state = false, .prev_state = false, .work_in_menu = true, .clickHandler = FRONTPANEL_BUTTONHANDLER_AGC, .holdHandler = FRONTPANEL_BUTTONHANDLER_AGC_SPEED},	  //AGC-AGCSPEED
	{.port = 1, .channel = 3, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_ArB, .holdHandler = FRONTPANEL_BUTTONHANDLER_ANT},		  //A=B-ANT
	{.port = 1, .channel = 2, .state = false, .prev_state = false, .work_in_menu = true, .clickHandler = FRONTPANEL_BUTTONHANDLER_TUNE, .holdHandler = FRONTPANEL_BUTTONHANDLER_TUNE},		  //TUNE
	{.port = 1, .channel = 1, .state = false, .prev_state = false, .work_in_menu = true, .clickHandler = FRONTPANEL_BUTTONHANDLER_RF_POWER, .holdHandler = FRONTPANEL_BUTTONHANDLER_SQUELCH}, //RFPOWER-SQUELCH
	{.port = 1, .channel = 0, .state = false, .prev_state = false, .work_in_menu = true, .clickHandler = FRONTPANEL_BUTTONHANDLER_BW, .holdHandler = FRONTPANEL_BUTTONHANDLER_HPF},			  //BW-HPF

	//{ .port = 2, .channel = 7, }, //SHIFT
	//{ .port = 2, .channel = 6, }, //AF GAIN
	{.port = 2, .channel = 5, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_DNR, .holdHandler = FRONTPANEL_BUTTONHANDLER_NB},			 //DNR-NB
	{.port = 2, .channel = 4, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_NOTCH, .holdHandler = FRONTPANEL_BUTTONHANDLER_NOTCH_MANUAL}, //NOTCH-MANUAL
	{.port = 2, .channel = 3, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_CLAR, .holdHandler = FRONTPANEL_BUTTONHANDLER_SHIFT},		 //CLAR-SHIFT
	{.port = 2, .channel = 2, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = NULL, .holdHandler = NULL},															 //REC-PLAY
	{.port = 2, .channel = 1, .state = false, .prev_state = false, .work_in_menu = true, .clickHandler = FRONTPANEL_BUTTONHANDLER_SERVICES, .holdHandler = FRONTPANEL_BUTTONHANDLER_SERVICES},															 //SERVICES
	{.port = 2, .channel = 0, .state = false, .prev_state = false, .work_in_menu = true, .clickHandler = FRONTPANEL_BUTTONHANDLER_MENU, .holdHandler = FRONTPANEL_BUTTONHANDLER_LOCK},			 //MENU-LOCK

	{.port = 3, .channel = 7, .state = false, .prev_state = false, .work_in_menu = true, .clickHandler = FRONTPANEL_BUTTONHANDLER_WPM, .holdHandler = FRONTPANEL_BUTTONHANDLER_KEYER},			//WPM-KEYER
	{.port = 3, .channel = 6, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_DOUBLE, .holdHandler = FRONTPANEL_BUTTONHANDLER_DOUBLEMODE}, //DOUBLE-&+
	{.port = 3, .channel = 5, .state = false, .prev_state = false, .work_in_menu = true, .clickHandler = FRONTPANEL_BUTTONHANDLER_FAST, .holdHandler = FRONTPANEL_BUTTONHANDLER_STEP},			//FAST-FASTSETT
	{.port = 3, .channel = 4, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_BAND_N, .holdHandler = FRONTPANEL_BUTTONHANDLER_BAND_N},		//BAND-
	{.port = 3, .channel = 3, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_BAND_P, .holdHandler = FRONTPANEL_BUTTONHANDLER_BAND_P},		//BAND+
	{.port = 3, .channel = 2, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_MODE_P, .holdHandler = FRONTPANEL_BUTTONHANDLER_MODE_P},		//MODE+
	{.port = 3, .channel = 1, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_MODE_N, .holdHandler = FRONTPANEL_BUTTONHANDLER_MODE_N},		//MODE-
	{.port = 3, .channel = 0, .state = false, .prev_state = false, .work_in_menu = false, .clickHandler = FRONTPANEL_BUTTONHANDLER_AsB, .holdHandler = FRONTPANEL_BUTTONHANDLER_BANDMAP},		//A/B-BANDMAP
};

void FRONTPANEL_ENCODER_checkRotate(void)
{
	static uint8_t ENClastClkVal = 0;
	static bool ENCfirst = true;
	uint8_t ENCODER_DTVal = HAL_GPIO_ReadPin(ENC_DT_GPIO_Port, ENC_DT_Pin);
	uint8_t ENCODER_CLKVal = HAL_GPIO_ReadPin(ENC_CLK_GPIO_Port, ENC_CLK_Pin);

	if (ENCfirst)
	{
		ENClastClkVal = ENCODER_CLKVal;
		ENCfirst = false;
	}
	if ((HAL_GetTick() - ENCODER_AValDeb) < CALIBRATE.ENCODER_DEBOUNCE)
		return;

	if (ENClastClkVal != ENCODER_CLKVal)
	{
		if (!CALIBRATE.ENCODER_ON_FALLING || ENCODER_CLKVal == 0)
		{
			if (ENCODER_DTVal != ENCODER_CLKVal)
			{ // If pin A changed first - clockwise rotation
				ENCODER_slowler--;
				if (ENCODER_slowler <= -CALIBRATE.ENCODER_SLOW_RATE)
				{
					FRONTPANEL_ENCODER_Rotated(CALIBRATE.ENCODER_INVERT ? 1 : -1);
					ENCODER_slowler = 0;
				}
			}
			else
			{ // otherwise B changed its state first - counterclockwise rotation
				ENCODER_slowler++;
				if (ENCODER_slowler >= CALIBRATE.ENCODER_SLOW_RATE)
				{
					FRONTPANEL_ENCODER_Rotated(CALIBRATE.ENCODER_INVERT ? -1 : 1);
					ENCODER_slowler = 0;
				}
			}
		}
		ENCODER_AValDeb = HAL_GetTick();
		ENClastClkVal = ENCODER_CLKVal;
	}
}

void FRONTPANEL_ENCODER2_checkRotate(void)
{
	uint8_t ENCODER2_DTVal = HAL_GPIO_ReadPin(ENC2_DT_GPIO_Port, ENC2_DT_Pin);
	uint8_t ENCODER2_CLKVal = HAL_GPIO_ReadPin(ENC2_CLK_GPIO_Port, ENC2_CLK_Pin);

	if ((HAL_GetTick() - ENCODER2_AValDeb) < CALIBRATE.ENCODER2_DEBOUNCE)
		return;

	if (!CALIBRATE.ENCODER_ON_FALLING || ENCODER2_CLKVal == 0)
	{
		if (ENCODER2_DTVal != ENCODER2_CLKVal)
		{ // If pin A changed first - clockwise rotation
			FRONTPANEL_ENCODER2_Rotated(CALIBRATE.ENCODER2_INVERT ? 1 : -1);
		}
		else
		{ // otherwise B changed its state first - counterclockwise rotation
			FRONTPANEL_ENCODER2_Rotated(CALIBRATE.ENCODER2_INVERT ? -1 : 1);
		}
	}
	ENCODER2_AValDeb = HAL_GetTick();
}

static void FRONTPANEL_ENCODER_Rotated(int8_t direction) // rotated encoder, handler here, direction -1 - left, 1 - right
{
	if (TRX.Locked)
		return;

	if (LCD_systemMenuOpened)
	{
		eventRotateSystemMenu(direction);
		return;
	}
	VFO *vfo = CurrentVFO();
	if (TRX.Fast)
	{
		TRX_setFrequency((uint32_t)((int32_t)vfo->Freq + ((int32_t)TRX.FRQ_FAST_STEP * direction)), vfo);
		if ((vfo->Freq % TRX.FRQ_FAST_STEP) > 0)
			TRX_setFrequency(vfo->Freq / TRX.FRQ_FAST_STEP * TRX.FRQ_FAST_STEP, vfo);
	}
	else
	{
		TRX_setFrequency((uint32_t)((int32_t)vfo->Freq + ((int32_t)TRX.FRQ_STEP * direction)), vfo);
		if ((vfo->Freq % TRX.FRQ_STEP) > 0)
			TRX_setFrequency(vfo->Freq / TRX.FRQ_STEP * TRX.FRQ_STEP, vfo);
	}
	LCD_UpdateQuery.FreqInfo = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_ENCODER2_Rotated(int8_t direction) // rotated encoder, handler here, direction -1 - left, 1 - right
{
	if (TRX.Locked)
		return;

	if (LCD_systemMenuOpened)
	{
		eventSecRotateSystemMenu(direction);
		return;
	}

	//NOTCH - default action
	if (CurrentVFO()->ManualNotchFilter)
	{
		if (CurrentVFO()->NotchFC > 50 && direction < 0)
			CurrentVFO()->NotchFC -= 25;
		else if (CurrentVFO()->NotchFC < CurrentVFO()->LPF_Filter_Width && direction > 0)
			CurrentVFO()->NotchFC += 25;
		LCD_UpdateQuery.StatusInfoGUI = true;
		NeedReinitNotch = true;
	}
	else
	{
		VFO *vfo = CurrentVFO();
		if (TRX.Fast)
		{
			TRX_setFrequency((uint32_t)((int32_t)vfo->Freq + (int32_t)TRX.FRQ_ENC_FAST_STEP * direction), vfo);
			if ((vfo->Freq % TRX.FRQ_ENC_FAST_STEP) > 0)
				TRX_setFrequency(vfo->Freq / TRX.FRQ_ENC_FAST_STEP * TRX.FRQ_ENC_FAST_STEP, vfo);
		}
		else
		{
			TRX_setFrequency((uint32_t)((int32_t)vfo->Freq + (int32_t)TRX.FRQ_ENC_STEP * direction), vfo);
			if ((vfo->Freq % TRX.FRQ_ENC_STEP) > 0)
				TRX_setFrequency(vfo->Freq / TRX.FRQ_ENC_STEP * TRX.FRQ_ENC_STEP, vfo);
		}
		LCD_UpdateQuery.FreqInfo = true;
	}
}

void FRONTPANEL_ENCODER2_checkSwitch(void)
{
	if (TRX.Locked)
		return;

	bool ENCODER2_SWNow = HAL_GPIO_ReadPin(ENC2_SW_GPIO_Port, ENC2_SW_Pin);
	if (ENCODER2_SWLast != ENCODER2_SWNow)
	{
		ENCODER2_SWLast = ENCODER2_SWNow;
		if (!ENCODER2_SWNow)
		{
			//ENC2 CLICK
			NeedReinitNotch = true;
			LCD_UpdateQuery.StatusInfoGUI = true;
			LCD_UpdateQuery.TopButtons = true;
			NeedSaveSettings = true;
		}
	}
}

void FRONTPANEL_Init(void)
{
	uint16_t test_value = FRONTPANEL_ReadMCP3008_Value(0, AD1_CS_GPIO_Port, AD1_CS_Pin);
	if (test_value == 65535)
	{
		FRONTPanel_MCP3008_1_Enabled = false;
		sendToDebug_strln("[ERR] Frontpanel MCP3008 - 1 not found, disabling... (FPGA SPI/I2S CLOCK ERROR?)");
		LCD_showError("MCP3008 - 1 init error (FPGA I2S CLK?)", true);
	}
	test_value = FRONTPANEL_ReadMCP3008_Value(0, AD2_CS_GPIO_Port, AD2_CS_Pin);
	if (test_value == 65535)
	{
		FRONTPanel_MCP3008_2_Enabled = false;
		sendToDebug_strln("[ERR] Frontpanel MCP3008 - 2 not found, disabling... (FPGA SPI/I2S CLOCK ERROR?)");
		LCD_showError("MCP3008 - 2 init error", true);
	}
	test_value = FRONTPANEL_ReadMCP3008_Value(0, AD3_CS_GPIO_Port, AD3_CS_Pin);
	if (test_value == 65535)
	{
		FRONTPanel_MCP3008_3_Enabled = false;
		sendToDebug_strln("[ERR] Frontpanel MCP3008 - 3 not found, disabling... (FPGA SPI/I2S CLOCK ERROR?)");
		LCD_showError("MCP3008 - 3 init error", true);
	}
	FRONTPANEL_Process();
}

void FRONTPANEL_Process(void)
{
	if (SPI_process)
		return;
	SPI_process = true;

	FRONTPANEL_ENCODER2_checkSwitch();
	uint16_t buttons_count = sizeof(PERIPH_FrontPanel_Buttons) / sizeof(PERIPH_FrontPanel_Button);
	uint16_t mcp3008_value = 0;

	//process regulators
	if (FRONTPanel_MCP3008_2_Enabled)
	{
		// AF_GAIN
		mcp3008_value = FRONTPANEL_ReadMCP3008_Value(6, AD2_CS_GPIO_Port, AD2_CS_Pin);
		TRX_Volume = (uint16_t)(1023.0f - mcp3008_value);

		// SHIFT or IF Gain
		mcp3008_value = FRONTPANEL_ReadMCP3008_Value(7, AD2_CS_GPIO_Port, AD2_CS_Pin);
		if (TRX.ShiftEnabled)
		{
			int_fast16_t TRX_SHIFT_old = TRX_SHIFT;
			TRX_SHIFT = (int_fast16_t)(((1023.0f - mcp3008_value) * TRX.SHIFT_INTERVAL * 2 / 1023.0f) - TRX.SHIFT_INTERVAL);
			if(TRX_SHIFT_old != TRX_SHIFT)
				TRX_setFrequency(CurrentVFO()->Freq, CurrentVFO());
		}
		else
		{
			TRX_SHIFT = 0;
			TRX.IF_Gain = (uint8_t)(0.0f + ((1023.0f - mcp3008_value) * 50.0f / 1023.0f));
		}
	}

	//process buttons
	for (uint16_t b = 0; b < buttons_count; b++)
	{
		//check disabled ports
		if (PERIPH_FrontPanel_Buttons[b].port == 1 && !FRONTPanel_MCP3008_1_Enabled)
			continue;
		if (PERIPH_FrontPanel_Buttons[b].port == 2 && !FRONTPanel_MCP3008_2_Enabled)
			continue;
		if (PERIPH_FrontPanel_Buttons[b].port == 3 && !FRONTPanel_MCP3008_3_Enabled)
			continue;

		//get state from ADC MCP3008 (10bit - 1024values)
		if (PERIPH_FrontPanel_Buttons[b].port == 1)
			mcp3008_value = FRONTPANEL_ReadMCP3008_Value(PERIPH_FrontPanel_Buttons[b].channel, AD1_CS_GPIO_Port, AD1_CS_Pin);
		if (PERIPH_FrontPanel_Buttons[b].port == 2)
			mcp3008_value = FRONTPANEL_ReadMCP3008_Value(PERIPH_FrontPanel_Buttons[b].channel, AD2_CS_GPIO_Port, AD2_CS_Pin);
		if (PERIPH_FrontPanel_Buttons[b].port == 3)
			mcp3008_value = FRONTPANEL_ReadMCP3008_Value(PERIPH_FrontPanel_Buttons[b].channel, AD3_CS_GPIO_Port, AD3_CS_Pin);

		//set state
		if (mcp3008_value < MCP3008_THRESHOLD)
			PERIPH_FrontPanel_Buttons[b].state = true;
		else
			PERIPH_FrontPanel_Buttons[b].state = false;

		//check state
		if ((PERIPH_FrontPanel_Buttons[b].prev_state != PERIPH_FrontPanel_Buttons[b].state) && PERIPH_FrontPanel_Buttons[b].state)
		{
			PERIPH_FrontPanel_Buttons[b].start_hold_time = HAL_GetTick();
			PERIPH_FrontPanel_Buttons[b].afterhold = false;
		}

		//check hold state
		if ((PERIPH_FrontPanel_Buttons[b].prev_state == PERIPH_FrontPanel_Buttons[b].state) && PERIPH_FrontPanel_Buttons[b].state && ((HAL_GetTick() - PERIPH_FrontPanel_Buttons[b].start_hold_time) > KEY_HOLD_TIME) && !PERIPH_FrontPanel_Buttons[b].afterhold)
		{
			PERIPH_FrontPanel_Buttons[b].afterhold = true;
			if (!TRX.Locked || (PERIPH_FrontPanel_Buttons[b].port == 2 && PERIPH_FrontPanel_Buttons[b].channel == 0)) //LOCK BUTTON
				if (!LCD_systemMenuOpened || PERIPH_FrontPanel_Buttons[b].work_in_menu)
					if (PERIPH_FrontPanel_Buttons[b].holdHandler != NULL)
						PERIPH_FrontPanel_Buttons[b].holdHandler();
		}

		//check click state
		if ((PERIPH_FrontPanel_Buttons[b].prev_state != PERIPH_FrontPanel_Buttons[b].state) && !PERIPH_FrontPanel_Buttons[b].state && ((HAL_GetTick() - PERIPH_FrontPanel_Buttons[b].start_hold_time) < KEY_HOLD_TIME) && !PERIPH_FrontPanel_Buttons[b].afterhold && !TRX.Locked)
		{
			if (!LCD_systemMenuOpened || PERIPH_FrontPanel_Buttons[b].work_in_menu)
				if (PERIPH_FrontPanel_Buttons[b].clickHandler != NULL)
					PERIPH_FrontPanel_Buttons[b].clickHandler();
		}

		//save prev state
		PERIPH_FrontPanel_Buttons[b].prev_state = PERIPH_FrontPanel_Buttons[b].state;
	}
	SPI_process = false;
}

static void FRONTPANEL_BUTTONHANDLER_DOUBLE(void)
{
	TRX.Dual_RX = !TRX.Dual_RX;
	LCD_UpdateQuery.StatusInfoGUI = true;
	NeedReinitAudioFilters = true;
}

static void FRONTPANEL_BUTTONHANDLER_DOUBLEMODE(void)
{
	if (!TRX.Dual_RX)
		return;

	if (TRX.Dual_RX_Type == VFO_A_AND_B)
		TRX.Dual_RX_Type = VFO_A_PLUS_B;
	else if (TRX.Dual_RX_Type == VFO_A_PLUS_B)
		TRX.Dual_RX_Type = VFO_A_AND_B;
	LCD_UpdateQuery.StatusInfoGUI = true;
	NeedReinitAudioFilters = true;
}

static void FRONTPANEL_BUTTONHANDLER_AsB(void) // A/B
{
	TRX_TemporaryMute();
	TRX.current_vfo = !TRX.current_vfo;
	TRX_setFrequency(CurrentVFO()->Freq, CurrentVFO());
	TRX_setMode(CurrentVFO()->Mode, CurrentVFO());
	LCD_UpdateQuery.TopButtons = true;
	LCD_UpdateQuery.FreqInfo = true;
	LCD_UpdateQuery.StatusInfoGUI = true;
	NeedSaveSettings = true;
	NeedReinitAudioFilters = true;
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_TUNE(void)
{
	TRX_Tune = !TRX_Tune;
	TRX_ptt_hard = TRX_Tune;
	LCD_UpdateQuery.StatusInfoGUI = true;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
	TRX_Restart_Mode();
}

static void FRONTPANEL_BUTTONHANDLER_PRE(void)
{
	TRX.LNA = !TRX.LNA;
	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
	{
		TRX.BANDS_SAVED_SETTINGS[band].LNA = TRX.LNA;
	}
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
	TRX_AutoGain_Stage = 0;
}

static void FRONTPANEL_BUTTONHANDLER_ATT(void)
{
	TRX.ATT = !TRX.ATT;

	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
	{
		TRX.BANDS_SAVED_SETTINGS[band].ATT = TRX.ATT;
		TRX.BANDS_SAVED_SETTINGS[band].ATT_DB = TRX.ATT_DB;
	}

	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
	TRX_AutoGain_Stage = 0;
}

static void FRONTPANEL_BUTTONHANDLER_ATTHOLD(void)
{
	TRX.ATT_DB += TRX.ATT_STEP;
	if (TRX.ATT_DB > 31.0f)
		TRX.ATT_DB = TRX.ATT_STEP;

	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
	{
		TRX.BANDS_SAVED_SETTINGS[band].ATT = TRX.ATT;
		TRX.BANDS_SAVED_SETTINGS[band].ATT_DB = TRX.ATT_DB;
	}

	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
	TRX_AutoGain_Stage = 0;
}

static void FRONTPANEL_BUTTONHANDLER_ANT(void)
{
	TRX.ANT = !TRX.ANT;

	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
		TRX.BANDS_SAVED_SETTINGS[band].ANT = TRX.ANT;

	LCD_UpdateQuery.StatusInfoGUI = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_PGA(void)
{
	if (!TRX.ADC_Driver && !TRX.ADC_PGA)
	{
		TRX.ADC_Driver = true;
		TRX.ADC_PGA = false;
	}
	else if (TRX.ADC_Driver && !TRX.ADC_PGA)
	{
		TRX.ADC_Driver = true;
		TRX.ADC_PGA = true;
	}
	else if (TRX.ADC_Driver && TRX.ADC_PGA)
	{
		TRX.ADC_Driver = false;
		TRX.ADC_PGA = true;
	}
	else if (!TRX.ADC_Driver && TRX.ADC_PGA)
	{
		TRX.ADC_Driver = false;
		TRX.ADC_PGA = false;
	}
	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
	{
		TRX.BANDS_SAVED_SETTINGS[band].ADC_Driver = TRX.ADC_Driver;
		TRX.BANDS_SAVED_SETTINGS[band].ADC_PGA = TRX.ADC_PGA;
	}
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
	TRX_AutoGain_Stage = 0;
}

static void FRONTPANEL_BUTTONHANDLER_FAST(void)
{
	TRX.Fast = !TRX.Fast;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_MODE_P(void)
{
	int8_t mode = (int8_t)CurrentVFO()->Mode;
	if (mode == TRX_MODE_LSB)
		mode = TRX_MODE_USB;
	else if (mode == TRX_MODE_USB)
		mode = TRX_MODE_LSB;
	else if (mode == TRX_MODE_CW_L)
		mode = TRX_MODE_CW_U;
	else if (mode == TRX_MODE_CW_U)
		mode = TRX_MODE_CW_L;
	else if (mode == TRX_MODE_NFM)
		mode = TRX_MODE_WFM;
	else if (mode == TRX_MODE_WFM)
		mode = TRX_MODE_NFM;
	else if (mode == TRX_MODE_DIGI_L)
		mode = TRX_MODE_DIGI_U;
	else if (mode == TRX_MODE_DIGI_U)
		mode = TRX_MODE_DIGI_L;
	else if (mode == TRX_MODE_AM)
		mode = TRX_MODE_IQ;
	else if (mode == TRX_MODE_IQ)
		mode = TRX_MODE_LOOPBACK;
	else if (mode == TRX_MODE_LOOPBACK)
		mode = TRX_MODE_AM;

	TRX_setMode((uint8_t)mode, CurrentVFO());
	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
		TRX.BANDS_SAVED_SETTINGS[band].Mode = (uint8_t)mode;
	TRX_Temporary_Stop_BandMap = true;
}

static void FRONTPANEL_BUTTONHANDLER_MODE_N(void)
{
	int8_t mode = (int8_t)CurrentVFO()->Mode;
	if (mode == TRX_MODE_LSB)
		mode = TRX_MODE_CW_L;
	else if (mode == TRX_MODE_USB)
		mode = TRX_MODE_CW_U;
	else if (mode == TRX_MODE_CW_L || mode == TRX_MODE_CW_U)
		mode = TRX_MODE_DIGI_U;
	else if (mode == TRX_MODE_DIGI_L || mode == TRX_MODE_DIGI_U)
		mode = TRX_MODE_NFM;
	else if (mode == TRX_MODE_NFM || mode == TRX_MODE_WFM)
		mode = TRX_MODE_AM;
	else
	{
		if (CurrentVFO()->Freq < 10000000)
			mode = TRX_MODE_LSB;
		else
			mode = TRX_MODE_USB;
	}

	TRX_setMode((uint8_t)mode, CurrentVFO());
	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
		TRX.BANDS_SAVED_SETTINGS[band].Mode = (uint8_t)mode;
	TRX_Temporary_Stop_BandMap = true;
}

static void FRONTPANEL_BUTTONHANDLER_BAND_P(void)
{
	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	band++;
	if (band >= BANDS_COUNT)
		band = 0;
	while (!BANDS[band].selectable)
	{
		band++;
		if (band >= BANDS_COUNT)
			band = 0;
	}

	TRX_setFrequency(TRX.BANDS_SAVED_SETTINGS[band].Freq, CurrentVFO());
	TRX_setMode(TRX.BANDS_SAVED_SETTINGS[band].Mode, CurrentVFO());
	TRX.LNA = TRX.BANDS_SAVED_SETTINGS[band].LNA;
	TRX.ATT = TRX.BANDS_SAVED_SETTINGS[band].ATT;
	TRX.ATT_DB = TRX.BANDS_SAVED_SETTINGS[band].ATT_DB;
	TRX.ADC_Driver = TRX.BANDS_SAVED_SETTINGS[band].ADC_Driver;
	TRX.FM_SQL_threshold = TRX.BANDS_SAVED_SETTINGS[band].FM_SQL_threshold;
	TRX.ADC_PGA = TRX.BANDS_SAVED_SETTINGS[band].ADC_PGA;
	CurrentVFO()->DNR = TRX.BANDS_SAVED_SETTINGS[band].DNR;
	CurrentVFO()->AGC = TRX.BANDS_SAVED_SETTINGS[band].AGC;
	TRX_Temporary_Stop_BandMap = false;

	LCD_UpdateQuery.TopButtons = true;
	LCD_UpdateQuery.FreqInfo = true;
	TRX_AutoGain_Stage = 0;
}

static void FRONTPANEL_BUTTONHANDLER_BAND_N(void)
{
	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	band--;
	if (band < 0)
		band = BANDS_COUNT - 1;
	while (!BANDS[band].selectable)
	{
		band--;
		if (band < 0)
			band = BANDS_COUNT - 1;
	}

	TRX_setFrequency(TRX.BANDS_SAVED_SETTINGS[band].Freq, CurrentVFO());
	TRX_setMode(TRX.BANDS_SAVED_SETTINGS[band].Mode, CurrentVFO());
	TRX.LNA = TRX.BANDS_SAVED_SETTINGS[band].LNA;
	TRX.ATT = TRX.BANDS_SAVED_SETTINGS[band].ATT;
	TRX.ATT_DB = TRX.BANDS_SAVED_SETTINGS[band].ATT_DB;
	TRX.ADC_Driver = TRX.BANDS_SAVED_SETTINGS[band].ADC_Driver;
	TRX.FM_SQL_threshold = TRX.BANDS_SAVED_SETTINGS[band].FM_SQL_threshold;
	TRX.ADC_PGA = TRX.BANDS_SAVED_SETTINGS[band].ADC_PGA;
	CurrentVFO()->DNR = TRX.BANDS_SAVED_SETTINGS[band].DNR;
	CurrentVFO()->AGC = TRX.BANDS_SAVED_SETTINGS[band].AGC;
	TRX_Temporary_Stop_BandMap = false;

	LCD_UpdateQuery.TopButtons = true;
	LCD_UpdateQuery.FreqInfo = true;
	TRX_AutoGain_Stage = 0;
}

static void FRONTPANEL_BUTTONHANDLER_RF_POWER(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		SYSMENU_TRX_RFPOWER_HOTKEY();
		drawSystemMenu(true);
	}
	else
	{
		eventCloseAllSystemMenu();
	}
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_AGC(void)
{
	CurrentVFO()->AGC = !CurrentVFO()->AGC;
	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
		TRX.BANDS_SAVED_SETTINGS[band].AGC = CurrentVFO()->AGC;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_AGC_SPEED(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		SYSMENU_AUDIO_AGC_HOTKEY();
		drawSystemMenu(true);
	}
	else
	{
		eventCloseAllSystemMenu();
	}
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_SQUELCH(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		SYSMENU_AUDIO_SQUELCH_HOTKEY();
		drawSystemMenu(true);
	}
	else
	{
		eventCloseAllSystemMenu();
	}
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_WPM(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		SYSMENU_CW_WPM_HOTKEY();
		drawSystemMenu(true);
	}
	else
	{
		eventCloseAllSystemMenu();
	}
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_KEYER(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		SYSMENU_CW_KEYER_HOTKEY();
		drawSystemMenu(true);
	}
	else
	{
		eventCloseAllSystemMenu();
	}
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_STEP(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		SYSMENU_TRX_STEP_HOTKEY();
		drawSystemMenu(true);
	}
	else
	{
		eventCloseAllSystemMenu();
	}
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_DNR(void)
{
	TRX_TemporaryMute();
	CurrentVFO()->DNR = !CurrentVFO()->DNR;
	int8_t band = getBandFromFreq(CurrentVFO()->Freq, true);
	if (band > 0)
		TRX.BANDS_SAVED_SETTINGS[band].DNR = CurrentVFO()->DNR;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_NB(void)
{
	TRX.NOISE_BLANKER = !TRX.NOISE_BLANKER;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_BW(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		if (CurrentVFO()->Mode == TRX_MODE_CW_L || CurrentVFO()->Mode == TRX_MODE_CW_U)
			SYSMENU_AUDIO_BW_CW_HOTKEY();
		else if (CurrentVFO()->Mode == TRX_MODE_NFM || CurrentVFO()->Mode == TRX_MODE_WFM)
			SYSMENU_AUDIO_BW_FM_HOTKEY();
		else if (CurrentVFO()->Mode == TRX_MODE_AM)
			SYSMENU_AUDIO_BW_AM_HOTKEY();
		else
			SYSMENU_AUDIO_BW_SSB_HOTKEY();
		drawSystemMenu(true);
	}
	else
	{
		eventCloseAllSystemMenu();
	}
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_HPF(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		if (CurrentVFO()->Mode == TRX_MODE_CW_L || CurrentVFO()->Mode == TRX_MODE_CW_U)
			SYSMENU_AUDIO_HPF_CW_HOTKEY();
		else
			SYSMENU_AUDIO_HPF_SSB_HOTKEY();
		drawSystemMenu(true);
	}
	else
	{
		eventCloseAllSystemMenu();
	}
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_ArB(void) //A=B
{
	if (TRX.current_vfo)
		memcpy(&TRX.VFO_A, &TRX.VFO_B, sizeof TRX.VFO_B);
	else
		memcpy(&TRX.VFO_B, &TRX.VFO_A, sizeof TRX.VFO_B);
	
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_NOTCH(void)
{
	if (CurrentVFO()->NotchFC > CurrentVFO()->LPF_Filter_Width)
		CurrentVFO()->NotchFC = CurrentVFO()->LPF_Filter_Width;
	CurrentVFO()->ManualNotchFilter = false;
	if (!CurrentVFO()->AutoNotchFilter)
		CurrentVFO()->AutoNotchFilter = true;
	else
		CurrentVFO()->AutoNotchFilter = false;

	NeedReinitNotch = true;
	LCD_UpdateQuery.StatusInfoGUI = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_NOTCH_MANUAL(void)
{
	if (CurrentVFO()->NotchFC > CurrentVFO()->LPF_Filter_Width)
		CurrentVFO()->NotchFC = CurrentVFO()->LPF_Filter_Width;
	CurrentVFO()->AutoNotchFilter = false;
	if (!CurrentVFO()->ManualNotchFilter)
		CurrentVFO()->ManualNotchFilter = true;
	else
		CurrentVFO()->ManualNotchFilter = false;

	NeedReinitNotch = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_SHIFT(void)
{
	TRX.ShiftEnabled = !TRX.ShiftEnabled;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_CLAR(void)
{
	TRX.CLAR = !TRX.CLAR;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_LOCK(void)
{
	if (!LCD_systemMenuOpened)
		TRX.Locked = !TRX.Locked;
	else
	{
		sysmenu_hiddenmenu_enabled = true;
		LCD_redraw();
	}
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_MENU(void)
{
	if (!LCD_systemMenuOpened)
		LCD_systemMenuOpened = true;
	else
		eventCloseSystemMenu();
	LCD_redraw();
}

static void FRONTPANEL_BUTTONHANDLER_MUTE(void)
{
	TRX_Mute = !TRX_Mute;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static void FRONTPANEL_BUTTONHANDLER_BANDMAP(void)
{
	TRX.BandMapEnabled = !TRX.BandMapEnabled;
	LCD_UpdateQuery.TopButtons = true;
	NeedSaveSettings = true;
}

static uint16_t FRONTPANEL_ReadMCP3008_Value(uint8_t channel, GPIO_TypeDef *CS_PORT, uint16_t CS_PIN)
{
	uint8_t outData[3] = {0};
	uint8_t inData[3] = {0};
	uint16_t mcp3008_value = 0;

	outData[0] = 0x18 | channel;
	bool res = SPI_Transmit(outData, inData, 3, CS_PORT, CS_PIN, false);
	if (res == false)
		return 65535;
	mcp3008_value = (uint16_t)(0 | ((inData[1] & 0x3F) << 4) | (inData[2] & 0xF0 >> 4));

	return mcp3008_value;
}

static void FRONTPANEL_BUTTONHANDLER_SERVICES(void)
{
	if (!LCD_systemMenuOpened)
	{
		LCD_systemMenuOpened = true;
		SYSMENU_HANDL_SERVICESMENU(1);
		drawSystemMenu(true);
	}
	else
	{
		eventCloseSystemMenu();
		//eventCloseAllSystemMenu();
	}
	LCD_redraw();
}
