## Modelo 01

> Esse modelo **não** poderá ser realizado com Arduino UNO pois a faixa de limite de tensão do MPU6050 é de 2.3V a 3.6V
> <br> O microcontrolador ideal é o ESP32 ou o Raspberry Pi Pico W

## Descrição

Esse modelo de medidor de ângulo eletrônico usa o sensor **MPU6050** em dois locais diferentes

O sensor utilizado é um acelerômetro com giroscópio e medidor de temperatura. 

> O MPU6050 precisa ser alimentado com 3.3V no máximo

## Funcionamento

Os botões disponíveis na placa servem para visualizar as informações pertinentes dos dois sensores, tanto o que fica na carroça, quando o que fica no robô.

## Lista de materiais

- 2 sensores MPU6050 (necessário)
- 1 Placa de desenvolvimento **ESP32** ou algum da família Raspberry Pi Pico, com **RP2040** (recomendo Raspberry Pi Pico ou Raspberry Pi Pico W)
- 1 Protoboard 400 pontos no mínimo
- 1 Display LCD com módulo I2C com backlight azul
- 1 Buzzer Ativo (dá pra colocar passivo mas não é necessário)
- Kit Jumpers 20cm 60 Peças MxF MxM FxF (20 unidades de cada)

