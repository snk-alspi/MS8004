

/* 
 * File:   ERDL VERSION
 * Author: SANK
 * Comments:
 * Revision history: 
 * Initial version
 * HARD WARE : 
 * SOFTWARE  :
 * 
 * MODELS :
 * MS3600 -> 0 -> Host Alone
 * MS3610 -> 1 -> 4-20mA 
 * MS3620 -> 2 -> Bluetooth
 * MS3630 -> 3 -> RS232
 * MS3640 -> 4 -> 4-20mA & RS232
 * 
 * STARTING NEW VERSION WITH NUMBER V001
 * 
 * This version is tested in hardware and the following working :
 *  1) Display
 *  2) Keypad
 *  3) RTC
 *  4) SD CARD
 *  5) USB
 * 
 * 
 * FINISHED CHECKING USB -> 30JUN2016 version 6
 * FOR CHECKING ANALOG BOARD -> 01JUL2016
 * 
 * 11JULY 2016
 * KEYPAD TIMING ISSUE CORRECTION
 * 
 * VERSION V004: 27 August 2016
 *  1)www.alspi.com in display removed
 *  2)LCD orientation corrected
 *  3)mils display corrected
 *  4)Keypad alteration: Removed from timer interrupt and created pin Change interrupt
 * 
 * VERSION V004.01: 29 August 2016
 *  1)Corrections Date and time setting
 *  2)LCD orientation corrected
 
 * Revision V004.02  Date : 30 Aug 2016
 *  1. Power Down Corrected after taking measurement
 *  2. Keypad timing increased to 500ms from 400ms to avoid jumping
 *  3. Corrected errors in power down menu 
 *  4. Left side missing part of letters corrected
 *  5. Validation added for all menus    
 * 
 * * Revision V004.04  Date : 1 Sep 2016
 *  1. Started editing menus as per Bala's design
 *  2. First Screen finished
 *  3. Config file edited to accomadate 5 probe IDs after GSM Number
 *  4. Create config and read config functions edited for 5 probe IDs (Write config to be edited) 
*         
 
 * REVISION V004.043 DATE : 05 SEP 206
 * Working till display probe ids read from config file
 * IMPORTANT : WRITE CONFIG SHOULD BE CORRECTED
 * 
 * Revision V004.05 Date : 05 Sep 2016
 * Enter New Probe id started working..
 * Delete probe id created
 * 
 * Revision V004.06 Date : 07 Sep 2016
 * Config file handling edited as per new decesion.
 * 
 * Revision V004.07 Date : 08 Sep 2016
 * Measurement screen editing..
 * 
 * Revision V004.071 Date : 08 Sep 2016
 * Measurement section error correction.
 * 
 * Revision V004.072 Date : 08 Spe 2016
 * Measurement section error correction.
 * 
 * Revision V004.073 Date : 09 Sep 2016
 * Probe ID section modifications
 * 
 * Revision V004.074 Date : 09 Sep 2016
 * Probe ID Active probe corrected
 * Measurement accepts active probe id
 * 
 * Revision V004.075 Date : 10 Sep 2016
 * Probe types ADDed
 * Measurement screen errors edited
 * Measurement section: measure again added
 * reply from analog board left in order to verify correct Probe type is sent to analog board  IMPORTANT //SHOULD BE REMOVED//
 * 
 * Revision V004.076 Date : 10 Sep 2016
 * Reply from analog board removed
 * Measurement Screen locations edited
 * 
 * * Revision V004.077 Date : 10 Sep 2016
 * Code edited to receive check value and display it 
 * Metal loss correction
 * 
 * Revision V004.079 Date : 10 Sep 2016
 * Completed 3 screens Main screen, Probe screens and measurement
 * Tested and found working.
 * 
 * Revision V004.08 Date : 12 Sep 2016
 * Revision V004.08 Date : 12 Sep 2016
 *  escape sequence done
 * File transfer screen created
 * 
 * Revision V004.081 Date : 12 Sep 2016
 * Creation of file while creating porbe id
 * 
 * Revision V004.082 Date : 13 Sep 2016
 * File creation added pid details in file
 * File deleted added in delete probe function
 * Enter probe validation added
 * Config menu created
 * set date and time completed
 * status screen created
 * 
 * Revision V004.083 Date : 13 Sep 2016
 * Config menu up key corrected
 * Del PID corrected
 * RTC update in SD functions
 * 
 *  Revision V004.084 Date : 14 Sep 2016
 * Probe entry screen edited back to same screen if error in input data
 * RTC update in SD functions
 * 
 * Revision V004.085 Date : 14 Sep 2016
 * Probe entry screen duplicate ID validation
 * 
 *  Revision V004.086 Date : 14 Sep 2016
 * Probe entry screen duplicate ID validation correction
 * entry menu status option selection correction
 * Date in file creation correction
 * 
 * Revision V004.087 Date : 15 Sep 2016
 * Probe entry screen duplicate ID validation corrected
 * Date in file creation correction
 * Status menu completed
 * Display Power down time menu created
 * Battery voltage warning menu created
 * 
 * Revision V004.088 Date : 15 Sep 2016
 * ADC correction
 * alarm menu created
 * 
 * Revision V004.089 Date : 15 Sep 2016
 * interval implementation
 * 
 * Revision V004.0891 Date : 17 Sep 2016
 * ADC Correction
 * Interval implementation checking
 * Probe entry screens tune-up
 * Power Down time implemented
 * 
 * Revision V004.0892 Date : 20 Sep 2016
 * ADC calculation Correction 
 * SET ALARAM display centered
 * Set Alaram Menu default set to hours
 * Set Alaram right key movement corrected
 * SD Free space error corrected
 * Display pwr dn alaram page corrected
 * Low battery warning menu added with password
 * Alaram Measurement Display corrected.
 * 
 * Revision V005.01 Date : 20 Sep 2016
 * Bluetooth started
 * 
 * Revision V005.02 Date : 21 Sep 2016
 * Bluetooth correction
 * 
 * Revision V005.03 Date : 22 Sep 2016
 * Bluetooth correction
 * Measurement correction
 * usb functions added
 * 
 * Revision V005.04 Date : 22 Sep 2016
 * Bluetooth correction
 * 
 * Revision V005.05 Date : 22 Sep 2016
 * Bluetooth correction
 * Bluetooth screen correction
 * 
 * Revision V005.06 Date : 23 Sep 2016
 * Bluetooth correction
 * USB file transfer correction
 * 
 * Revision V005.09 Date : 27 Sep 2016
 * Bluetooth correction
 * USB file transfer correction
 * 
 * Revision V005.09_1 Date : 28 Sep 2016
 * USB file transfer correction
 * 
 * Revision V005.09_3 Date : 28 Sep 2016
 * Bt File Working 
 * Testing
 * 
 * Revision V005.09_4 Date : 30 Sep 2016
 * Bt File Working 
 * Checking BT upload confing
 * 
 * Revision V005.09_5 Date : 1 Oct 2016
 * Bt File Working 
 * screen correction
 * 
 * Revision V005.09_6 Date : 3 Oct 2016
 * Bt File transfer edit
 * screen correction
 * Metal Loss in main screen correction
 * ML is changed to METAL LOSS in Display
 * Memory available
 * Select probe centered
 * 
 * Revision V005.09_7 Date : 4 Oct 2016
 * Bt File transfer edit
 * 
 * Revision V005.09_8 Date : 4 Oct 2016
 * Measurement correction
 * 
 * Revision V005.09_9 Date : 4 Oct 2016
 * Measurement correction
 * 
 * Revision V005.10_1 Date : 4 Oct 2016
 * Measurement correction
 * 
 * Revision V005.10_2 Date : 5 Oct 2016
 * Revision V005.10_2 Date : 5 Oct 2016
 * Display heading full reverse 
 * PID changed to 8 digits
 * 
 * Revision V005.10_3 Date : 5 Oct 2016 
 * PID changed to 10 digits
 * Unit ID menu added 
 *
 * 
 * Revision V005.10_4 Date : 8 Oct 2016 
 * Function Description in Display.C added
 * view previous data coding started
 * 
 * Revision V005.10_5 Date : 12 Oct 2016 
 * Heading Numbers removed
 * File copy menu edited
 * Active probe symbol edited
 * 
 * Revision V005.10_6 Date : 12 Oct 2016 
 * Probe enter problem corrected
 * LEDs swaped
 * 
 * Revision V005.10_7 Date : 12 Oct 2016 
 * Read config & write config corrected to accomadate 10digit pid and 20 digit unit id
 * 
 * 
 * Revision V005.10_8 Date : 14 Oct 2016 
 * Available PID correction
 * Displayed interrupt register of rtc for debugging   // TO BE REMOVED
 * 
 * Revision V005.10_9 Date : 14 Oct 2016 
 * Now Shows no PID when new
 * Trying to correct No Space error even no PID is available 
 * 
 * Revision V005.11_0 to V005.11_2 Date : 14 Oct 2016 
 * Cleared all the Enter probe menu
 * Probe Set menu Found working properly
 * 
 * Revision V005.11_3 Date : 15 Oct 2016 
 * Created a file to store the last reading
 * Measurement will not taken if the battery voltage will be less than 5V
 * 
 * Revision V005.12_4 Date : 20 Oct 2016 
 * display last readings.
 * Manual Measurement : Will Save only after reading.
 * Low battery checking added to bluetooth
 * Low space SD card checking added
 * 
 * Revision V005.12_5 Date : 20 Oct 2016 
 * Low space in SD Corrected
 * 
 * Revision V005.12_6 Date : 21 Oct 2016 
 * Comma added between time and date in data file.
 * Config file edited to accomadate individual alarm for each PID
 *  1) Variables alarm_mode, alarm_onoff, alarm_time added
 *  2) Create config edited 
 *  3) Read Config edited
 *  4) Write config edited
 * Enter key introduced to move to next option
 * 
 * Revision V005.12_7 Date : 24 Oct 2016 
 * Measurement : No PID available for measurement
 * Last measurement reset if active pid changed
 * Trailing zeros in PID is removed on Main Screen, Measurement screen (manual & auto)
 * Mins for measurement added
 * 
 * Revision V005.12_8 Date : 24 Oct 2016 
 * Alarm time menu : Probe ID & details added
 * 
 * Revision V005.12_9 Date : 25 Oct 2016 
 * PID zero's replaced with space
 *  1) Changed in Create config
 *  2) Read Config
 * 
 * Revision V005.12_10 Date : 26 Oct 2016 
 * Removed change prid in display.c (which is not used)
 * Interval format changed
 *  1) Mins added
 *  2) Read config -> Removed read interval section
 *  3) Create config -> Removed creation of interval entry
 *  4) Write Config -> Removed creation of interval section
 * alarm menu edited to show mins, hours & days
 * New snooze alarm 2 created
 * 
 * Revision V005.12_11 Date : 26 Oct 2016 
 * All Default values of PID's are set to '0' for testing
 * Revision V005.12_12 Date : 26 Oct 2016 
 * Revision V005.12_13 Date : 26 Oct 2016 
 * Revision V005.12_14 Date : 26 Oct 2016
 * RTC: Snooze alarm corrected  
 * 
 * Revision V005.12_15 Date : 26 Oct 2016 
 * Added read / write alarm time(Separate entries)
 * 
 * Revision V005.12_16 Date : 27 Oct 2016 
 * Cleaned all the warnings regarding strings
 * 
 * Revision V005.12_17 Date : 27 Oct 2016 
 * Increased reading buffer rbuff from 30 to 50  --------- THIS SOLVES THE PID PROBLEM ---------
 * 
 * Revision V005.12_18 Date : 27 Oct 2016 
 * Revision V005.12_19 Date : 27 Oct 2016 
 * Alarm menu values problem corrected
 * 
 * Revision V005.12_20 Date : 28 Oct 2016 
 * Alarm mins from 59 to 60
 * Last reading format changed
 * Read Last Reading file also changed
 *
 * 
 * Revision V005.12_21 Date : 28 Oct 2016
 * Alarm working fine in minutes 
 * Alarm format -> alarm Mode changed 
 *      0-> alarm off
 *      1-> Minutes
 *      2-> Hours
 *      3-> days 
 * Alarm Menu Edited
 * Read config edited
 * Write Config edited
 * Create config edited
 * -------- DAYS SETTING YET TO BE IMPLEMENTED ----------------
 * 
 * 
 * Revision V005.12_22 Date : 31 Nov 2016 
 * Power down enable added at end of measurement
 * Probe sel menu -> Previous menu -> Corrected.
 * No SD Card warning at entering of menu
 * Front screen display values only if SD Card is available
 * Front screen last line -> corrected
 * Alarm menu-> Space added 
 * Alarm menu -> Current time moved up
 * Booting - Key Press problem solved
 * Manufacturer config, Batt. voltage removed
 * alarm menu saving problem -> corrected
 * 
 * Revision V005.12_23 Date : 1 Nov 2016 
 * Un commented the lines for used for checking
 * 
 * Revision V005.12_24 Date : 1 Nov 2016 
 * Next Alarm set at the end of power down if alarm is ON for the active pid
 * Bluetooth put in Reset to save power
 * USB File creation with Unit ID
 * Power Enable and Disable added at measurement subroutine
 * Power Enable and Disable added at USB subroutine
 * Normal Display-> Last line -> corrected
 * Last Reading -> Reset problem corrected
 * Logging.txt -> Removed again
 * Menu button (from main screen) will not work without SD Card
 *                  
 * Revision V005.12_25 Date : 2 Nov 2016 
 * Alarm happens in next day problem -> Corrected, check and update
 * Date & time menu -> Individual digits editing enabled
 * View last readings -> Corrected -> check and update
 * 
 * Revision V005.12_26 Date : 3 Nov 2016 
 * Removed commented lines used for my checking
 * 
 * Revision V005.12_27 Date : 3 Nov 2016 
 * Removed commented lines used for my checking
 * Snozee alarm display comments commented Lines 650-670, 612-615
 * 
 * Revision V005.12_28 Date : 7 Nov 2016 
 * Setting alarm at power down commented for checking
 * Added delay to make centre of reading in RTC Read (SPI)
 * RTC Init edited as in other code.
 * 
 * Revision V005.12_29 Date : 7 Nov 2016 
 * Set Next alarm display reactivated for testing.
 * 
 * Revision V005.12_30 Date : 11 Nov 2016 
 * Time validation added
 * 
 * Revision V005.12_31 Date : 11 Nov 2016 
 * Alarm problem - Edited lines 252, 256 in rtc.c. See there for description
 * 
 * Revision V005.12_32 Date : 12 Nov 2016 
 * Date & Time setting -> Validated / corrected
 * Measurement power down after measuring section added
 * Power up & down of measurement board in Manual measurement
 * Power up & down of measurement board in BT measurement
 * Power up & down of measurement board in USB menu
 * Enter to save in measurement problem solved
 * Recall readings -> Single screen corrected, up and down key to be implemented
 * 
 * Revision V005.12_33 Date : 12 Nov 2016 
 * Day roll over corrected rtc.c line 599
 * Revision added at the starting of main()
 * Status menu -> Revision corrected
 * Front screen -> Metal loss value corrected.
 * 
 * Revision V005.12_34 Date : 16 Nov 2016 
 * Keys changed global variables.h line 172
 * 
 * Revision V005.12_34 Date : 16 Nov 2016 
 * Keys Correction global variables.h line 172
 * Navigation in menus Up key and Dn key interchanged
 * 
 * Revision V005.12_35 Date : 23 Nov 2016 
 * 10 to 20 Secs lag in reading corrected by setting secs to 0 always.
 * 
 * Revision V005.12_36 Date : 23 Dec 2016 
 * Reset alarm on power down of manual ON
 * Display reset : timing increased to solve no display problem.
 * 
 * Revision V005.12_37 Date : 30 Mar 2017
 * Moved lcd Init to top to clear display problem
 * 
 * Revision V005.12_39 Date : 26 June 2017
 * REMOVED COMMENTED ITEMS
 * COMPLETE Checking 
 * In add probe warning screen missing letters corrected.
 * Low Battery warning message timing increased.
 * File transfer selection coding corrected.
 * UNIT ID limited to 10 Characters from previous 20 characters.
 * Measurement display corrected. Commented out code used for testing.
 * 
 * Revision V005.12_40 Date : 01 July 2017
 * Display Refresh corrected.
 * Lights Red & Green corrected.
 * Battery low menu letters corrected
 * Password changed to 3333
 * Frimware version moved to front screen
 * Last readings corrected
 * Metal Loss at front screen corrected
 * USB device checking corrected.
 * 
 * -------------------------------------------------------------------------------------
 * Revision V005.13_00 Date : 04 July 2017
 * Adding model numbers
 * Included DEE Emulation 16-bit.C & .H files
 * Still not able to write / read FLASH   -----> TO BE DONE
 * Added DEVICE ID to the menu.
 * 
 * Revision V005.13_01 Date : 08 July 2017
 * Changing config file to add Device ID
 *           ------------------
 * Versions 13 left to maintain old version line till all corrections are made.
 * ---------------------------------------------------------------------------------------
 * 
 *  Revision V005.12_41 Date : 10 July 2017
 * Adding model numbers -> NOT DONE
 * Included DEE Emulation 16-bit.C & .H files
 * Still not able to write / read FLASH   -----> TO BE DONE
 * Added DEVICE ID to the menu.
 * Changing config file to add Device ID
 * Copying to USB Stick corrected and checked 5 times.
 * Compiler Version Changed to XC16 v1.31 (for testing flash writing, but failed writing).
 * 
 * 
 * Revision V005.12_42 Date : 11 July 2017
 * Adding model numbers Temperovarily stored in SD Card in last line.
 * Create config, Read config & Write config edited to add Model Number.
 * Inserted -420mA coding...  
 * Bluetooth Menu changed to Communication Menu
 * Different initialisation of UART4 for BT and Serial port
 * Initialise 4-20mA SPI if model is 1 or 4
 * UART section working.
 * 
 * Revision V005.12_43 Date : 17 July 2017
 * Error codes added for measurement section
 * Problem found in Readings.  Frequent Measurement Error 2 faced. To solve this
 * UART initialization edited. Two of the settings share same POR register.
 * Problem found: If the unit powered between alarm measurements, The next alarm
 * fails. Resetting interrupts in 2 places removed. one in main and another in
 * RTC init.
 * Details for probe is stored in flash- decided.
 * 
 * ////////////////////////////// Flash address for model and probe details ////////////////////////
 * Address          Details
 * 3              Model Number
 * 
 * 
 * 
 * 
 * 
 * /////////////////////////////////////////////////////////////////////////////////////////////////
 * 
 * Revision V005.12_44 Date : 20 July 2017
 * Issues to be solved..
 * 
    Current is too high (80mA) , I assume that you have turned on all transistors. The Measurement section power can be turned off. After measurement the current goes to low(34mA)
    After USb copying the current is reducing further down? it means after USB intialisation you are closing some functions and it reducing the current. Please note that the 
 * current is high before inserting USB (33mA) and after copying and removing the USB the current goes to 18mA.
    USB is not copying the files.
    USB section is still with your testing firmware hence all characters shows up
    Measurement screen some characters shows up during measurement in third/fourth line,You can add a text permanently if it is better for diagnosis
    Battery voltage is not changing at all .
    Please do as below
        During intial power ensure all the pins and unwanted thins are off in initialization condition
        Check and ensure unnecessary coding which is not required for continuous loop , this also reduces some current.
    During Alarm interval the display can be turned off and the current may go further down.
 * Action:
 * USB power disabled after checking alarm on section.
 * Extra characters in Measurement screen removed.
 * 
 * 
 * Revision V005.12_45 Date : 21 July 2017
 * Default values for unit id and device id changed to 9999999999
 * Voltage calculation corrected as per Bala's advice
 * USB copying not working without powering up the USB at startup
 * 
 * Revision V005.12_46 Date : 28 July 2017
 * Trying to reduce measurement time
 * Display strart uptime reduced to 100ms from 1 sec
 * Found calculations were wrong for measuring volatage --> to be corrected.
 * Voltage checking for Manual measurement added
 * Few display commands removed / edited to reduce time in USB Copying.
 * Last few readings -> corrected.
 * Copying Config file from USB Stick started.
 * Error code write in log file missing is added again in this version
 * Red LED lits when an error occurs.
 * 
 * Revision V005.12_47 Date : 04 Aug 2017
 * Power down in usb mode corrected to avoid powering up on menu.
 * Probe ID, Unit ID, Device ID reduced to 8 digits.
 * Create config reduced to 8 digits.
 * Read config reduce to 8 digits.
 * Write Config reduced to 8 digits.
 * All sprintf commands using %d changed to %u (for correcting error in last few readings).
 * Initialising PROBE deatils for zero -> No active PID.
 * 
 * Revision V005.12_48 Date : 09 Aug 2017
 * Corrected alaram section.
 * Measurement time reduced to save battery.
 * Last measurement took around 30 secs.
 * 
 * Revision V005.12_49 Date : 10 Aug 2017
 * Added current alarm time in alarm setting page
 *  
 * Revision V005.12_50 Date : 11 Aug 2017
 * Corrected usb stick to device copying.
 * Device ID placed in Flash and can be written only once. - checked and working fine.
 * Removed Device ID and Model from CONFIG.TXT  
 *              Removed from wr_config
 *              Removed from rd_config
 *              Removed from create_config
 * Corrected extra digits shown in Device_ID menu
 * Corrected extra digits shown in UnitID menu
 * Corrected error showing PL in normal screen
 * 
 * Revision V005.12_51 Date : 12 Aug 2017
 * USB Copying : All files problem corrected. - to be checked
 * Copying config corrected - to be checked
 * Removed "MSC" Before PID in file name
 * Individual alarm is removed in all places -> alarm working should be checked
 * 
 * Revision V005.12_52 Date : 13 Aug 2017
 * Password 5th Digit hidden
 * "Enter New Time" added to Alarm screen
 * USB correction -> added 2sec delay to existing timing.
 * 
 * Revision V005.12_53 Date : 14 Aug 2017
 * Device ID changed to Serial No. in Status Page
 * In Normal Display, ID is changed to PID
 * Added existing unit_id to be displayed in unitID menu
 * Removed Model in front screen and replaced it with Unit ID.
 * 
 * Revision V005.12_54 Date : 14 Aug 2017
 * Version corrected to show in capital letters
 * Serial number corrected to show in capital letters
 * 
 * Revision V005.12_55 Date : 15 Aug 2017
 * get last counter moved to old space to correct the serial number in data file.
 * 
 * Revision V005.12_56 Date : 16 Aug 2017
 * get last counter subroutine corrected.
 * Active probe change is not saved.  Corrected.
 * 
 * Revision V005.12_57 Date : 16 Aug 2017
 * get last counter calling in Wr_sd checking.
 * 
 * Revision V005.12_58 Date : 30 Aug 2017
 * Alarm Hours mode problem
 * 
 * Revision V005.12_59 Date : 13 Sep 2017
 * Alarm Hours mode problem
 * 
 * Revision V005.12_60 Date : 07 Oct 2017
 * Checking USB Copying.,.
 * Added all debug information to test USB Copying.
 * 
 * *****  DEBUG INFORMATION SHOULD BE REMOVED LATER *********
 * 
 * Testing Started.
 * 
 * Revision V005.12_61 Date : 25 Oct 2017
 * 
 * *********** DEBUG INFORMATION DELETED ***************
 * 
 * Code added to send Data to 4-20mA if model is selected while manual measuring.
 * Testing code removed in USB section. (Unwanted lines displayed is removed) in SD_Card.c
 * 
 * Revision V005.12_62 Date : 26 Oct 2017
 * USB section messages alteration
 * 
 * Revision V005.12_63 Date : 30 Oct 2017
 * Correction in BT initialisation in main.c
 * Removed line displaying next alarm set at in alarm menu.
 * Spelling Correction
 * Battery Voltage moved to separate function.
 * ADC is sampled 200 times. 
 * 
 * Revision V005.12_64 Date : 10 Nov 2017
 * Set voltage uses fixed values corrected. Replaced with variable "set_vol".
 * Battery Voltage Calculation correction
 * Only one instance of "low_vol" variable and is replaced with set_vol.
 * Variable "low_vol" is removed.
 * Recall Readings: If only one PID available, the cursor stays at 1st PID instead of zero.
 * 
 * Revision V005.12_65 Date : 11 Nov 2017
 * Device ID Display characters corrected.
 * Recall Readings: Empty file hanging problem solved.
 * Config File : Error checking added.
 * 
 * Revision V005.12_66 Date : 23 Nov 2017
 * This version is started to correct the errors occured in new hardware.
 * Unit ID and Probe ID limited to 7 digits.
 * 
 * Revision V005.12_67 Date : 23 Nov 2017
 * USB Correction: Old data file if available in the USB will be deleted and new file created/copied to the
 *           USB Stick. This is to avoid Append of whole data to the exisiting file.
 * 
 * Revision V005.12_68 Date : 28 Nov 2017
 * Corrosion Rate calculation added.
 * 
 *  Revision V005.12_69 Date : 04 Dec 2017
 * Model number added 3500 series added.
 * 
 * Revision V005.12_70 Date : 05 Dec 2017
 * 4-20mA Loop testing added.
 * 
 * Revision V005.12_71 Date : 08 Dec 2017
 * On entering the first probe it is made as active probe as default.
 * 
 * Revision V005.12_72 Date : 09 Dec 2017
 * Loop Current set to 4mA if any Error occurred during measurement.
 * 
 * Revision V005.12_73 Date : 12 Dec 2017
 * Loop port pins initialised and de initalised before and after using ports.
 * UART initialisation moved to separate function and called when needed.
 * 
 * Revision V005.12_74 Date : 13 Dec 2017
 * Unit ID shows only 7 char. -> Corrected.
 * Removed powering up usb/measurement section.
 * Removed all Power Down timer
 * Added Resetting Power Down Timer while writing USB
 * Added Resetting After measurement (to prevent premature power down after measurement
 * Removed Mconfig functions as it is no longer used.
 * Low battery warning setup added and included in Unit ID menu under specific password.
 * 
 * Revision V005.12_75 Date : 06 Feb 2018
 * Removed restriction to save serial number only once.  Now serial number can be saved any number of times.
 * Restricted minutes mode in alarm.  Added variable in mins enable to check minutes in alarm is selected or not
 * 
 * Revision V005.12_76 Date : 27 Feb 2018
 * Inserting MICROCOR code 
 * Probe Type increased from 7 to 8.
 * New Probe is named as "MC-E"
 * 
 * Revision V005.12_77 Date : 17 Mar 2018
 * Delete probe deletes a different file -> Removed Deleting file
 * Power Down Time disabled in USB Menu.
 * Power OFF Command Added
 * ///////////////////////////////////////////////////////////////
 *  PRESSING menu AND back KEY AT THE SAME TIME POWER OFFS THE UNIT
 * //////////////////////////////////////////////////////////////
 * 
 *  Revision V005.12_77_a Date : 20 Mar 2018
 * SD Card losses file or get corrupted issue solving
 *  - Added a flag to sens whether file in open and close it before power down.
 * 
 * *  Revision V005.12_78 Date : 22 Mar 2018
 * SD Card losses file or get corrupted issue solving
 * Config data to be saved in flash.
 * 
 *   Revision V005.12_79 Date : 03 Apr 2018
 * Config Bit edited
 * Check Reading converted to 65535 in Display.c
 * 
 * Revision V005.12_7980 Date : 09 Apr 2018
 * Config Bit edited
 * Factory Reset added under Unit ID menu
 * On Factory reset, Model Number and Dev ID (Serial Number) will remain same.
 * 
 * 
 * 
 */



// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

