#-------------------------------------------------------------------------------
# SOURCE / MEMORY
#-------------------------------------------------------------------------------

SRC_DIR = $(EQUS_UTIL_MEM_DIR)
EQUS_UTIL_MEM_OBJS = $(TARGET)\au_mem.o
                    
$(EQUS_UTIL_MEM_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# EQUS / State
#-------------------------------------------------------------------------------

SRC_DIR = $(EQUS_STATE_DIR)

EQUS_STATE_OBJS =  $(TARGET)\eqs_aapi.o \
                   $(TARGET)\eqs_swin.o

$(EQUS_STATE_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_EQUS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# EQUS / UTIL
#-------------------------------------------------------------------------------

SRC_DIR = $(EQUS_UTIL_DIR)

EQUS_UTIL_OBJS =  $(TARGET)\eqs_list.o \
                  $(TARGET)\eqs_timr.o \
                  $(TARGET)\eqs_mem.o \
                  $(TARGET)\eqs_util.o 

$(EQUS_UTIL_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_EQUS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# EQUS / Display
#-------------------------------------------------------------------------------

SRC_DIR = $(EQUS_DISPLAY_DIR)

EQUS_DISPLAY_OBJS = $(TARGET)\eqs_grph.o \
                    $(TARGET)\eqs_dc.o \
                    $(TARGET)\eqs_gui.o \
                    $(TARGET)\eqs_frmt.o 

$(EQUS_DISPLAY_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_EQUS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# EQUS / Control
#-------------------------------------------------------------------------------

SRC_DIR = $(EQUS_CTRL_DIR)

EQUS_CTRL_OBJS =    $(TARGET)\eqc.o \
                    $(TARGET)\eqc_stxt.o \
                    $(TARGET)\eqc_scrl.o \
                    $(TARGET)\eqc_nedt.o \
                    $(TARGET)\eqc_tedt.o \
                    $(TARGET)\eqc_msgb.o \
                    $(TARGET)\eqc_pmnu.o \
                    $(TARGET)\eqc_anim.o \
                    $(TARGET)\eqc_aicn.o \
                    $(TARGET)\eqc_img.o \
                    $(TARGET)\eqc_spin.o  \
                    $(TARGET)\eqc_rbtn.o \
                    $(TARGET)\eqc_ckbx.o \
                    $(TARGET)\eqc_pbar.o \
                    $(TARGET)\eqc_pbtn.o \
                    $(TARGET)\eqc_menu.o \
					$(TARGET)\eqc_time.o


$(EQUS_CTRL_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_EQUS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# EQUS / I18N 
#-------------------------------------------------------------------------------
SRC_DIR = $(EQUS_I18N_DIR)

EQUS_I18N_OBJS = $(TARGET)\eqs_i18n.o \
                 $(TARGET)\eqs_tokn.o

$(EQUS_I18N_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_EQUS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# EQUS / PLATFORM
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# EQUS / RESOURCE 
#-------------------------------------------------------------------------------

SRC_DIR = $(EQUS_RES_IMG_DIR)

EQUS_RES_IMG_OBJS = $(TARGET)\eqs_data.o                     

$(EQUS_RES_IMG_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------



#-------------------------------------------------------------------------------
# SKY / DEVICE / LCD
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_DEVICE_LCD_DIR)

SKY_DEVICE_LCD_OBJS = $(TARGET)\sky_lcd.o \
                      $(TARGET)\sky_slcd.o

$(SKY_DEVICE_LCD_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS0) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / I18N 
#-------------------------------------------------------------------------------
SRC_DIR = $(SKY_I18N_DIR)

SKY_I18N_OBJS = $(TARGET)\sky_tokn.o

$(SKY_I18N_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / PLATFORM / REX
#-------------------------------------------------------------------------------
SRC_DIR = $(SKY_PLT_REX_DIR)

SKY_PLT_REX_OBJS = $(TARGET)\skyrefs.o \
                   $(TARGET)\skyrrex.o  

$(SKY_PLT_REX_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------



#-------------------------------------------------------------------------------
# SKY / RESOURCE 
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_RES_IMG_DIR)

SKY_RES_IMG_OBJS = $(TARGET)\sky_data.o                     

$(SKY_RES_IMG_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / RESOURCE / MENU
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_RES_MENU_DIR)

SKY_RES_MENU_OBJS = $(TARGET)\sky_mtxt.o                     

$(SKY_RES_MENU_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / SYSTEM
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_SYS_DIR)

SKY_SYS_OBJS = $(TARGET)\sky_mem.o \
               $(TARGET)\sky_cm.o \
               $(TARGET)\sky_cml.o \
               $(TARGET)\sky_conf.o \
               $(TARGET)\sky_evnt.o \
               $(TARGET)\sky_file.o \
               $(TARGET)\sky_hcbt.o \
               $(TARGET)\sky_hcmd.o \
               $(TARGET)\sky_hkey.o \
               $(TARGET)\sky_hs.o \
               $(TARGET)\sky_hsig.o \
!IFDEF IRDA
               $(TARGET)\sky_irda.o \
!ENDIF              
               $(TARGET)\sky_nv.o \
               $(TARGET)\sky_scrn.o \
               $(TARGET)\sky_sms.o \
               $(TARGET)\sky_snd.o \
               $(TARGET)\sky_annu.o \
               $(TARGET)\sky_ui.o \
!IFDEF VAS
               $(TARGET)\sky_sock.o \
!ENDIF
               $(TARGET)\uixcm.o


$(SKY_SYS_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# SKY / TEMP - only 4 build 
#-------------------------------------------------------------------------------

#SRC_DIR = $(SKY_APP_TEST_DIR)
#
#SKY_TEMP_OBJS = $(TARGET)\sky_temp.o
#
#$(SKY_TEMP_OBJS) : $(SRC_FILE)
#        @echo ------------------------------------------------------------------
#        @echo OBJECT $(@F)
#        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / SYSTEM
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_SYS_DIR)

SKY_APP_SYS_OBJS =  $(TARGET)\ska_off.o \
                    $(TARGET)\ska_strt.o \
                    $(TARGET)\ska_idle.o \
                    $(TARGET)\ska_call.o \
                    $(TARGET)\ska_clid.o \
                    $(TARGET)\ska_code.o \
                    $(TARGET)\ska_nam.o

$(SKY_APP_SYS_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKT Teletech Data Service Module. author : khekim,htg,jylee
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------

SRC_DIR = Source\DS
DSSKT_OBJS = $(TARGET)\dsskt.o \
             $(TARGET)\dsatskt.o
$(DSSKT_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_DS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
#-------------------------------------------------------------------------------

SRC_DIR = Source\SOCKAPI
SOCKAPI_OBJS = $(TARGET)\uisockApi.o \
               $(TARGET)\sockctl.o \
			   $(TARGET)\sockApi.o 
$(SOCKAPI_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_DS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE) 
        @echo ------------------------------------------------------------------
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / EIF
#-------------------------------------------------------------------------------




SRC_DIR = $(SKY_APP_EIF_DIR)

SKY_APP_EIF_OBJS = $(TARGET)\ska_eif.o 


$(SKY_APP_EIF_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# SKY / APPLET / PBOOK
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_PBOOK_DIR)

SKY_APP_PBOOK_OBJS = $(TARGET)\ska_bstr.o \
                     $(TARGET)\ska_bapi.o \
                     $(TARGET)\ska_butl.o \
                     $(TARGET)\ska_pbo.o  			

$(SKY_APP_PBOOK_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / BELL_VIB
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_BELL_VIB_DIR)

SKY_APP_BELL_VIB_OBJS = $(TARGET)\ska_bell.o \
                        $(TARGET)\ska_mbel.o \
                        $(TARGET)\ska_warn.o 
                                                   

$(SKY_APP_BELL_VIB_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS0) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

!IFDEF CHARACTER_BELL
#-------------------------------------------------------------------------------
# SKY / APPLET / CHARACBELL
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_CHARACBELL_DIR)

SKY_APP_CHARACBELL_OBJS = $(TARGET)\ska_cbel.o 

$(SKY_APP_CHARACBELL_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS0) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
!ENDIF

#-------------------------------------------------------------------------------
# SKY / APPLET / CALC
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_CALC_DIR)

SKY_APP_CALC_OBJS = $(TARGET)\ska_calc.o \
                                                   

$(SKY_APP_CALC_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / SWATCH
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_SWATCH_DIR)

SKY_APP_SWATCH_OBJS = $(TARGET)\ska_stop.o \
                                                   

$(SKY_APP_SWATCH_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / WORLDTIME
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_WTIME_DIR)

SKY_APP_WTIME_OBJS = $(TARGET)\ska_wtim.o \
                                                   

$(SKY_APP_WTIME_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------



#-------------------------------------------------------------------------------
# SKY / APPLET / RECPLAY
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_RECPLAY_DIR)

SKY_APP_RECPLAY_OBJS = $(TARGET)\ska_vmemo.o

$(SKY_APP_RECPLAY_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# SKY / APPLET / VOCODER
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_VOCODER_DIR)

SKY_APP_VOCODER_OBJS = $(TARGET)\ska_vcmm.o

$(SKY_APP_VOCODER_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / AUTO ANSWER
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_AUTOANS_DIR)

SKY_APP_AUTOANS_OBJS = $(TARGET)\ska_aans.o \
                       $(TARGET)\ska_ment.o\
                       $(TARGET)\ska_aapi.o\
                       $(TARGET)\ska_crec.o\
                       $(TARGET)\ska_hdon.o

$(SKY_APP_AUTOANS_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------




#-------------------------------------------------------------------------------
# SKY / APPLET / Memo
#-------------------------------------------------------------------------------

#SRC_DIR = $(SKY_APP_MEMO_DIR)

#SKY_APP_MEMO_OBJS = $(TARGET)\ska_memo.o 

#$(SKY_APP_MEMO_OBJS) : $(SRC_FILE)
#        @echo ------------------------------------------------------------------
#        @echo OBJECT $(@F)
#        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / Schedule
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_SCHEDULE_DIR)

SKY_APP_SCHEDULE_OBJS = $(TARGET)\ska_schl.o \
						$(TARGET)\ska_sfil.o \
						$(TARGET)\ska_sbel.o \
						$(TARGET)\ska_scal.o \
						$(TARGET)\ska_slst.o \
						$(TARGET)\ska_shld.o

$(SKY_APP_SCHEDULE_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / Alarm
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_ALARM_DIR)

SKY_APP_ALARM_OBJS = $(TARGET)\ska_alrm.o 

$(SKY_APP_ALARM_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / Memo
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_MEMO_DIR)

SKY_APP_MEMO_OBJS = $(TARGET)\ska_memo.o 

$(SKY_APP_MEMO_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

!IFDEF MCOMMERCE
#-------------------------------------------------------------------------------
# SKY / APPLET / Mobile Wallet
#-------------------------------------------------------------------------------
MW_DIR = SOURCE\MCommerce
CFLAGS_MW = -I $(MW_DIR)

SRC_DIR = $(SKY_APP_MWALLET_DIR)

SKY_APP_MWALLET_OBJS = $(TARGET)\ska_mwlt.o 

$(SKY_APP_MWALLET_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(CFLAGS_MW) -ec $(DMSS_CFLAGS_SKY) $(DMSS_CFLAGS_MW) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
!ENDIF

!IFDEF PHOTOMAIL
#-------------------------------------------------------------------------------
# SKY / APPLET / PM
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_PM_DIR)

SKY_APP_PM_OBJS = $(TARGET)\ska_vm.o \

$(SKY_APP_PM_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#SKY_APP_PM_ENCODER_OBJS = $(SKY_APP_PM_DIR)\TCM2Encode.o

!ENDIF

!IFDEF VAS
#-------------------------------------------------------------------------------
# SKY / APPLET / VAS
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_VAS_DIR)

SKY_APP_VAS_OBJS = $(TARGET)\ska_vas.o 

$(SKY_APP_VAS_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

!ENDIF

#-------------------------------------------------------------------------------
# SKY / APPLET / SETTING
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_SETTING_DIR)

SKY_APP_SETTING_OBJS = $(TARGET)\ska_011.o \
                       $(TARGET)\ska_017.o \
                       $(TARGET)\ska_2get.o \
                       $(TARGET)\ska_info.o \
                       $(TARGET)\ska_init.o \
                       $(TARGET)\ska_lock.o \
                       $(TARGET)\ska_NAMs.o \
                       $(TARGET)\ska_ntop.o \
                       $(TARGET)\ska_tnum.o \
                       $(TARGET)\ska_ligt.o \
					   $(TARGET)\ska_volm.o \
                       $(TARGET)\ska_add.o \
                       $(TARGET)\ska_ver.o \
                       $(TARGET)\ska_thme.o

$(SKY_APP_SETTING_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / DATACFG
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_DATACFG_DIR)

SKY_APP_DATACFG_OBJS = $(TARGET)\ska_dscf.o 

$(SKY_APP_DATACFG_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / DATAMGR
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_DATAMGR_DIR)

SKY_APP_DATAMGR_OBJS = $(TARGET)\ska_dmgr.o \
                       $(TARGET)\ska_dsvc.o 

$(SKY_APP_DATAMGR_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# SKY / APPLET / CALL LIST
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_CALLLIST_DIR)

SKY_APP_CALLLIST_OBJS = $(TARGET)\ska_clst.o


$(SKY_APP_CALLLIST_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


!IF "$(MODEL_ID)" == "MODEL_B2"
#-------------------------------------------------------------------------------
# SKY / APPLET / CAMERA
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_CAMERA_DIR)

SKY_APP_CAMERA_OBJS = $(TARGET)\ska_cam.o 

$(SKY_APP_CAMERA_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
!ENDIF

!IF "$(MODEL_ID)" == "MODEL_B2"
#-------------------------------------------------------------------------------
# Select WAVELET compiling  #yoursun 01/06/08
#-------------------------------------------------------------------------------

!IFDEF FEATURE_WAVELET

SOCKAPI = FEATURE_SKT_SOCKAPI
SKY_APP_WAVELET_DIR         = $(SKY_APP_DIR)\WAVELET

SKY_APP_WAVDECODE_OBJS		= $(SKY_APP_WAVELET_DIR)\LIB\tcm2-skteletech-arm250-1.0.2.5.lib 

#-------------------------------------------------------------------------------
# SKY / APPLET / WAVELET  #yoursun 01/06/08
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_WAVELET_DIR)

SKY_APP_WAVELET_OBJS = $(TARGET)\ska_wav.o \
					   $(TARGET)\wav_multisock.o 

$(SKY_APP_WAVELET_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -ec $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
!ENDIF #### !IFDEF FEATURE_WAVELET

!ENDIF #### !IF "$(MODEL_ID)" == "MODEL_B2"

#-------------------------------------------------------------------------------
# SKY / APPLET / Menu
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_MENU_DIR)

SKY_APP_MENU_OBJS = $(TARGET)\ska_menu.o  

$(SKY_APP_MENU_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / Control Panel
#-------------------------------------------------------------------------------

#SRC_DIR = $(SKY_APP_CTRL_DIR)

#SKY_APP_CTRL_OBJS = $(TARGET)\ska_ctrl.o  

#$(SKY_APP_CTRL_OBJS) : $(SRC_FILE)
#        @echo ------------------------------------------------------------------
#        @echo OBJECT $(@F)
#        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# SKY / APPLET / SMS                   by reo 00-07-28
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_SMS_DIR)

SKY_APP_SMS_OBJS = $(TARGET)\ska_sms.o \
		           $(TARGET)\ska_smsl.o \
		           $(TARGET)\ska_mybe.o \
                   $(TARGET)\ska_vms.o \
                   $(TARGET)\ska_mo.o  \
				   $(TARGET)\ska_loca.o



$(SKY_APP_SMS_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / PICTURE                   by woodstok 00-12-30
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_PIC_DIR)

SKY_APP_PIC_OBJS = $(TARGET)\ska_pic.o \
                   $(TARGET)\ska_mani.o

$(SKY_APP_PIC_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


!IFDEF IRDA
#-------------------------------------------------------------------------------
# SKY / APPLET / IRDA                   by woodstok 01-01-04
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_IRDA_DIR)

SKY_APP_IRDA_OBJS = $(TARGET)\ska_irpc.o \
                    $(TARGET)\ska_irph.o \
                    $(TARGET)\ska_irda.o \
                    $(TARGET)\ska_ftrs.o \
                    $(TARGET)\ska_iani.o \
!IF "$(MODEL_ID)" == "MODEL_B2"
                    $(TARGET)\ska_ipht.o \
!ENDIF
                    $(TARGET)\ska_imel.o \
                    $(TARGET)\ska_ibok.o 

$(SKY_APP_IRDA_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

!ENDIF
           
#-------------------------------------------------------------------------------
# SKY / APPLET / GAME
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_GAME_LIB_DIR)

SKY_APP_GAME_LIB_OBJS = $(TARGET)\GExt.o \
                        $(TARGET)\GLibMain.o \
                        $(TARGET)\GLibScore.o 

$(SKY_APP_GAME_LIB_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


SRC_DIR = $(SKY_APP_GAME_DIR)

SKY_APP_GAME_OBJS = $(TARGET)\Apache5.o \
                        $(TARGET)\MarsScouter3.o \
                        $(TARGET)\Puzzle3.o \
                        $(TARGET)\Raccoon3.o \
                        $(TARGET)\samtris3.o 

$(SKY_APP_GAME_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
 
                 
SRC_DIR = $(SKY_APP_GAMEAPP_DIR)

SKY_APP_GAMEAPP_OBJS = $(TARGET)\ska_game.o 
                       

$(SKY_APP_GAMEAPP_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SKY / APPLET / EMail                   by woodstok 01-02-08
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_EMAIL_DIR)

SKY_APP_EMAIL_OBJS = $(TARGET)\ska_mail.o \
                     $(TARGET)\ska_mbox.o \
                     $(TARGET)\ska_smtp.o \
                     $(TARGET)\ska_pop3.o

$(SKY_APP_EMAIL_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# SKY / APPLET / TEST 
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_TEST_DIR)

SKY_APP_TEST_OBJS = $(TARGET)\MelodyTest.o \
#                    $(TARGET)\ska_efs.o \
#                    $(TARGET)\ctrltest.o \
                    $(TARGET)\LcdTest.o \
                    $(TARGET)\RFTest.o \
!IF "$(MODEL_ID)" == "MODEL_B0"
                    $(TARGET)\ska_smb.o \
!endif
!IF "$(MODEL_ID)" == "MODEL_B2"
                    $(TARGET)\cameratest.o \
!ENDIF
                    $(TARGET)\ska_test.o
#                        $(TARGET)\testgrph.o \
#$(TARGET)\TestMenu.o \
#							 $(TARGET)\AudioTest.o \
# tasik 00.08.01			 $(TARGET)\BatteryTest.o \
#							 $(TARGET)\IrDATest.o \
#							 $(TARGET)\McTest.o \
#							 $(TARGET)\RssiTest.o \
#							 $(TARGET)\BlightTest.o \
#							 $(TARGET)\VolumeTest.o \
#							 $(TARGET)\VolumeUpDTest.o \
#							 $(TARGET)\LedTest.o \
#	tasik 00.08.01			 $(TARGET)\LcdTest.o \
#                             $(TARGET)\LcdTest.o \
#							 $(TARGET)\RingTest.o 


$(SKY_APP_TEST_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

!IFDEF WAP2
#-------------------------------------------------------------------------------
# SKY / APPLET / WAP_APP
#-------------------------------------------------------------------------------

SRC_DIR = $(SKY_APP_WAP_DIR)

SKY_APP_WAP_OBJS = $(TARGET)\ska_WAPAPP.o  \
				   $(TARGET)\ska_wapdev.o
$(SKY_APP_WAP_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
		@echo OBJECT $(@F)
		$(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
		@echo ------------------------------------------------------------------
!ENDIF

!IFDEF GVM
#-------------------------------------------------------------------------------
# SKY / APPLET / GVM
#-------------------------------------------------------------------------------
SRC_DIR = $(SKY_APP_GVM_DIR)
GVM_OBJS = $(TARGET)\ska_gvm.o \
#           $(TARGET)\gvmShare.o
           
$(GVM_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(GVM_CFLAGS) $(DMSS_CFLAGS_GVM) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

SRC_DIR = $(SKY_APP_GVM_DIR)\SWAP
GVM_LIB_OBJS = $(TARGET)\Mil.o \
                $(TARGET)\SCL.o \
				$(TARGET)\SVM.o \
				$(TARGET)\MILNet.o \
				$(TARGET)\Midd.o
$(GVM_LIB_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(GVM_CFLAGS) $(DMSS_CFLAGS_GVM) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


SRC_DIR = $(SKY_APP_GVM_DIR)\AtSWAP
GVM_ATSWAP_OBJS = $(TARGET)\AtSWAP.o
$(GVM_ATSWAP_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(GVM_CFLAGS) $(DMSS_CFLAGS_GVM) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

!ENDIF

!IFDEF SKVM
#-------------------------------------------------------------------------------
# SKY / APPLET / SKVM            
#-------------------------------------------------------------------------------
SRC_DIR = $(SKY_APP_SKVM_DIR)
SKY_APP_SKVM_OBJS = $(TARGET)\ska_skvm.o \
				  $(TARGET)\graphics.o \
#				  $(TARGET)\mdarm.o \
				  $(TARGET)\mdarm_main.o \
				  $(TARGET)\mdarm_sys.o \
				  $(TARGET)\mdarm_skt.o \
				  $(TARGET)\mdarm_net.o \
				  $(TARGET)\mdarm_terminal.o \
				  $(TARGET)\romfile.o
$(SKY_APP_SKVM_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
		$(CC) $(SKVM_CFLAGS0) $(DMSS_CFLAGS_SKVM) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)

        @echo ------------------------------------------------------------------

SKY_APP_SKVM_LIB_OBJS = $(SKY_APP_SKVM_LIB_DIR)\externlib.o \
#               $(SKY_APP_SKVM_LIB_DIR)\mdsock.o \
				$(SKY_APP_SKVM_LIB_DIR)\romlib.o \
				$(SKY_APP_SKVM_LIB_DIR)\xvmlib.o \
				$(SKY_APP_SKVM_LIB_DIR)\nativelib.o 
!ENDIF

!IFDEF MCOMMERCE

SRC_DIR = SOURCE\MCommerce
SKY_INC = -I SOURCE\SKY\B2\APPLET\INCLUDE

MCOMMERCE_WPKI =$(TARGET)\asn1enc_new.o \
				$(TARGET)\asn1param_new.o \
				$(TARGET)\asn1pri_new.o \
				$(TARGET)\asn1pub_new.o \
				$(TARGET)\asn1sig_new.o \
				$(TARGET)\asn1_lib_new.o \
				$(TARGET)\a_bitstr_new.o \
				$(TARGET)\a_bytes_new.o \
				$(TARGET)\a_int_new.o \
				$(TARGET)\a_object_new.o \
				$(TARGET)\a_octet_new.o \
				$(TARGET)\base64_new.o \
				$(TARGET)\bn_add_new.o \
				$(TARGET)\bn_asm_new.o \
				$(TARGET)\bn_ctx_new.o \
				$(TARGET)\bn_div_new.o \
				$(TARGET)\bn_exp_new.o \
				$(TARGET)\bn_gcd_new.o \
				$(TARGET)\bn_lib_new.o \
				$(TARGET)\bn_mont_new.o \
				$(TARGET)\bn_mul_new.o \
				$(TARGET)\bn_prime_new.o \
				$(TARGET)\bn_print_new.o \
				$(TARGET)\bn_rand_new.o \
				$(TARGET)\bn_shift_new.o \
				$(TARGET)\bn_sqr_new.o \
				$(TARGET)\bn_word_new.o \
				$(TARGET)\buffer_new.o \
				$(TARGET)\cert_manage_new.o \
				$(TARGET)\cert_request_new.o \
				$(TARGET)\content_lib_new.o \
				$(TARGET)\create_Signed_Certificate_new.o \
				$(TARGET)\cryptlib_new.o \
				$(TARGET)\crypto_script_new.o \
				$(TARGET)\crypto_size_new.o \
				$(TARGET)\d2i_r_pr_new.o \
				$(TARGET)\d2i_r_pu_new.o \
				$(TARGET)\decrypttext_new.o \
				$(TARGET)\des_enc_new.o \
				$(TARGET)\dummy_new.o \
				$(TARGET)\Ecc_new.o \
				$(TARGET)\Ecdh_new.o \
				$(TARGET)\Ecdsa_new.o \
				$(TARGET)\Ecelgamal_new.o \
				$(TARGET)\encrypttext_new.o \
				$(TARGET)\esc2str_new.o \
				$(TARGET)\ex_data_new.o \
				$(TARGET)\Field163_new.o \
				$(TARGET)\get_ca_certificate_new.o \
				$(TARGET)\Group163_new.o \
				$(TARGET)\i2d_r_pr_new.o \
				$(TARGET)\i2d_r_pu_new.o \
				$(TARGET)\key_hash_new.o \
				$(TARGET)\key_op_new.o \
				$(TARGET)\manage_TCA_Cert_Info_new.o \
				$(TARGET)\md5_dgst_new.o \
				$(TARGET)\md5_one_new.o \
				$(TARGET)\md_rand_new.o \
				$(TARGET)\mem_new.o \
				$(TARGET)\parse_Hashed_Certificate_new.o \
				$(TARGET)\parse_Signed_Certificate_new.o \
				$(TARGET)\parse_wtls_new.o \
				$(TARGET)\parse_x509_new.o \
				$(TARGET)\proc_cert_response_new.o \
				$(TARGET)\rand_key_new.o \
				$(TARGET)\rand_lib_new.o \
				$(TARGET)\rsa_chk_new.o \
				$(TARGET)\rsa_eay_new.o \
				$(TARGET)\rsa_gen_new.o \
				$(TARGET)\rsa_lib_new.o \
				$(TARGET)\rsa_oaep_new.o \
				$(TARGET)\rsa_pk1_new.o \
				$(TARGET)\rsa_ssl_new.o \
				$(TARGET)\rsa_verify_new.o \
				$(TARGET)\seed_new.o \
				$(TARGET)\seed_rand_key_new.o \
				$(TARGET)\set_key_new.o \
				$(TARGET)\sha1dgst_new.o \
				$(TARGET)\sha1_one_new.o \
				$(TARGET)\signtext_new.o \
				$(TARGET)\signtext_rsa_new.o \
				$(TARGET)\sig_template_new.o \
				$(TARGET)\stack_new.o \
				$(TARGET)\verify_cert_new.o \
				$(TARGET)\verify_Hashed_Certificate_new.o \
				$(TARGET)\verify_Signed_Certificate_new.o \
				$(TARGET)\x509_verify_new.o \

$(MCOMMERCE_WPKI) : $(SRC_FILE)
	@echo ------------------------------------------------------------------
	@echo OBJECT $(@F)
	$(CC) $(CFLAGS) $(SKY_INC) -DGETPID_IS_MEANINGLESS -DL_ENDIAN -DSCRIPT_ECC $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------

SRC_DIR = SOURCE\MCommerce

MCOMMERCE_WALLET =$(TARGET)\mwcore_new.o \
				  $(TARGET)\mwdata_new.o \
				  $(TARGET)\mwgetdata_new.o \
                  $(TARGET)\mwscript_new.o \

$(MCOMMERCE_WALLET) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(SKY_INC) -DGETPID_IS_MEANINGLESS -DL_ENDIAN -DSCRIPT_ECC $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

MCOMMERCE_OBJS = $(MCOMMERCE_WPKI) \
                 $(MCOMMERCE_WALLET) \
!ENDIF

#-------------------------------------------------------------------------------
# Add here !!!
#-------------------------------------------------------------------------------
#SRC_DIR = 
#
#EQUS_XXXX_OBJS = $(TARGET)\
#
#$(EQUS_XXXX_OBJS) : $(SRC_FILE)
#        @echo ------------------------------------------------------------------
#        @echo OBJECT $(@F)
#        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SKY) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#        @echo ------------------------------------------------------------------

!IFDEF IRDA
#-------------------------------------------------------------------------------
# IrDA Module Porting
#-------------------------------------------------------------------------------

#----------
#  APP
#----------
SRC_DIR  = $(IRDA_DIR)
IRDA_OBJ = $(TARGET)\msmuart1.o \
		   $(TARGET)\msmuart2.o \
		   $(TARGET)\msmtimer.o \
		   $(TARGET)\irmodem.o  \
		   $(TARGET)\irdatool_b0.o

$(IRDA_OBJ) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_IRDA) $(IRDA_INC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#----------
#  LIB
#----------
SRC_DIR       = $(IRDA_IRUTY)
IRDA_IRUTYOBJ = $(TARGET)\irdauty.o

$(IRDA_IRUTYOBJ) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_IRDA) $(IRDA_INC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
##
SRC_DIR       = $(IRDA_IRSIR)
IRDA_IRSIROBJ = $(TARGET)\irsirapi.o \
                $(TARGET)\irhwif.o   \
				$(TARGET)\irsirtx.o  \
				$(TARGET)\irsirrx.o  \
				$(TARGET)\irtmrapi.o \
				$(TARGET)\irsircrc.o

$(IRDA_IRSIROBJ) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_IRDA) $(IRDA_INC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
##
SRC_DIR       = $(IRDA_PROTO)
IRDA_PROTOOBJ = $(TARGET)\irlap.o  \
                $(TARGET)\irlmp.o  \
				$(TARGET)\tinytp.o \
				$(TARGET)\ircomm.o \
				$(TARGET)\irmisc.o 

$(IRDA_PROTOOBJ) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_IRDA) $(IRDA_INC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
##
IRDA_OBJS = $(IRDA_OBJ)      \
            $(IRDA_IRUTYOBJ) \
		    $(IRDA_IRSIROBJ) \
		    $(IRDA_PROTOOBJ)

!ENDIF      # !IFDEF IRDA #


# set source folder
SRC_DIR = $(QUALCOMM_DIR)

#===============================================================================
# make Image file
#===============================================================================

STAMP_OBJ = $(OBJ_CMD) $(TARGET)\mobile.o
STAMP_SRC = $(SRC_DIR)\mobile.c

ALL_OBJECTS =    $(OBJECTS) \
!IFDEF WAP2
				 $(WAP_OBJECTS_ALL) \
!ENDIF
!IFDEF IRDA									# Enable IRDA profiling support
				 $(IRDA_OBJS) \
!ENDIF										# Enable IRDA profiling support
                 $(MELODY_OBJS) \
                 $(AUDIO_OBJS) \
				 $(SKY_DEVICE_IDLCD_OBJS) \
!IFDEF SMB
                 $(SMB_OBJS) \
!ENDIF
                 $(SKY_APP_CAMERA_OBJS) \
!IFDEF FEATURE_WAVELET  #yoursun 01/06/08
				$(SKY_APP_WAVELET_OBJS) \
				$(SKY_APP_WAVDECODE_OBJS) \
!ENDIF #FEATURE_WAVELET
!if "$(MODEL_ID)" == "MODEL_B0"
				 $(SMB_OBJS) \
!endif
                 $(FS_DEV_OBJS) \
                 $(FILESYS_OBJS) \
                 $(SMEMORY_OBJS) \
                 $(SKTT_RF_OBJS) \
#jrkwon       
!IFDEF  SOCKAPI   
				 $(SOCKAPI_OBJS) \
!ENDIF
!IFDEF GVM
				 $(GVM_OBJS) \
				 $(GVM_LIB_OBJS) \
				 $(GVM_ATSWAP_OBJS) \
!ENDIF
!IFDEF SKVM
				 $(SKY_APP_SKVM_OBJS) \
				 $(SKY_APP_SKVM_LIB_OBJS) \
!ENDIF
!IFDEF MCOMMERCE
				 $(MCOMMERCE_OBJS) \
!ENDIF
!IFDEF CHARACTER_BELL
				 $(SKY_APP_CHARACBELL_OBJS) \
!ENDIF
				 $(DSSKT_OBJS) \
				 $(EQUS_UTIL_MEM_OBJS) \
                 $(EQUS_STATE_OBJS) \
                 $(EQUS_UTIL_OBJS) \
                 $(EQUS_DISPLAY_OBJS) \
                 $(EQUS_CTRL_OBJS) \
                 $(EQUS_I18N_OBJS) \
                 $(EQUS_RES_IMG_OBJS) \
                 $(SKY_DEVICE_LCD_OBJS) \
                 $(SKY_I18N_OBJS) \
                 $(SKY_PLT_REX_OBJS) \
                 $(SKY_RES_IMG_OBJS) \
                 $(SKY_RES_MENU_OBJS) \
                 $(SKY_SYS_OBJS) \
#                 $(SKY_TEMP_OBJS) \
                 $(SKY_APP_SYS_OBJS) \
				 $(SKY_APP_EIF_OBJS) \
                 $(SKY_APP_PBOOK_OBJS) \
                 $(SKY_APP_BELL_VIB_OBJS) \
                 $(SKY_APP_PIC_OBJS) \
				 $(SKY_APP_CALC_OBJS) \
				 $(SKY_APP_SWATCH_OBJS)\
				 $(SKY_APP_WTIME_OBJS)\
                 $(SKY_APP_SCHEDULE_OBJS) \
                 $(SKY_APP_ALARM_OBJS) \
                 $(SKY_APP_MEMO_OBJS) \
!IFDEF MCOMMERCE
				 $(SKY_APP_MWALLET_OBJS) \
!ENDIF
!IFDEF PHOTOMAIL
                 $(SKY_APP_PM_OBJS) \
!ENDIF
!IFDEF VAS
                 $(SKY_APP_VAS_OBJS) \
!ENDIF
                 $(SKY_APP_SETTING_OBJS) \
				 $(SKY_APP_DATACFG_OBJS) \
                 $(SKY_APP_DATAMGR_OBJS) \
				 $(SKY_APP_CALLLIST_OBJS) \
                 $(SKY_APP_MENU_OBJS) \
                 $(SKY_APP_RECPLAY_OBJS) \
                 $(SKY_APP_SMS_OBJS) \
                 $(SKY_APP_VOCODER_OBJS) \
                 $(SKY_APP_AUTOANS_OBJS) \
!IFDEF IRDA
                 $(SKY_APP_IRDA_OBJS) \
!ENDIF                
                 $(SKY_APP_GAME_LIB_OBJS) \
                 $(SKY_APP_GAME_OBJS) \
                 $(SKY_APP_GAMEAPP_OBJS) \
                 $(SKY_APP_EMAIL_OBJS) \
                 $(SKY_APP_TEST_OBJS) \
				 $(SKY_APP_WAP_OBJS) 
#                 $(SKY_APP_MEMO_OBJS) \
#                 $(SKY_APP_CTRL_OBJS) \
#                 $(SKY_APP_SMS_OBJS) \


$(TARGET_EXE) : $(ALL_OBJECTS)
        @echo Recompile mobile.c for Time Stamp
        -$(CC) $(CFLAGS) $(DMSS_CFLAGS_MOB) $(MSG_FILE) $(STAMP_OBJ) $(STAMP_SRC)
        @echo ---------------------------------------------------------------
        @echo TARGET $@
        $(LD) -$(EXETYPE) $(LFLAGS) $(OBJ_CMD) $@ $(LIBS) -VIA <<
        $(ALL_OBJECTS)
<<

