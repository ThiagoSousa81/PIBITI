// Constante que define o giro máximo do potenciômetro
const int Pang = 260;
void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);    
  pinMode(A2, INPUT);
}

void loop() {
  // Lendo os valores dos potenciômetros com o ADC
  int x = analogRead(A0);
  int y = analogRead(A1);
  int z = analogRead(A2);
  
  // Definindo o ângulo em graus
  int angulox = (int)(Pang*(float)x/1023);
  int anguloy = (int)(Pang*(float)y/1023);
  int anguloz = (int)(Pang*(float)z/1023);

  // Mostrando os ângulos na tela
  Serial.print("Angulo X: ");
  Serial.println(angulox);
  Serial.print("Angulo Y: ");
  Serial.println(anguloy);
  Serial.print("Angulo Z: ");
  Serial.println(anguloz);

  delay(100);
}
