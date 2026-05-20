需要[[开启ADC]]
```
/*校准ADC*/
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
```

# HAL库

```
HAL_ADCEx_Calibration_Start(&hadc1);
```