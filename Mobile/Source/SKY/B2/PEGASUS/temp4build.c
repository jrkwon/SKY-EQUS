#define FAKE_CHECKSUM   0xFFFF

const char mob_sw_rev[] = "S0119001";

unsigned short app_image_checksum = FAKE_CHECKSUM;

unsigned short cal_chksum(void)
{
    return FAKE_CHECKSUM;
}

SKA_MainSKVMBG()
{
    return;
}

int g_bPlaySkvmInBG = 0;


//#include "ska.h"
unsigned char uinuasms_SMS_call_active;
int bAudioEnByYamaha;
int xvm_state;
int smbKeyTouchFlag;
int g_MyHolidayT[10];
char	ui_bIrDAExchangeMode;
int	 g_iSKVMInputDataLen;
char g_pszSKVMInputData[1] = {0};
char g_pszmsdURL[1] = {0};
/* uiscall.c */
int r_SavSmsSize;
unsigned short *MSM_image_H;

int g_fWAP_RET_CALL;

SKY_IsInSMSCall()
{
    return 0;
}


int SKVMMain() { return -1; }


/* uiusmsl.c */
//ui_sms_store_new_msg() {}
char *wwSetHashedPIN(char *pin_in) 
{
    static char szDummy[20];

    return szDummy;
}

rex_int_free() {}
rex_int_lock() {}
dsi_put_nv_item() {}

boot_clear_sl() {}

uasms_ack_message() {}
reset_ones_detect() {}

//mmSmsAlarm() {}   //00.12.26
rf_codec_on() {}
ERR() {}
b_unpackb() {}
//mnSmsAlarm() {}

voc_reset_ones_detect() {}
vmux_acquire(){}

snd_set_volume(){}
nd_set_mic_gain() {}
snd_set_device() {}
snd_music_id_start() {}
snd_sound_id_start() {}
snd_set_mic_gain() {}
snd_sample_music_id_start() {}
snd_sample_sound_id_start() {}
snd_sound_start(){}
snd_music_stop() {}
snd_sound_stop() {}
snd_tone_start() {}
snd_stereo_mute_ctl() {}
snd_music_start() {}
snd_sample_music_start() {}
snd_filters() {}
SKY_CurrentCallIsSMS() {}
err_fatal_lcd_mesg() {}

snd_voice_start() {}
snd_voice_stop() {}
snd_sample_tone_start() {}
snd_tone_stop() {}
uinuasms_init() {}
//g_NewMybellSave() {}
Convert_RingerToNote() {}
Make_MyMelody_Header() {}
flfread() {}
mc_cmd_free_q() {}
mc_cmd() {}
vc_tx_offset() {}
vc_rx_offset() {}
vocm2_select_offsets() {}

//flfwrite() {}
voc_fn_ptr() {}
ReInitialidLCD() {}
Ma2w_led_control() {}
Ma2w_motor_control() {}
Ma2w_eq_vol() {}
//rf_codec_on() {}
adc_read() {}
#if SKY_MODEL_ID != SKY_MODEL_B1 && (SKY_MODEL_ID != SKY_MODEL_B2)
EQA_CameraTestMenuTop() {}
#endif
flfwrite() {}
Packet_play() {}
outsb_rep() {}
StopCamera() {}
CameraCapturedRGB() {}
InitCamera() {}


getfreeindxnum() {}
voc_config() {}
flfsize() {}
voc_release() {}
flfopen() {}
getfn() {}
flferase() {}
flfclose() {}
fs_alloc_gc_eval() {}
erase_unused_Fn() {}
voc_acquire() {}
//subcmd_play_rec() {}
voice_erase_all_when_nv_setup() {}
mccreg_clear_reg() {}


nvimio_io()   {}
nvimw_write() {}
MSG_ERROR()   {}


PtpLinkConnect() {}
PtpLinkRelease() {}
EnterPtpLinkMode() {}






dog_report() {}

nv_cmd() {}

mc_task_start() {}

snd_cmd() {}



clk_reg() {}
clk_dereg() {}
clk_def() {}
clk_read_ms() {}
clk_busy_wait() {}

cm_ph_cmd_rssi_delta_threshold() {}
cm_client_act() {}
cm_nc_reg() {}
cm_client_inband_reg() {}
cm_client_ss_reg() {}
cm_client_ph_reg() {}
cm_client_call_reg() {}
cm_client_init() {}

cm_call_cmd_orig() {}
cm_call_cmd_answer() {}
cm_call_cmd_end() {}
uasms_DTC_disconnect() {}
cm_call_cmd_flash() {}
cm_ph_cmd_oprt_mode() {}
cm_ph_cmd_sys_pref() {}
cm_ph_cmd_mode_pref() {}
cm_ph_cmd_nam_sel() {}
cm_ph_cmd_answer_voice() {}
cm_inband_cmd_burst_dtmf() {}
cm_inband_cmd_start_cont_dtmf() {}
cm_inband_cmd_stop_cont_dtmf() {}
cm_ph_cmd_get_ph_info() {}
cm_call_cmd_get_call_info() {}
cm_ss_cmd_get_ss_info() {}

err_put_log() {}

rex_set_cpsr() {}
err_fatal_put_log() {}



rex_sched() {}
rex_start_task() {}




tramp_set_isr() {}
/*
SOUND_COMMAND_U Command;
SOUND_COMMAND_U *snd_get_packet() 
{
    return &Command;
}
*/
snd_get_packet() { return 0; }
ui_cli_clear() {}
//ui_setup_cli_cnap() {}

cm_ext_disp_itr_get() {}
cm_ext_disp_itr_more() {}
cm_ext_disp_itr_next() {}
cm_ext_disp_itr_reset() {}

void idLCDDisplayOn ( void ) {}
void idLCDDisplayOff ( void ) {}

void EQS_hs_update_IDLCD_screen(void) {}
void hs_idlcd_contrast( word ) {}

void InitialidLCD ( void ) {}

void SKY_InitDisplay() {}

//void SKA_SetVoiceMode() {}
//void SKY_InitSubLCD() {}
//void SKY_UpdateSubScreen() {}
//void SKY_SubLCDOff() {}
//void SKY_SubLCDOn() {}
void rxc_get_mode() {}
void voc_rx_cmd()   {}
unsigned short srch_get_tx_adj() {}
unsigned short srch_get_rx_agc() {}

smb_talk_state_time() {}
smb_is_operating() {}
smb_percent_capacity() {}
smb_idle_state_time() {}
smb_init_at_extpwr() {}
smbIsOperation() {}
SmbWrite1Byte() {}
SmbRead1Byte() {}
SmbEvent() {}
smb_check_RAM_validity() {}
cdma() {}

snd_music_buf_start() {}
snd_sample_music_buf_start() {}
snd_is_connection_tone_played() {}

nv_memavail() {}


SKA_BatteryInit() {}
smb_init() {}
SKY_MMInvsetup() {}

InitMyHoliday() {}
//SKA_MainMyHoliday() {}

ChangeIrDAMode() {}
IrDA_DrawIrDAStatusLine() {}
short IrDA_AutoOffTimeOutCounter;
IrDA_GetOneByte() {}
IrDA_Check_Rx_Buffer() {}
MDM_SearchIrDADevice() {}
IrCOMM_SendData() {}
MDM_Slsap() {}
MDM_GetSetting() {}
MDM_CheckLAPConnection() {}

subcmd_play_rec() {}
//rf_codec_on() {}

crc_30_calc() {}
crc_16_calc() {}

void dog_kick(void) {}

//BuildMyHoliday() {}

dsi_get_nv_item() {}
uis_InitSavSmsList() {}
uis_FindFreeSavSmsBuffer() {}
ui_free_nv() {}
SearchGroupIndex() {}
EQA_CameraTestMenuTop() {}
//SKA_MainIrDAPhoto() {}
//rf_codec_on() {}

SKA_MainWAP() {}
SKA_SMSSendMsg() {}

//read_superblock() {}
at_send_extended_text() {}
SKA_MainDuDeoGi() {}
SKA_FromWAP(){}
SKA_MainApache() {}
SKA_MainRally() {}
SKA_MainPolar() {}
SKA_Mars() {}
SKA_MainSamtris() {}
SKA_MainPuzzle() {}
SKA_MainSKVM() {}
SvrLinkConnect() {}
SvrLinkReconnect() {}
SvrLinkReleasebyUi() {}
SvrLinkSend() {}
SvrLinkRcv() {}

hw_reset() {}
ds_cmd()   {}
dsi_requested_sig_mask() {}

uinuasms_convert_to_ti() {}

midd_PlayVocoder() {}
SKA_MainWAPTimeInfo(){}
SKA_SendWAPMsg() {}
errorType() {}
errorNumber(){}

SKA_WAPEXIT() {}
SKA_WAPLOADING() {}
SKA_WAPHANDLE_KEY() {}

smbIsBrokenDown() {}
smb_batt_curve_estimate() {}
uiuasms_SMS_call_active() {}
SKA_WAPFLAG_TEST2() {}
SKA_WAPFLAG_TEST1() {}
SKA_MainEif() {}
g_nKeyFlag() {}
EIF_Event() {}
g_nNewMsgTry() {}
g_fSMSNewMsg() {}
SKA_MainSMBTest() {}
//SKY_InitMOSave() {}
//SKA_MainIrDAPhoto() {}

//g_TypeConnectSvr() {}
//SKA_MainGVM() {}
//SKA_TCPConnect() {}
FormatWapMailMesasge() {}
SKA_MainRFTest() {}
SKA_TuneChannel() {}
SKA_AdjustPower() {}
SKA_RF_TxOff() {}
SKA_RF_TxOn() {}
g_pszPushUrl() {}

CLNTa_log() {}
SKA_ChooseMenu() {}

//CheckCurrentTime() {}

//SKA_VMViewMsg() {}

SKA_LocationSendMsg() {}
SKA_LocationView() {}
CheckCurrentTime() {}
SetUIPktRelease() {}
msg_file() {}
SKA_LocationNotif() {}

SKY_IS_FROM_SKVM() {}
midd_VoiceCall() {}
PtpLinkRcv() {}
uasms_DTC_connect() {}
mb_MusicBellPreBuffer() {}
SKA_MainMWallet() {}

int g_pPMList;
int g_unNumOfPM;
photomail_main() {} 
dsDormantEnabled() {}

int g_fGVMIncomingCall;
//SKA_RunStandAloneGame() {}
WritePhotoMail() {}
MM_TCM2Encode() {}
SeekWorkingArea() {}

ds_socfg_get_p_rev_index() {}
SKY_IS_IN_WAP(){}
MUSIC_hw_power_dowm() {}
SW_Reset() {}
SKY_SET_FROM_GVM() {}
SKY_IS_FROM_GVM() {}
Game() {}
//SKA_GVMScriptList(){}

INTFREE_SAV() {}
INTLOCK_SAV() {}

IsEIF_AutoReceive(){}
fIsGame(){}
//IsSpace2RecordAutoAnswer() {}


midd_BackToBrowser(){}
midd_PtpRelease(){}
midd_PtpWait(){}
midd_PtpCall(){}
midd_Connect(){}
SendSMSMsg(){}
midd_Disconnect(){}
scl_DoSwapEvent(){}
midd_GetScriptInfo(){}
scl_ModifyScriptInfo(){}
scl_CheckValidate(){}
midd_DeleteScript(){}
scl_StoreRunInfo(){}
mc_get_p_rev_in_use(){}
midd_SetSoMo(){}
midd_StopTimer(){}

ERR_FATAL() {}
mcsyspr_scan_list_count() {}

SKA_RunGVM() {}
int g_fGVMPaused;
SKA_MainGVM() {}
GoToGVM() {}

int g_fDownloadRequestFromSMS;

vbatt_temp_read() {}
send_ui_cmd() {}
SKY_IS_IN_WAVELET() {}
SKA_RecoverPhotoList(){}