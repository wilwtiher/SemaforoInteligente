# Aluno: Wilton Lacerda Silva Júnior
## Matrícula: TIC370100193
# Video explicativo: https://youtu.be/JoGXh48Usw4
# Semáforo Inteligente
O objetivo do projeto é desenvolver utilizando as tarefas do sistema operacional FreeRTOS para o Raspberry PI PICO W um esquema de Semáforo.
## Funcionalidades

- **Display OLED**
  - O display mostrará o em qual estado o semáforo está.
- **2 Botões**
  - O botão B está sendo utilizado com a função de reiniciar a placa no modo de bootload.
  - O botão A está sendo utilizado com a função de colocar o programa no modo noturno, mudando a lógica do semáforo.
- **MATRIZ DE LEDs**
   - A matriz de LEDs simulará o semáforo.
- **BUZZER**
  - O buzzer servirá como identificação sonora para o semáforo.
- **LED RGB**
   - O LED RGB do meio da placa também servirá para mostrar as cores do semáforo.

# Requisitos
## Hardware:

- Raspberry Pi Pico W.
- 1 display ssd1306 com o sda na porta 14 e o scl na porta 15.
- 1 matriz de led 5x5 na porta 7
- 1 led rgb, com o led vermelho no pino 13 e o led verde no pino 11
- 2 botões, um no pino 5 e outro no pino 6
- 1 buzzer no pino 10

## Software:

- Ambiente de desenvolvimento VS Code com extensão Pico SDK.

# Instruções de uso
## Configure o ambiente:
- Certifique-se de que o Pico SDK está instalado e configurado no VS Code.
- Configure o FreeRTOS para a configuração da sua máquina.
- Compile o código utilizando a extensão do Pico SDK.
## Teste:
- Utilize a placa BitDogLab para o teste. Caso não tenha, conecte os hardwares informados acima nos pinos correspondentes.

# Explicação do projeto:
## Contém:
- O projeto terá 1 meio de entrada: O botão.
- Também contará com saídas visuais, sendo a matriz de led, o led rgb no meio da placa, o buzzer, e o display OLED.

## Funcionalidades:
- O programa mostrará uma representação de um semáforo na matriz de led.
- O programa mostrará a cor acesa do semáforo no led central.
- O buzzer representará sonoramente o estado do semáforo.
- O display OLED mostrará em qual estádo está o semáforo
- O botão alterará o estado do semáforo para modo norturno.
