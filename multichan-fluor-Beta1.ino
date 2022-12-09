#include <ADC.h>
#include <ADC_Module.h>
#include <ADC_util.h>
#include <SD.h>
//#include <SPI.h>
#include <TimeLib.h>
#include <string.h>

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
File root;
int fileCountOnSD = 0;
File myFile;
File myFile2;
File myFile3;
const int chipSelect = BUILTIN_SDCARD;
int AUTO = 1; //0 sets single aquisitions, 1 sets auto aquistion.
const int BEEFY = 3250;  //Size of arrays to be used to collect samples. 
const int BEEF = 100;  //Size of arrays to be used to collect samples. 
int REP = 1;  //Number of repetitions of the full protocol
int DELAYED = 2000;
int del = 200;  //Delay between flashes in protocol
const int SIG = A0;   //Analog channel used for reading fluorescence signal
ADC *adc = new ADC(); // adc object
int sample0 = 100;  //Size of array dictates number of samples taken/ length of flash. Each sample is 1.18 useconds
int sample1 = 100;  //Size of array dictates number of samples taken/ length of flash. Each sample is 1.18 useconds
int sample2 = 100;  //Size of array dictates number of samples taken/ length of flash. Each sample is 1.18 useconds
int sample3 = 100;  //Size of array dictates number of samples taken/ length of flash. Each sample is 1.18 useconds
int initial = 20; //Number of samples to be recorded prior to flash protocol. this is used to normalize F0.
int tau0 = 62;
int tau1 = 62;
int tau2 = 62;
int tau3 = 62;
float tauexp = 1.1;
int annn=50;
int ann=50;
int TOIMER1 = 70;
int TOIMER2 = 490;  //was at 490
String fourthy = "Dat";
String fourn = "Dat";
String curvesFile = "Datr.csv";
String dataFile = "Datd.csv";

int DeletedCount = 0;
int FolderDeleteCount = 0;
int FailCount = 0;
String rootpath = "/";


//String PSIFile = "DatPSI.csv";
int time[BEEFY]; //Time constant. Adjusts automatically to size of array/number of samples.
float TIMEX0[BEEFY];  //Time constant. Adjusts automatically to size of array/number of samples.
float TIMEX1[BEEFY];  //Time constant. Adjusts automatically to size of array/number of samples.
float TIMEX2[BEEFY];  //Time constant. Adjusts automatically to size of array/number of samples.
float TIMEX3[BEEFY];  //Time constant. Adjusts automatically to size of array/number of samples.

float TAUEX0[BEEF];  //Time constant. Adjusts automatically to size of array/number of samples.
float TAUEX1[BEEF];  //Time constant. Adjusts automatically to size of array/number of samples.
float TAUEX2[BEEF];  //Time constant. Adjusts automatically to size of array/number of samples.
float TAUEX3[BEEF];  //Time constant. Adjusts automatically to size of array/number of samples.

float timepersample0; //Time constant. Adjusts automatically to size of array/number of samples.
float timepersample1; //Time constant. Adjusts automatically to size of array/number of samples.
float timepersample2; //Time constant. Adjusts automatically to size of array/number of samples.
float timepersample3; //Time constant. Adjusts automatically to size of array/number of samples.

float timepertau0; //Time constant. Adjusts automatically to size of array/number of samples.
float timepertau1; //Time constant. Adjusts automatically to size of array/number of samples.
float timepertau2; //Time constant. Adjusts automatically to size of array/number of samples.
float timepertau3; //Time constant. Adjusts automatically to size of array/number of samples.

float sampletimey0;
float sampletimey1;
float sampletimey2;
float sampletimey3;

float sampletau0;
float sampletau1;
float sampletau2;
float sampletau3;

int PARcol0 = 40200;
int PARcol1 = 35620;
int PARcol2 = 30050;
int PARcol3 = 30500;

float valpar;
int increment = 1; //increment in the curvefit routine. The higher the number, the less data is used. Saves considerable time.

//FOR Channel 0 LED EXCITATION READINGS
//float COL0Ref[BEEFY]; // variable to store the value coming from Reference Channel
float COL0Sig[BEEFY]; // variable to store the value coming from Fluoresence Channel
float COL0Tau[BEEF]; // variable to store the value coming from Fluoresence Channel
float COL0TauNorm[BEEF];
//float COL0AveRef[BEEFY]; // Average Reference readings based on each REP
float COL0AveSig[BEEFY]; // Average Fluorecence readings based in each REP
float COL0Norm[BEEFY]; // Normalize Fluorescence readings by Reference.
float COL0PreS; //Average Fluorsence reading prior to excitation
float COL0PreR; //Average Reference reading prior to excitation
float COL0LINREG[5]={0,0,0,0,0}; //Array to store initial Lin Regression values and readings from average and normalized data.
float COL0fit[BEEFY]; //variable to store fit curve

//FOR Channel 1 LED EXCITATION READINGS
//float COL1Ref[BEEFY]; // variable to store the value coming from Reference Channel
float COL1Sig[BEEFY]; // variable to store the value coming from Fluoresence Channel
float COL1Tau[BEEF]; // variable to store the value coming from Fluoresence Channel
float COL1TauNorm[BEEF];
//float COL1AveRef[BEEFY]; // Average Reference readings based on each REP
float COL1AveSig[BEEFY]; // Average Fluorecence readings based in each REP
float COL1Norm[BEEFY]; // Normalize Fluorescence readings by Reference.
float COL1PreS; //Average Fluorsence reading prior to excitation
float COL1PreR; //Average Reference reading prior to excitation
float COL1LINREG[5]={0,0,0,0,0}; //Array to store initial Lin Regression values and readings from average and normalized data.
float COL1fit[BEEFY]; //variable to store fit curve

//FOR Channel 2 LED EXCITATION READINGS
//float COL2Ref[BEEFY]; // variable to store the value coming from Reference Channel
float COL2Sig[BEEFY]; // variable to store the value coming from Fluoresence Channel
float COL2Tau[BEEF]; // variable to store the value coming from Fluoresence Channel
float COL2TauNorm[BEEF];
//float COL2AveRef[BEEFY]; // Average Reference readings based on each REP
float COL2AveSig[BEEFY]; // Average Fluorecence readings based in each REP
float COL2Norm[BEEFY]; // Normalize Fluorescence readings by Reference.
float COL2PreS; //Average Fluorsence reading prior to excitation
float COL2PreR; //Average Reference reading prior to excitation
float COL2LINREG[5]={0,0,0,0,0}; //Array to store initial Lin Regression values and readings from average and normalized data.
float COL2fit[BEEFY]; //variable to store fit curve

//FOR Channel 3 LED EXCITATION READINGS
//float COL3Ref[BEEFY]; // variable to store the value coming from Reference Channel
float COL3Sig[BEEFY]; // variable to store the value coming from Fluoresence Channel
float COL3Tau[BEEF]; // variable to store the value coming from Fluoresence Channel
float COL3TauNorm[BEEF];
//float COL3AveRef[BEEFY]; // Average Reference readings based on each REP
float COL3AveSig[BEEFY]; // Average Fluorecence readings based in each REP
float COL3Norm[BEEFY]; // Normalize Fluorescence readings by Reference.
float COL3PreS; //Average Fluorsence reading prior to excitation
float COL3PreR; //Average Reference reading prior to excitation
float COL3LINREG[5]={0,0,0,0,0}; //Array to store initial Lin Regression values and readings from average and normalized data.
float COL3fit[BEEFY]; //variable to store fit curve

int result[BEEFY]; //SET up for simulatneous ADC reading
int i=0;
int t=0;
int g=0;

//UW
const int Col0 = 2; //digitalWrite pin for UV LED excitation -blue-label-in-code-
const int Col1 = 9; //digitalWrite pin for BLUE LED excitation -red-label-in-code
const int Col2 = 4; //digitalWrite pin for GREEN LED excitation -green-label-in-code
const int Col3 = 3; //digitalWrite pin for CYAN LED excitation -orange-label-in-code

//BENCH TOP
//const int Col0 = 2; //digitalWrite pin for UV LED excitation -blue-label-in-code-
//const int Col1 = 3; //digitalWrite pin for BLUE LED excitation -red-label-in-code
//const int Col2 = 4; //digitalWrite pin for GREEN LED excitation -green-label-in-code
//const int Col3 = 9; //digitalWrite pin for CYAN LED excitation -orange-label-in-code

const int FR = 5; //digitalWrite pin for Far Red
const int Actinic = 10; //digitalWrite pin for GREEN LED excitation -green-label-in-code
const int work = 25; //Turns slave microcontroller on
const int timers = 28;
const int MAG1 = 37;
const int MAG2 = 36;

U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
#define INFO_SCREEN_DELAY 3000


void setup() 
{
     u8g2.begin();
     Serial.begin(115200);
     Serial4.begin(9600);
     Serial.setTimeout(5000);
     pinMode(Col0, OUTPUT);
     pinMode(Col1, OUTPUT);
     pinMode(Col2, OUTPUT);
     pinMode(Col3, OUTPUT);
     pinMode(FR, OUTPUT);
     pinMode(work, OUTPUT);
     pinMode(Actinic, OUTPUT);
     pinMode(timers, OUTPUT);
     pinMode(SIG, INPUT);
     pinMode(MAG1, INPUT);
     pinMode(MAG2, INPUT);
     analogWriteResolution(12);
     
     adc->adc0->setAveraging(1);
     adc->adc0->setResolution(12);
     adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
     adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed

     //adc->adc0->enableInterrupts(adc0_isr);
     //adc->adc0->startContinuous(SIG);

     ////// ADC1 /////
     #ifdef ADC_DUAL_ADCS
     adc->adc1->enableInterrupts(adc1_isr);
     adc->adc1->setAveraging(1); // set number of averages
     adc->adc1->setResolution(12); // set bits of resolution
     adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
     adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed
     #endif
}


void loop() 
{        
   if (AUTO == 0)
   {   serialInput(); //search for serial input of new variables
       timeSave();
 
       analogWrite(Actinic, 4095);
       digitalWrite(work, LOW);
       digitalWrite(FR,LOW);
       //newdata=false; //reset PSI serial intake pipe
       delay(1000);
   } 
   if (AUTO == 1)
   {   serialInput();     //search for serial input of new variables
       analogWrite(Actinic, 4095);
       digitalWrite(FR,LOW);
       digitalWrite(work, HIGH);
       data();            //data aqcuisition
       curvefitter();     //runs curve fit protocol
       draw_m0_t(COL0LINREG[1], COL0LINREG[3], fourn);
       CURVEdataPrint();  //print normalized induction curves to serial
       PHOTOdataPrint();  //print calculated photochemical values to serial
       valpar = 0;
       //dataSav();         //write to SD card 
       AUTO = 0;
       Serial.println("complete run");
       Serial.println("complete run");

       //delay(2000);
   }  
   if (AUTO == 2)
   {   serialInput();     //search for serial input of new variables
       digitalWrite(work, HIGH);
       int temp = del;
       del=6000;
       int tempRep = REP;
       REP=1;

       data();            //data aqcuisition
       curvefitter();     //runs curve fit protocol
       draw_m0_t(COL0LINREG[1], COL0LINREG[3], fourn);
       CURVEdataPrint();  //print normalized induction curves to serial
       PHOTOdataPrint();  //print calculated photochemical values to serial
       valpar = 0;
       dataSav(); 

       del=temp;
       REP=tempRep;

       float LightCurve[23]= {4094, 2110, 2110, 2110, 2110, 2110, 2110, 2110, 2110, 2140, 2140, 2140, 2140, 2070, 2070, 2070, 2070, 2070, 2070, 4095, 4095, 4095, 4095};
       float LightCurveSteps[23] = {0, 200, 200, 200, 200, 200, 200, 200, 200, 100, 100, 100, 100, 450, 450, 450, 450, 450, 450, 0, 0, 0, 0};
              
       int STEPS = 23;
       for (int W = 0; W < STEPS; W++)
       {   
           float valued = (LightCurve[W]);
           valpar = (LightCurveSteps[W]);
           analogWrite(Actinic, valued);
           delay(DELAYED);
           data();            //data aqcuisition
           curvefitter();     //runs curve fit protocol
           draw_m0_t(COL0LINREG[1], COL0LINREG[3], fourn);
           CURVEdataPrint();  //print normalized induction curves to serial
           PHOTOdataPrint();  //print calculated photochemical values to serial
           dataSav();         //write to SD card 
       }
       AUTO = 0; 

       Serial.println("complete run");
       Serial.println("complete run");
   }    
   if (AUTO == 10)
   { 
      analogWrite(Actinic, 95);
      delay(200);
      analogWrite(Actinic, 4095);
      delay(200);
      analogWrite(Actinic, 95);
      delay(200);
      analogWrite(Actinic, 4095);
      
      printDirectory();
      Serial.println("dump complete");
      delay(1000);
      AUTO = 0;

      analogWrite(Actinic, 95);
      delay(200);
      analogWrite(Actinic, 4095);
      delay(200);
      analogWrite(Actinic, 95);
      delay(200);
      analogWrite(Actinic, 4095);
   }
   if (AUTO == 11)
   { 
      analogWrite(Actinic, 95);
      delay(600);
      analogWrite(Actinic, 4095);
      delay(600);
      analogWrite(Actinic, 95);
      delay(600);
      analogWrite(Actinic, 4095);
      delay(600);
      analogWrite(Actinic, 95);
      delay(600);
      analogWrite(Actinic, 4095);
      delay(600);
      analogWrite(Actinic, 95);
      delay(600);
      analogWrite(Actinic, 4095);

      int DeletedCount = 0;
      int FolderDeleteCount = 0;
      int FailCount = 0;
      String rootpath = "/";
      dele();
      Serial.println("dump complete");
      delay(1000);
      AUTO = 0;

      analogWrite(Actinic, 95);
      delay(600);
      analogWrite(Actinic, 4095);
      delay(600);
      analogWrite(Actinic, 95);
      delay(600);
      analogWrite(Actinic, 4095);
      delay(600);
      analogWrite(Actinic, 95);
      delay(600);
      analogWrite(Actinic, 4095);
   }
}



void data()
{   //Serial.println(dataFile);
    for (int R = 0; R < REP; R++)
    {         
              //Channel 0
              i=0;
              while (i<initial)
              { 
                COL0PreS +=(uint16_t)adc->adc0->analogRead(SIG);
                COL0PreR += 1;
                i += 1;
              }
              
              i=0;
              while (i<sample0)
              { 
                digitalWriteFast(Col0, HIGH);
                TIMERSS(TOIMER1);
                COL0Sig[i] +=(uint16_t)adc->adc0->analogRead(SIG);
                digitalWriteFast(Col0, LOW);
                TIMERSS(TOIMER2);
                time[i]=micros();
                //COL0Ref[i] += 4095;
                i += 1;
              } 
              digitalWriteFast(Col0, LOW);
              timepersample0 += (time[sample0-1]-time[0]);  
              i=0;
              ann=annn;
              while (i<tau0)
              { 
                time[i]=micros();
                digitalWriteFast(Col0, HIGH);
                TIMERSS(TOIMER1);
                result[i]=(uint16_t)adc->adc0->analogRead(SIG);
                digitalWriteFast(Col0, LOW);
                ann = ann*tauexp;
                delayMicroseconds(ann);
                int resid = (uint16_t)adc->adc0->analogRead(SIG);
                COL0Tau[i] += (result[i]-resid);
                i += 1;
              } 
              timepertau0 += (time[tau0-1]-time[0]);
              delay(del);

              //Channel 1
              i=0;
              while (i<initial)
              { 
                COL1PreS +=(uint16_t)adc->adc0->analogRead(SIG);
                COL1PreR += 1;
                i += 1;
              }
              
              i=0;
              while (i<sample1)
              { 
                digitalWriteFast(Col1, HIGH);
                TIMERSS(TOIMER1);
                COL1Sig[i] +=(uint16_t)adc->adc0->analogRead(SIG);
                digitalWriteFast(Col1, LOW);
                TIMERSS(TOIMER2);
                time[i]=micros();
                //COL1Ref[i] += 4095;
                i += 1;
              } 
              digitalWriteFast(Col1, LOW);
              timepersample1 += (time[sample1-1]-time[0]); 
              i=0;
              ann=annn;
              while (i<tau1)
              { 
                time[i]=micros();
                digitalWriteFast(Col1, HIGH);
                TIMERSS(TOIMER1);
                result[i]=(uint16_t)adc->adc0->analogRead(SIG);
                digitalWriteFast(Col1, LOW);
                ann = ann*tauexp;
                delayMicroseconds(ann);
                int resid = (uint16_t)adc->adc0->analogRead(SIG);
                COL1Tau[i] += (result[i]-resid);
                i += 1;
              } 
              timepertau1 += (time[tau1-1]-time[0]);
              delay(del);

              //Channel 2
              i=0;
              while (i<initial)
              { 
                COL2PreS +=(uint16_t)adc->adc0->analogRead(SIG);
                COL2PreR += 1;
                i += 1;
              }
              
              i=0;
              while (i<sample2)
              { 
                digitalWriteFast(Col2, HIGH);
                TIMERSS(TOIMER1);
                COL2Sig[i] +=(uint16_t)adc->adc0->analogRead(SIG);
                digitalWriteFast(Col2, LOW);
                TIMERSS(TOIMER2);
                time[i]=micros();
                //COL2Ref[i] += 4095;
                i += 1;
              } 
              digitalWriteFast(Col2, LOW);
              timepersample2 += (time[sample2-1]-time[0]); 
              i=0;
              ann=annn;
              while (i<tau2)
              { 
                time[i]=micros();
                digitalWriteFast(Col2, HIGH);
                TIMERSS(TOIMER1);
                result[i]=(uint16_t)adc->adc0->analogRead(SIG);
                digitalWriteFast(Col2, LOW);
                ann = ann*tauexp;
                delayMicroseconds(ann);
                int resid = (uint16_t)adc->adc0->analogRead(SIG);
                COL2Tau[i] += (result[i]-resid);
                i += 1;
              } 
              timepertau2 += (time[tau2-1]-time[0]);
              delay(del);

               //Channel 3
              i=0;
              while (i<initial)
              { 
                COL3PreS +=(uint16_t)adc->adc0->analogRead(SIG);
                COL3PreR += 1;
                i += 1;
              }
              
              i=0;
              while (i<sample3)
              { 
                digitalWriteFast(Col3, HIGH);
                TIMERSS(TOIMER1);
                COL3Sig[i] +=(uint16_t)adc->adc0->analogRead(SIG);
                digitalWriteFast(Col3, LOW);
                TIMERSS(TOIMER2);
                time[i]=micros();
                //COL3Ref[i] += 4095;
                i += 1;
              } 
              digitalWriteFast(Col3, LOW);
              timepersample3 += (time[sample3-1]-time[0]); 
              i=0;
              ann=annn;
              while (i<tau3)
              { 
                time[i]=micros();
                digitalWriteFast(Col3, HIGH);
                TIMERSS(TOIMER1);
                result[i]=(uint16_t)adc->adc0->analogRead(SIG);
                digitalWriteFast(Col3, LOW);
                ann = ann*tauexp;
                delayMicroseconds(ann);
                int resid = (uint16_t)adc->adc0->analogRead(SIG);
                COL3Tau[i] += (result[i]-resid);
                i += 1;
              } 
              timepertau3 += (time[tau3-1]-time[0]);
              delay(del);
    }

    //Builds timing array based on Col0 excitation. 
    TIMEX0[i] = 0;
    TAUEX0[i] = 0;
    float perSample0 = (timepersample0/REP)/sample0;
    float perTau0 = (timepertau0/REP)/tau0;
    sampletimey0 = perSample0;
    sampletau0 = perTau0;
    //Serial.println(perSample);
    for (i=1;i<sample0;i++)
    {
           TIMEX0[i] = TIMEX0[i-1]+perSample0;
    }
    timepersample0 = 0;
    int Taustart0 = TIMEX0[sample0];
    for (i=1;i<tau0;i++)
    {
           TAUEX0[i] = Taustart0+i;
    }
    timepertau0 = 0;
    
    //Builds timing array based on Col1 excitation. 
    TIMEX1[i] = 0;
    TAUEX1[i] = 0;
    float perSample1 = (timepersample1/REP)/sample1;
    float perTau1 = (timepertau1/REP)/tau1;
    sampletimey1 = perSample1;
    sampletau1 = perTau1;
    //Serial.println(perSample);
    for (i=1;i<sample1;i++)
    {
           TIMEX1[i] = TIMEX1[i-1]+perSample1;
    }
    timepersample1 = 0;
    int Taustart1 = TIMEX1[sample1];
    for (i=1;i<tau1;i++)
    {
           TAUEX1[i] = Taustart1+i;
    }
    timepertau1 = 0;

    //Builds timing array based on Col2 excitation. 
    TIMEX2[i] = 0;
    TAUEX2[i] = 0;
    float perSample2 = (timepersample2/REP)/sample2;
    float perTau2 = (timepertau2/REP)/tau2;
    sampletimey2 = perSample2;
    sampletau2 = perTau2;
    //Serial.println(perSample);
    for (i=1;i<sample2;i++)
    {
           TIMEX2[i] = TIMEX2[i-1]+perSample2;
    }
    timepersample2 = 0;
    int Taustart2 = TIMEX2[sample2];
    for (i=1;i<tau2;i++)
    {
           TAUEX2[i] = Taustart2+i;
    }
    timepertau2 = 0;

    //Builds timing array based on Col3 excitation. 
    TIMEX3[i] = 0;
    TAUEX3[i] = 0;
    float perSample3 = (timepersample3/REP)/sample3;
    float perTau3 = (timepertau3/REP)/tau3;
    sampletimey3 = perSample3;
    sampletau3 = perTau3;
    //Serial.println(perSample);
    for (i=1;i<sample3;i++)
    {
           TIMEX3[i] = TIMEX3[i-1]+perSample3;
    }
    timepersample3 = 0;
    int Taustart3 = TIMEX3[sample3];
    for (i=1;i<tau3;i++)
    {
           TAUEX3[i] = Taustart3+i;
    }
    timepertau3 = 0;
    
    //INITIAL OFFSET REMOVES ambient stray light from Fo readings
    float SCOL0Pre = COL0PreS/initial;
    float RCOL0Pre = COL0PreR/initial;

    float SCOL1Pre = COL1PreS/initial;
    float RCOL1Pre = COL1PreR/initial;

    float SCOL2Pre = COL2PreS/initial;
    float RCOL2Pre = COL2PreR/initial;

    float SCOL3Pre = COL3PreS/initial;
    float RCOL3Pre = COL3PreR/initial;

    //RESULTS-AVERAGING


    for (i=0;i<sample0;i++)
    {
      COL0AveSig[i] = ((COL0Sig[i]-SCOL0Pre)/REP);
      //COL0AveRef[i] = ((COL0Ref[i]-RCOL0Pre)/REP);
      COL0Norm[i] = COL0AveSig[i]; //test
      COL0Sig[i] = 0;
      //COL0Ref[i] = 0;
      COL0PreS = 0;
      COL0PreR = 0;
    }
    for (i=0;i<tau0;i++)
    {
      COL0TauNorm[i] = COL0Tau[i]/REP; //test
      COL0Tau[i] = 0;
    }


    for (i=0;i<sample1;i++)
    {
      COL1AveSig[i] = ((COL1Sig[i]-SCOL1Pre)/REP);
      //COL1AveRef[i] = ((COL1Ref[i]-RCOL1Pre)/REP);
      COL1Norm[i] = COL1AveSig[i]; //test
      COL1Sig[i] = 0;
      //COL1Ref[i] = 0;
      COL1PreS = 0;
      COL1PreR = 0;
    }
    for (i=0;i<tau1;i++)
    {
      COL1TauNorm[i] = COL1Tau[i]/REP; //test
      COL1Tau[i] = 0;
    }

    for (i=0;i<sample2;i++)
    {
      COL2AveSig[i] = ((COL2Sig[i]-SCOL2Pre)/REP);
      //COL2AveRef[i] = ((COL2Ref[i]-RCOL2Pre)/REP);
      COL2Norm[i] = COL2AveSig[i]; //test
      COL2Sig[i] = 0;
      //COL2Ref[i] = 0;
      COL2PreS = 0;
      COL2PreR = 0;
    }
    for (i=0;i<tau2;i++)
    {
      COL2TauNorm[i] = COL2Tau[i]/REP; //test
      COL2Tau[i] = 0;
    }

    for (i=0;i<sample3;i++)
    {
      COL3AveSig[i] = ((COL3Sig[i]-SCOL3Pre)/REP);
      //COL3AveRef[i] = ((COL3Ref[i]-RCOL3Pre)/REP);
      COL3Norm[i] = COL3AveSig[i]; //test
      COL3Sig[i] = 0;
      //COL3Ref[i] = 0;
      COL3PreS = 0;
      COL3PreR = 0;
    }
    for (i=0;i<tau3;i++)
    {
      COL3TauNorm[i] = COL3Tau[i]/REP; //test
      COL3Tau[i] = 0;
    }
}

void curvefitter()
{
    COL0fit[i]= 0;
    COL0LINREG[i]= 0;
    LREGdataAnalysis(COL0Norm, COL0LINREG, COL0fit, TIMEX0, sample0, PARcol0);

    COL1fit[i]= 0;
    COL1LINREG[i]= 0;
    LREGdataAnalysis(COL1Norm, COL1LINREG, COL1fit, TIMEX1, sample1, PARcol1);

    COL2fit[i]= 0;
    COL2LINREG[i]= 0;
    LREGdataAnalysis(COL2Norm, COL2LINREG, COL2fit, TIMEX2, sample2, PARcol2);

    COL3fit[i]= 0;
    COL3LINREG[i]= 0;
    LREGdataAnalysis(COL3Norm, COL3LINREG, COL3fit, TIMEX3, sample3, PARcol3);
}

void TIMERSS(int TOIMER)
{
  for (int i=0; i<TOIMER;i++)
  {
    digitalWriteFast(timers, HIGH); 
    digitalWriteFast(timers, LOW); 
  }
}

void draw_m0_t(float Fmaxs, float Quant, String sampler)
{
  u8g2.clearBuffer();
  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_cu12_tr);  
  //u8g2.setFont(u8x8_font_courB18_2x3_f);
  
  u8g2.setCursor(0,15);
  u8g2.print("Fm:");
  u8g2.print(Fmaxs);
  u8g2.print(" Q:");
  u8g2.print(Quant);
  u8g2.setCursor(0,30);
  u8g2.print("Test: ");
  u8g2.print(sampler);
  
  u8g2.sendBuffer();
  delay(INFO_SCREEN_DELAY);
}


void LREGdataAnalysis(float*Fluor, float*LINREGG, float*curvefit1, float*TIMEX, int sample, int SIGer)
{   
      float sum_x=0.000;
      float sum_y=0.000;
      float sum_xy=0.000;
      float sum_xx=0.000;
      float sum_yy=0.000;
      int lineA = 1;
      int lineB = 6;
      int lineC = lineB-lineA;
      int MAXinit = sample-1;
      int maxAve = 0;
      int Residual =0;
      float FoX1 = 1.000;
      float FmX1 = 2.000;
      float SigX1 = 0.0200;
      float FmX2 = 0.2000;
      float SigX2 = 0.0020;
      float FmX3 = 0.0200;
      float SigX3 = 0.0002;
      float Fo = 0.0000;
      float Fm = 0.0000;
      float Sig = 0.0000;
      float LINREG[4]={0.000,0.000,0.000,0.000};
      float curvefit2[BEEFY];
      float minimumFo = 0.0000;
      float minimumFm = 0.0000;
      float minimumSig = 0.0000;
      float minimumRES = 10000000;      
    
      for (int i=lineA; i<lineB;i++)
      {
          sum_x = sum_x+TIMEX[i];
          sum_y = sum_y+Fluor[i];
          sum_xy = sum_xy+TIMEX[i]*Fluor[i];
          sum_xx = sum_xx+TIMEX[i]*TIMEX[i];
          sum_yy = sum_yy+Fluor[i]*Fluor[i];
      }
      
      LINREG[0]=(lineC*sum_xy-sum_x*sum_y)/(lineC*sum_xx-sum_x*sum_x);
      LINREG[1]=(sum_y/lineC)-((LINREG[0]*sum_x)/lineC);
      
      for (int i=MAXinit; i<sample;i++)
      {
           maxAve += Fluor[i];
      }
  
      LINREG[2]= maxAve/(sample-MAXinit);
      LINREG[3]= (LINREG[2]-LINREG[1])/LINREG[2]; 

      for (i=1;i<12;i++)
      {   
          Fm = ((LINREG[2]-(FmX1*5)) + (FmX1*i));
          for (t=1;t<20;t++)
          {   
              Sig =  ((0.1-(SigX1*5)) + (SigX1*t));
              for (g=1;g<sample+1;g=g+increment)
              {
                curvefit2[g] = LINREG[1] + (Fm-LINREG[1])*(1-exp(-(Sig*g)));
                Residual += (Fluor[g]-curvefit2[g])*(Fluor[g]-curvefit2[g]);
              }
              if (Residual < minimumRES) 
              {
                   minimumFo = LINREG[1];
                   minimumFm = Fm;
                   minimumSig = Sig;
                   minimumRES = Residual;
                   //Serial.print(minimumRES);
                   //Serial.print(' ');
              }
              Residual = 0;
         } 
      }

      for (i=1;i<12;i++)
      {   
          Fm = ((minimumFm-(FmX2*5)) + (FmX2*i));
          for (t=1;t<12;t++)
          {   
              Sig =  ((minimumSig-(SigX2*5)) + (SigX2*t));
              for (g=1;g<sample+1;g=g+increment)
              {
                curvefit2[g] = LINREG[1] + (Fm-LINREG[1])*(1-exp(-(Sig*g)));
                Residual += (Fluor[g]-curvefit2[g])*(Fluor[g]-curvefit2[g]);
              }
              if (Residual < minimumRES) 
              {
                   minimumFo = LINREG[1];
                   minimumFm = Fm;
                   minimumSig = Sig;
                   minimumRES = Residual;
                   //Serial.print(minimumRES);
                   //Serial.print(' ');
              }
              Residual = 0;
         } 
      }

      for (i=1;i<12;i++)
      {   
          Fm = ((minimumFm-(FmX3*5)) + (FmX3*i));
          for (t=1;t<12;t++)
          {   
              Sig =  ((minimumSig-(SigX3*5)) + (SigX3*t));
              for (g=1;g<sample+1;g=g+increment)
              {
                curvefit2[g] = LINREG[1] + (Fm-LINREG[1])*(1-exp(-(Sig*g)));
                Residual += (Fluor[g]-curvefit2[g])*(Fluor[g]-curvefit2[g]);
              }
              if (Residual < minimumRES) 
              {
                   minimumFo = LINREG[1];
                   minimumFm = Fm;
                   minimumSig = Sig;
                   minimumRES = Residual;
                   //Serial.print(minimumRES);
                   //Serial.print(' ');
              }
              Residual = 0;
         } 
      }

      for (i=1;i<22;i++)
      {   
          Fm = minimumFm;
          Sig =  minimumSig;
          Fo = ((minimumFo-(FoX1*10)) + (FoX1*i));
          for (g=1;g<sample+1;g=g+increment)
          {
            curvefit2[g] = Fo + (Fm-Fo)*(1-exp(-(Sig*g)));
            Residual += (Fluor[g]-curvefit2[g])*(Fluor[g]-curvefit2[g]);
          }
          if (Residual < minimumRES) 
          {
               minimumFo = Fo;
               minimumFm = Fm;
               minimumSig = Sig;
               minimumRES = Residual;
               //Serial.println(minimumRES);
          }
          Residual = 0;
          
      }
      
      for (g=1;g<sample+1;g++)
      {
        curvefit1[g] = minimumFo + (minimumFm-minimumFo)*(1-exp(-(minimumSig*g)));
      }
      LINREGG[0] = curvefit1[1];
      LINREGG[1] = curvefit1[sample];
      LINREGG[2] = minimumSig/SIGer*1000000;
      LINREGG[3] = ((curvefit1[sample]-curvefit1[1])/curvefit1[sample]);
      LINREGG[4] = minimumRES;
}


void CURVEdataPrint()
{     
      for (i=1;i<sample0;i++)
      {
            Serial.print(TIMEX0[i],4);
            Serial.print(' ');
            Serial.print(COL0Norm[i],4);
            Serial.print(' ');
            Serial.println("CURVES0");
      }
      for (i=1;i<tau0;i++)
      {
            Serial.print(TAUEX0[i],4);
            Serial.print(' ');
            Serial.print(COL0TauNorm[i],4);
            Serial.print(' ');
            Serial.println("CURVES0");
      }

      for (i=1;i<sample1;i++)
      {
            Serial.print(TIMEX1[i],4);
            Serial.print(' ');
            Serial.print(COL1Norm[i],4);
            Serial.print(' ');
            Serial.println("CURVES1");
      }
      for (i=1;i<tau1;i++)
      {
            Serial.print(TAUEX1[i],4);
            Serial.print(' ');
            Serial.print(COL1TauNorm[i],4);
            Serial.print(' ');
            Serial.println("CURVES1");
      }

      for (i=1;i<sample2;i++)
      {
            Serial.print(TIMEX2[i],4);
            Serial.print(' ');
            Serial.print(COL2Norm[i],4);
            Serial.print(' ');
            Serial.println("CURVES2");
      }
      for (i=1;i<tau2;i++)
      {
            Serial.print(TAUEX2[i],4);
            Serial.print(' ');
            Serial.print(COL2TauNorm[i],4);
            Serial.print(' ');
            Serial.println("CURVES2");
      }

      for (i=1;i<sample3;i++)
      {
            Serial.print(TIMEX3[i],4);
            Serial.print(' ');
            Serial.print(COL3Norm[i],4);
            Serial.print(' ');
            Serial.println("CURVES3");
      }
      for (i=1;i<tau3;i++)
      {
            Serial.print(TAUEX3[i],4);
            Serial.print(' ');
            Serial.print(COL3TauNorm[i],4);
            Serial.print(' ');
            Serial.println("CURVES3");
      }
      Serial.print("END");
      Serial.print(' ');
      Serial.print("END");
      Serial.print(' ');
      Serial.println("END");
      Serial.flush();
}


void PHOTOdataPrint()
{
        Serial.print(COL0LINREG[1],4);
        Serial.print(' ');
        Serial.print(COL1LINREG[1],4);
        Serial.print(' ');
        Serial.print(COL2LINREG[1],4);
        Serial.print(' ');
        Serial.print(COL3LINREG[1],4);
        Serial.print(' ');
        Serial.print(COL0LINREG[2],4);
        Serial.print(' ');
        Serial.print(COL1LINREG[2],4);
        Serial.print(' ');
        Serial.print(COL2LINREG[2],4);
        Serial.print(' ');
        Serial.print(COL3LINREG[2],4);
        Serial.print(' ');
        Serial.print(COL0LINREG[3],4);
        Serial.print(' ');
        Serial.print(COL1LINREG[3],4);
        Serial.print(' ');
        Serial.print(COL2LINREG[3],4);
        Serial.print(' ');
        Serial.print(COL3LINREG[3],4);
        Serial.print(' ');     
        Serial.print(now());
        Serial.print(' ');
        Serial.println("PHOTOCHEM");
        Serial.flush();
}

void dataSav()
{     
      if (SD.exists(curvesFile.c_str()))
      {   myFile = SD.open(curvesFile.c_str(), FILE_WRITE);
          myFile.print("Time");
          myFile.print(',');
          myFile.print(year());
          myFile.print(',');
          myFile.print(month());
          myFile.print(',');
          myFile.print(day());
          myFile.print(',');
          myFile.print(hour());
          myFile.print(',');
          myFile.print(minute());
          myFile.print(',');
          myFile.print(second());
          myFile.print(',');
          myFile.print(now());
          myFile.print(',');
          myFile.print(valpar);
          myFile.println();     
          //Col0
          myFile.print("Col0norm");
          for (i=1;i<sample0;i++)
          {
              myFile.print(',');
              myFile.print(COL0Norm[i],4);
          } 
          for (i=1;i<tau0;i++)
          {
            myFile.print(',');
            myFile.print(COL0TauNorm[i],4);
          }
          myFile.println();   
          //Col1
          myFile.print("Col1norm");
          for (i=1;i<sample1;i++)
          {
              myFile.print(',');
              myFile.print(COL1Norm[i],4);
          }
          for (i=1;i<tau1;i++)
          {
            myFile.print(',');
            myFile.print(COL1TauNorm[i],4);
          } 
          myFile.println();
          //Col2
          myFile.print("Col2norm");
          for (i=1;i<sample2;i++)
          {
              myFile.print(',');
              myFile.print(COL2Norm[i],4);
          }
          for (i=1;i<tau2;i++)
          {
            myFile.print(',');
            myFile.print(COL2TauNorm[i],4);
          } 
          myFile.println();
          //Col3
          myFile.print("Col3norm");
          for (i=1;i<sample3;i++)
          {
              myFile.print(',');
              myFile.print(COL3Norm[i],4);
          } 
          for (i=1;i<tau3;i++)
          {
            myFile.print(',');
            myFile.print(COL3TauNorm[i],4);
          }
          myFile.println();
          myFile.close();
      }
      else 
      {   File myFile = SD.open(curvesFile.c_str(), FILE_WRITE);
          myFile.print("Time");
          myFile.print(',');
          myFile.print(year());
          myFile.print(',');
          myFile.print(month());
          myFile.print(',');
          myFile.print(day());
          myFile.print(',');
          myFile.print(hour());
          myFile.print(',');
          myFile.print(minute());
          myFile.print(',');
          myFile.print(second());
          myFile.print(',');
          myFile.print(now());
          myFile.print(',');
          myFile.print(valpar);
          myFile.println();     
          //Col0
          myFile.print("Col0norm");
          for (i=1;i<sample0;i++)
          {
              myFile.print(',');
              myFile.print(COL0Norm[i],4);
          }
          for (i=1;i<tau0;i++)
          {
            myFile.print(',');
            myFile.print(COL0TauNorm[i],4);
          } 
          myFile.println();   
          //Col1
          myFile.print("Col1norm");
          for (i=1;i<sample1;i++)
          {
              myFile.print(',');
              myFile.print(COL1Norm[i],4);
          }
          for (i=1;i<tau1;i++)
          {
            myFile.print(',');
            myFile.print(COL1TauNorm[i],4);
          } 
          myFile.println();
          //Col2
          myFile.print("Col2norm");
          for (i=1;i<sample2;i++)
          {
              myFile.print(',');
              myFile.print(COL2Norm[i],4);
          }
          for (i=1;i<tau2;i++)
          {
            myFile.print(',');
            myFile.print(COL2TauNorm[i],4);
          } 
          myFile.println();
          //Col3
          myFile.print("Col3norm");
          for (i=1;i<sample3;i++)
          {
              myFile.print(',');
              myFile.print(COL3Norm[i],4);
          }
          for (i=1;i<tau3;i++)
          {
            myFile.print(',');
            myFile.print(COL3TauNorm[i],4);
          } 
          myFile.println();
          myFile.close();
      }

      if (SD.exists(dataFile.c_str()))
      {   myFile2 = SD.open(dataFile.c_str(), FILE_WRITE);
          myFile2.print("Time");
          myFile2.print(',');
          myFile2.print(year());
          myFile2.print(',');
          myFile2.print(month());
          myFile2.print(',');
          myFile2.print(day());
          myFile2.print(',');
          myFile2.print(hour());
          myFile2.print(',');
          myFile2.print(minute());
          myFile2.print(',');
          myFile2.print(second());
          myFile2.print(',');
          myFile2.print(now());
          myFile2.print(',');
          myFile2.print(valpar);
          myFile2.print(',');
          myFile2.print(COL0LINREG[0],4);
          myFile2.print(',');
          myFile2.print(COL0LINREG[1],4);
          myFile2.print(',');
          myFile2.print(COL0LINREG[2],4);
          myFile2.print(',');
          myFile2.print(COL0LINREG[3],4);
          myFile2.print(',');
          myFile2.print(COL0LINREG[4],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[0],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[1],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[2],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[3],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[4],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[0],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[1],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[2],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[3],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[4],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[0],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[1],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[2],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[3],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[4],4);
          myFile2.println();   
          myFile2.close();
      }
      else 
      {   File myFile2 = SD.open(dataFile.c_str(), FILE_WRITE);
          myFile2.print("Time");
          myFile2.print(',');
          myFile2.print("year");
          myFile2.print(',');
          myFile2.print("month");
          myFile2.print(',');
          myFile2.print("day");
          myFile2.print(',');
          myFile2.print("hour");
          myFile2.print(',');
          myFile2.print("minute");
          myFile2.print(',');
          myFile2.print("second");
          myFile2.print(',');
          myFile2.print("compTime");
          myFile2.print(',');
          myFile2.print("PAR");
          myFile2.print(',');
          myFile2.print("COL0-Fo");
          myFile2.print(',');
          myFile2.print("COL0-Fm");
          myFile2.print(',');
          myFile2.print("COL0-Sig");
          myFile2.print(',');
          myFile2.print("COL0-fvfm");
          myFile2.print(',');
          myFile2.print("COL0-resid");
          myFile2.print(',');
          myFile2.print("COL1-Fo");
          myFile2.print(',');
          myFile2.print("COL1-Fm");
          myFile2.print(',');
          myFile2.print("COL1-Sig");
          myFile2.print(',');
          myFile2.print("COL1-fvfm");
          myFile2.print(',');
          myFile2.print("COL1-resid");
          myFile2.print(',');
          myFile2.print("COL2-Fo");
          myFile2.print(',');
          myFile2.print("COL2-Fm");
          myFile2.print(',');
          myFile2.print("COL2-Sig");
          myFile2.print(',');
          myFile2.print("COL2-fvfm");
          myFile2.print(',');
          myFile2.print("COL2-resid");
          myFile2.print(',');
          myFile2.print("COL3-Fo");
          myFile2.print(',');
          myFile2.print("COL3-Fm");
          myFile2.print(',');
          myFile2.print("COL3-Sig");
          myFile2.print(',');
          myFile2.print("COL3-fvfm");
          myFile2.print(',');
          myFile2.print("COL3-resid");
          myFile2.println(); 
      
          myFile2.print("Time");
          myFile2.print(',');
          myFile2.print(year());
          myFile2.print(',');
          myFile2.print(month());
          myFile2.print(',');
          myFile2.print(day());
          myFile2.print(',');
          myFile2.print(hour());
          myFile2.print(',');
          myFile2.print(minute());
          myFile2.print(',');
          myFile2.print(second());
          myFile2.print(',');
          myFile2.print(now());
          myFile2.print(',');
          myFile2.print(valpar);
          myFile2.print(',');
          myFile2.print(COL0LINREG[0],4);
          myFile2.print(',');
          myFile2.print(COL0LINREG[1],4);
          myFile2.print(',');
          myFile2.print(COL0LINREG[2],4);
          myFile2.print(',');
          myFile2.print(COL0LINREG[3],4);
          myFile2.print(',');
          myFile2.print(COL0LINREG[4],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[0],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[1],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[2],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[3],4);
          myFile2.print(',');
          myFile2.print(COL1LINREG[4],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[0],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[1],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[2],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[3],4);
          myFile2.print(',');
          myFile2.print(COL2LINREG[4],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[0],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[1],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[2],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[3],4);
          myFile2.print(',');
          myFile2.print(COL3LINREG[4],4);
          myFile2.println();   
          myFile2.close();
      }
}

void SlaveReader()
{   
      //Serial4.print(PSIFile);
      //Serial4.print(',');
      //Serial4.print('>');
}



void serialInput()
{       
     while (Serial.available() > 0)
     {    String dats = Serial.readStringUntil('STOP');
          int first = dats.indexOf(',');
          String firsty = dats.substring(0,first);
          int seconds = dats.indexOf(',', first+1);
          String secondy = dats.substring(first+1,seconds);
          int thirds = dats.indexOf(',', seconds+1);
          String thirdy = dats.substring(seconds+1,thirds);
          int fourths = dats.indexOf(',', thirds+1);
          String fourthy = dats.substring(thirds+1,fourths);
          int fifths = dats.indexOf(',', fourths+1);
          String fifthy = dats.substring(fourths+1,fifths);
          int six = dats.indexOf(',', fifths+1);
          String sixth = dats.substring(fifths+1,six);
          int seven = dats.indexOf(',', six+1);
          String seventh = dats.substring(six+1,seven);
          int eight = dats.indexOf(',', seven+1);
          String eighth = dats.substring(seven+1,eight);
          int nine = dats.indexOf(',', eight+1);
          String ninth = dats.substring(eight+1,nine);
          
          DELAYED = firsty.toFloat()*1000;
          REP = secondy.toFloat();
          del = thirdy.toFloat();
          AUTO = fifthy.toFloat();
          sample0 = sixth.toFloat()/sampletimey0;
          sample1 = seventh.toFloat()/sampletimey1;
          sample2 = eighth.toFloat()/sampletimey2;
          sample3 = ninth.toFloat()/sampletimey3;
          String namer1 = fourthy + "r.csv";
          String namer2 = fourthy + "d.csv";
          String namer3 = fourthy + "";
          curvesFile.replace(curvesFile, namer1);
          dataFile.replace(dataFile, namer2);
          fourn.replace(fourn, namer3);
     }
}


void timeSave()
{
     //Initialize the SD card
     Serial.println("Initializing SD card...");
     if (!SD.begin(chipSelect)) 
     {    Serial.println("initialization fucked!");
     }
     else
     {    
          Serial.println("initialization done."); 
     }

     //Synchronize Time
     setSyncProvider(getTeensy3Time);
     if (!Serial);
     if (timeStatus() != timeSet) 
     {    Serial.println("Unable to sync with the RTC");
     }
     else
     {    Serial.println("RTC has set the system time");   
     }  
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}


void printDirectory() 
{  root = SD.open("/");
   while(true) 
   {
     
     File entry =  root.openNextFile();
     if (! entry) 
     {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<0; i++) 
     {
       Serial.print('\t');
     }
     
     if (entry.isDirectory()) 
     {
       //Serial.println("/");
       //printDirectory(entry, numTabs+1);
     } 
     else 
     { Serial.println('\t');
       Serial.print(entry.name());
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
       dump(entry.name());
       Serial.println("\t");
       delay(1000);
     }
     entry.close();
   }
}



void dump(String nams)
{
  // open the file.
  File dataFile = SD.open(nams.c_str());

  // if the file is available, write to it:
  if (dataFile) 
  {
    while (dataFile.available()) 
    {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }  
  // if the file isn't open, pop up an error:
  else 
  {
    Serial.println("error opening datalog.txt");
  } 
}


void FileNumb() 
{  root = SD.open("/");
   while(true) 
   {
     
     File entry =  root.openNextFile();
     if (! entry) 
     {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<0; i++) 
     {
       Serial.print('\t');
     }
     fileCountOnSD++;
     if (entry.isDirectory()) 
     {
       //Serial.println("/");
       //printDirectory(entry, numTabs+1);
     } 
     else 
     {  
     }
     entry.close();
   }
}


void dele()
{ SD.begin(chipSelect);

  root = SD.open("/");
  delay(2000);

  rm(root, rootpath);

  if( !DeletedCount && !FailCount && !FolderDeleteCount )
  {

  } 
  else
  {
    //Serial.print("Deleted ");
    //Serial.print(DeletedCount);
    //Serial.print(" file");
    if( DeletedCount != 1 )
    {
      //Serial.print("s");
    }
    //Serial.print(" and ");
    //Serial.print(FolderDeleteCount);
    //Serial.print(" folder");
    if ( FolderDeleteCount != 1 )
    {
      //Serial.print("s");
    }
    //Serial.println(" from SD card.");
    if( FailCount > 0 )
    {
      //Serial.print("Failed to delete ");
      //Serial.print(FailCount);
      //Serial.print(" item");
      if( FailCount != 1 )
      {
        //Serial.print("s");
      }
    }
  }
}


void rm(File dir, String tempPath) {
  while(true) {
    File entry =  dir.openNextFile();
    String localPath;

    //Serial.println("");
    if (entry) {
      if ( entry.isDirectory() )
      {
        localPath = tempPath + entry.name() + rootpath + '\0';
        char folderBuf[localPath.length()];
        localPath.toCharArray(folderBuf, localPath.length() );
        rm(entry, folderBuf);


        if( SD.rmdir( folderBuf ) )
        {
          //Serial.print("Deleted folder ");
          //Serial.println(folderBuf);
          FolderDeleteCount++;
        } 
        else
        {
          //Serial.print("Unable to delete folder ");
          //Serial.println(folderBuf);
          FailCount++;
        }
      } 
      else
      {
        localPath = tempPath + entry.name() + '\0';
        char charBuf[localPath.length()];
        localPath.toCharArray(charBuf, localPath.length() );

        if( SD.remove( charBuf ) )
        {
          //Serial.print("Deleted ");
          //Serial.println(localPath);
          DeletedCount++;
        } 
        else
        {
          //Serial.print("Failed to delete ");
          //Serial.println(localPath);
          FailCount++;
        }

      }
    } 
    else {
      // break out of recursion
      break;
    }
  }
}



void adc0_isr(void) 
{
        adc->adc0->analogReadContinuous();
}
#ifdef ADC_DUAL_ADCS
void adc1_isr(void) 
{
        adc->adc1->analogReadContinuous();
}
#endif
