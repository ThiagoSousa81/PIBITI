// Elaborado por Thiago Sousa

// https://wokwi.com/projects/416316369662326785

// github.com/thiagosousa81
// Sensores MPU-6050

// Inclusão das Bibliotecas
#include <Wire.h>

// Endereços I2C dos sensores MPU-6050
const int MPU1 = 0x68; // Sensor no robô
const int MPU2 = 0x69; // Sensor na carroça

// Constantes do filtro complementar
const float alpha = 0.98; // Peso do giroscópio (filtro complementar)
const float dt = 0.01;    // Intervalo de tempo (10ms)

// Variáveis para armazenar valores de ambos os sensores
float AccX1, AccY1, AccZ1, GyrX1, GyrY1, GyrZ1;
float AccX2, AccY2, AccZ2, GyrX2, GyrY2, GyrZ2;
float angleX1, angleY1, angleZ1, angleX2, angleY2, angleZ2;

void setup() {
  Serial.begin(9600);

  // Inicializa I2C e configura os sensores
  Wire.begin();

  Wire.beginTransmission(MPU1);
  Wire.write(0x6B); // Inicializa o sensor
  Wire.write(0);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU2);
  Wire.write(0x6B); // Inicializa o sensor
  Wire.write(0);
  Wire.endTransmission(true);

  pinMode(2, INPUT);  // Define o pino de interrupção (INT) no Arduino
  attachInterrupt(digitalPinToInterrupt(2), onInterrupt1, FALLING); // Configura a interrupção

  pinMode(3, INPUT);  // Define o pino de interrupção (INT) no Arduino
  attachInterrupt(digitalPinToInterrupt(2), onInterrupt2, FALLING); // Configura a interrupção

  pinMode(6, INPUT);
  pinMode(7, INPUT);

}

// Aqui seriam ações de interrupção
void onInterrupt1() {
  readMPU(MPU1, AccX1, AccY1, AccZ1, GyrX1, GyrY1, GyrZ1);
}
void onInterrupt2() {
  readMPU(MPU2, AccX2, AccY2, AccZ2, GyrX2, GyrY2, GyrZ2);
}

void loop() {
  // Obter dados do MPU1
  readMPU(MPU1, AccX1, AccY1, AccZ1, GyrX1, GyrY1, GyrZ1);
  // Obter dados do MPU2
  readMPU(MPU2, AccX2, AccY2, AccZ2, GyrX2, GyrY2, GyrZ2);

  // atan2() é o Arco-Tangente em Radianos entre X e Y
  // Calcula os ângulos do MPU1 usando filtro complementar
  float accelAngleX1 = atan2(AccY1, sqrt(AccX1 * AccX1 + AccZ1 * AccZ1)) * 180 / PI;
  float accelAngleY1 = atan2(-AccX1, sqrt(AccY1 * AccY1 + AccZ1 * AccZ1)) * 180 / PI;
  angleX1 = alpha * (angleX1 + GyrX1 * dt) + (1 - alpha) * accelAngleX1;
  angleY1 = alpha * (angleY1 + GyrY1 * dt) + (1 - alpha) * accelAngleY1;

  // Calcula os ângulos do MPU2 usando filtro complementar
  float accelAngleX2 = atan2(AccY2, sqrt(AccX2 * AccX2 + AccZ2 * AccZ2)) * 180 / PI;
  float accelAngleY2 = atan2(-AccX2, sqrt(AccY2 * AccY2 + AccZ2 * AccZ2)) * 180 / PI;
  angleX2 = alpha * (angleX2 + GyrX2 * dt) + (1 - alpha) * accelAngleX2;
  angleY2 = alpha * (angleY2 + GyrY2 * dt) + (1 - alpha) * accelAngleY2;

  // Calcula os ângulos relativos entre o robô e a carroça
  float deltaX = angleX2 - angleX1;
  float deltaY = angleY2 - angleY1;

  // Cálculo do ângulo Z (torção)
  // O ângulo Z pode ser calculado utilizando a função atan2 com os componentes X e Y
  // A diferença entre os dois sensores nos dá o Delta Z
  float accelAngleZ1 = atan2(AccZ1, sqrt(AccX1 * AccX1 + AccY1 * AccY1)) * 180 / PI;
  float accelAngleZ2 = atan2(AccZ2, sqrt(AccX2 * AccX2 + AccY2 * AccY2)) * 180 / PI;

  // Calcular o Delta Z (diferença do ângulo de torção entre o robô e a carroça)
  float deltaZ = accelAngleZ2 - accelAngleZ1;

  if (digitalRead(6) == HIGH){
    Serial.println("Dados do sensor do robô:");
    printSensorData(AccX1, AccY1, AccZ1, GyrX1, GyrY1, GyrZ1, angleX1, angleY1);
  }
  else if (digitalRead(7) == HIGH){
    Serial.println("Dados do sensor da carroça:");
    printSensorData(AccX2, AccY2, AccZ2, GyrX2, GyrY2, GyrZ2, angleX2, angleY2);
  }
  else if (digitalRead(6) == LOW && digitalRead(7) == LOW) {
    // Exibe os ângulos e alertas
    Serial.print("Delta X (Lateral): ");
    Serial.println(deltaX);
    if (deltaX < -90 || deltaX > 90) {
      Serial.println("ALERTA: Ângulo lateral fora do limite!");
    }

    Serial.print("Delta Y (Frontal): ");
    Serial.println(deltaY);
    if (deltaY < -20 || deltaY > 20) {
      Serial.println("ALERTA: Ângulo frontal fora do limite!");
    }

    // O ângulo da torção não está pronto
    // Será necessário talvez fazer uma subtração do atrito do engate para ficar mais realista
    Serial.print("Delta Z (Torção): ");
    Serial.println(deltaZ);
    if (deltaZ < -5 || deltaZ > 5) {
      Serial.println("ALERTA: Ângulo de torção fora do limite!");
    }

    Serial.println("========================");
  }
  
  delay(600);
}

// Função para imprimir os dados do sensor
void printSensorData(float AccX, float AccY, float AccZ, float GyrX, float GyrY, float GyrZ, float angleX, float angleY) {
  Serial.print("Acelerômetro X: ");
  Serial.println(AccX);
  Serial.print("Acelerômetro Y: ");
  Serial.println(AccY);
  Serial.print("Acelerômetro Z: ");
  Serial.println(AccZ);
  Serial.print("Giroscópio X: ");
  Serial.println(GyrX);
  Serial.print("Giroscópio Y: ");
  Serial.println(GyrY);
  Serial.print("Giroscópio Z: ");
  Serial.println(GyrZ);
  Serial.print("Ângulo X: ");
  Serial.println(angleX);
  Serial.print("Ângulo Y: ");
  Serial.println(angleY);
}

void readMPU(int address, float &AccX, float &AccY, float &AccZ, float &GyrX, float &GyrY, float &GyrZ) {
  Wire.beginTransmission(address);
  Wire.write(0x3B); // Registro inicial de leitura
  Wire.endTransmission(false);
  Wire.requestFrom(address, 14, true);

  // A operação Left-Shift (<<) desloca 2 hexadecimais pra esquerda e acrescenta mais 2 zeros
  // O que o procedimento está fazendo é juntando os 16 bytes da leitura de cada dimensão 
  // através de uma concatenação direta em Bytes e dividindo pelo fator de sensibilidade

  float sens_acc = 16384.0; // 19.6m/s² 
  float sens_gir = 131.0; // 250°/s

  // Leitura do acelerômetro
  AccX = (Wire.read() << 8 | Wire.read()) / sens_acc;
  AccY = (Wire.read() << 8 | Wire.read()) / sens_acc;
  AccZ = (Wire.read() << 8 | Wire.read()) / sens_acc;

  // Leitura do giroscópio
  Wire.read(); Wire.read(); // Ignora temperatura
  GyrX = (Wire.read() << 8 | Wire.read()) / sens_gir;
  GyrY = (Wire.read() << 8 | Wire.read()) / sens_gir;
  GyrZ = (Wire.read() << 8 | Wire.read()) / sens_gir;
}
