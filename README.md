# Sistema de Biofeedback Postural

Protótipo desenvolvido como parte de um Trabalho de Conclusão de Curso em Engenharia da Computação.

O projeto utiliza um ESP32 e um sensor MPU6050 para identificar a inclinação do tronco durante o agachamento. Quando a inclinação relativa ultrapassa o limite definido, o sistema emite um alerta sonoro.

## Status atual



- Simulação validada na plataforma Wokwi;
- Firmware compilado para ESP32;
- Leitura de inclinação com MPU6050;
- Calibração inicial da postura neutra;
- Alerta sonoro para inclinação acima de 30 graus;
- Montagem física prevista como próxima etapa.

## Simulação no Wokwi

O circuito e os testes iniciais podem ser acessados na plataforma Wokwi:

[Visualizar simulação do sistema de biofeedback postural](https://wokwi.com/projects/476704888770307073)

## Componentes previstos

- ESP32 DevKit V1;
- MPU6050;
- Buzzer;
- Protoboard e jumpers;
- Cinta elástica para fixação do sensor.

## Ligações previstas

| Componente | ESP32 |
|---|---|
| MPU6050 SDA | GPIO 21 |
| MPU6050 SCL | GPIO 22 |
| MPU6050 VCC | 3V3 |
| MPU6050 GND | GND |
| Buzzer | GPIO 25 e GND |

## Estrutura do projeto

- `src/main.cpp`: firmware principal;
- `platformio.ini`: configuração da placa ESP32 e do framework Arduino;
- `.gitignore`: arquivos que não devem ser enviados ao GitHub.

## Autoria

João Vitor Bettiol  
Engenharia da Computação — PUC Goiás