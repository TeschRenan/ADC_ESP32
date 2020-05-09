
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <esp_log.h>

 
esp_adc_cal_characteristics_t adc_cal;//Estrutura que contem as informacoes para calibracao

//Codigo fonte extraido da Espressif IDF para medição do ADC1_CHANNEL_5 = GPIO 33, com atenuação de 0dB, faixa de leitura de 0 a 1.1V
 
void setup() {

  Serial.begin(115200);

 
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);
    
    esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, 1100, &adc_cal);//Inicializa a estrutura de calibracao
 
    if (adc_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        Serial.println("ADC CALV ref eFuse encontrado: ");
        Serial.print(adc_cal.vref);
        Serial.print("mV");
    }
    else if (adc_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        Serial.println("ADC CAL Two Point eFuse encontrado");
    }
    else
    {
        Serial.println("ADC CAL Nada encontrado, utilizando Vref padrao: ");
        Serial.print(adc_cal.vref);
        Serial.print("mV");
    }
    
}

void loop() {
  uint32_t AD = 0;
        for (int i = 0; i < 100; i++)
        {
            AD += adc1_get_raw(ADC1_CHANNEL_5);//Obtem o valor RAW do ADC
            ets_delay_us(30);
        }
        AD /= 100;
 
        Serial.print("Valor do AD: ");
        Serial.print(AD);
        Serial.println("");
        AD = esp_adc_cal_raw_to_voltage(AD, &adc_cal);//Converte e calibra o valor lido (RAW) para mV

        Serial.print("Valor do AD em mV: ");
        Serial.print(AD);
        Serial.println("mV");
        Serial.println("");
        delay(1000);

}
