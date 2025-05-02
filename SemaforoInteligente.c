#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include <stdio.h>
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "hardware/pwm.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Matriz de LEDs
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
uint8_t led_r = 5; // Intensidade do vermelho
uint8_t led_g = 5; // Intensidade do verde
uint8_t led_b = 5; // Intensidade do azul

#define buzzer 10    // Pino do buzzer A
#define led_RED 13   // Red=13, Blue=12, Green=11
#define led_GREEN 11 // Red=13, Blue=12, Green=11
#define botao_pinA 5 // Botão A = 5, Botão B = 6 , BotãoJoy = 22

// Variaveis globais
bool Noturno = false;

void vSemaforoTask()
{
    gpio_init(led_RED);
    gpio_set_dir(led_RED, GPIO_OUT);
    gpio_init(led_GREEN);
    gpio_set_dir(led_GREEN, GPIO_OUT);
    gpio_put(led_RED, false);
    gpio_put(led_GREEN, false);

    gpio_set_function(buzzer, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(buzzer);
    pwm_set_wrap(slice_num, 4096);
    // Define o clock divider como 440
    pwm_set_clkdiv(slice_num, 440.0f);
    pwm_set_enabled(slice_num, true);

    while (true)
    {
        gpio_put(led_GREEN, true);
        pwm_set_gpio_level(buzzer, 2048);
        vTaskDelay(pdMS_TO_TICKS(150));
        pwm_set_gpio_level(buzzer, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_put(led_RED, true);
        for(int i = 0; i < 4; i++){
            pwm_set_gpio_level(buzzer, 2048);
            vTaskDelay(pdMS_TO_TICKS(150));
            pwm_set_gpio_level(buzzer, 0);
            vTaskDelay(pdMS_TO_TICKS(200));
        }
        gpio_put(led_GREEN, false);
        pwm_set_gpio_level(buzzer, 2048);
        vTaskDelay(pdMS_TO_TICKS(500));
        pwm_set_gpio_level(buzzer, 0);
        vTaskDelay(pdMS_TO_TICKS(1500));
        gpio_put(led_RED, false);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vDisplay3Task()
{
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
    ssd1306_t ssd;                                                // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    char str_y[5]; // Buffer para armazenar a string
    int contador = 0;
    bool cor = true;
    while (true)
    {
        sprintf(str_y, "%d", contador); // Converte em string
        contador++;                     // Incrementa o contador
        ssd1306_fill(&ssd, !cor);                          // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);      // Desenha um retângulo
        ssd1306_line(&ssd, 3, 25, 123, 25, cor);           // Desenha uma linha
        ssd1306_line(&ssd, 3, 37, 123, 37, cor);           // Desenha uma linha
        ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 6); // Desenha uma string
        ssd1306_draw_string(&ssd, "EMBARCATECH", 20, 16);  // Desenha uma string
        ssd1306_draw_string(&ssd, "  FreeRTOS", 10, 28); // Desenha uma string
        ssd1306_draw_string(&ssd, "Contador  LEDs", 10, 41);    // Desenha uma string
        ssd1306_draw_string(&ssd, str_y, 40, 52);          // Desenha uma string
        ssd1306_send_data(&ssd);                           // Atualiza o display
        sleep_ms(735);
    }
}

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}

int main()
{
    // Para ser utilizado o modo BOOTSEL com botão B
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    // Fim do trecho para modo BOOTSEL com botão B

    stdio_init_all();

    xTaskCreate(vSemaforoTask, "Semaforo Task", configMINIMAL_STACK_SIZE,
         NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vDisplay3Task, "Cont Task Disp3", configMINIMAL_STACK_SIZE, 
        NULL, tskIDLE_PRIORITY, NULL);
    vTaskStartScheduler();
    panic_unsupported();
}
