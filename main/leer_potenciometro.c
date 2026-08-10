#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"

void app_main(void)
{
    // 1. Inicializar la unidad ADC1
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config, &adc1_handle);

    // 2. Configurar el canal 3 (GPIO 39) con atenuación para medir hasta 3.3V
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, // 12 bits (0 - 4095)
        .atten = ADC_ATTEN_DB_12,         // Atenuación para rango completo (0 - 3.3V)
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_3, &config);

    int adc_raw = 0;

    while (1) {
        // Leer valor del ADC (0 a 4095)
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_3, &adc_raw);

        // Conversión matemática simple
        float porcentaje = ((float)adc_raw / 4095.0f) * 100.0f;
        float voltaje = ((float)adc_raw / 4095.0f) * 3.3f;

        // Mostrar en consola
        printf("ADC Raw: %4d | Voltaje: %.2f V | Posicion: %.1f%%\n", adc_raw, voltaje, porcentaje);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}