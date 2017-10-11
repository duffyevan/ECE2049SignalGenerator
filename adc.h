#ifndef ADC_H
#define ADC_H
__interrupt void ADC12ISR (void);
void setupADC();
void waitForADC();
void startADCConversion();
int getMeasurement();
void enableConversionInterrupt();
float adcReadingToTemperatureInC(unsigned int reading);
void fillMeasurementsArrayWithInvalidValues();
void getMeasurements();

#endif
