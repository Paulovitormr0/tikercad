// ===== AUTOMAÇÃO DE CASA COM SENSOR DE MOVIMENTO =====
// Sistema de controle de iluminação automática baseado em movimento
// Componentes: Arduino, Sensor PIR, LEDs, Resistores

// ===== DEFINIÇÃO DE PINOS =====
const int pinSensorMovimento = 2;    // Pino digital para sensor PIR
const int pinLED1 = 3;               // LED da sala
const int pinLED2 = 4;               // LED do corredor
const int pinLED3 = 5;               // LED da cozinha
const int pinBotaoManual = 6;        // Botão para desligar manualmente

// ===== VARIÁVEIS DE CONTROLE =====
bool movimentoDetectado = false;
bool modoManual = false;
unsigned long tempoUltimoMovimento = 0;
const unsigned long TEMPO_ESPERA = 30000;  // 30 segundos para desligar após movimento

// ===== SETUP =====
void setup() {
  // Configurar pinos como entrada/saída
  pinMode(pinSensorMovimento, INPUT);
  pinMode(pinBotaoManual, INPUT_PULLUP);
  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);
  pinMode(pinLED3, OUTPUT);
  
  // Inicializar serial para debug
  Serial.begin(9600);
  Serial.println("=== Sistema de Automação Iniciado ===");
  
  // Desligar todos os LEDs no início
  desligarTodosLEDs();
}

// ===== LOOP PRINCIPAL =====
void loop() {
  // Verificar sensor de movimento
  movimentoDetectado = digitalRead(pinSensorMovimento);
  
  // Verificar botão manual
  verificarBotaoManual();
  
  // Se movimento foi detectado
  if (movimentoDetectado == HIGH) {
    tempoUltimoMovimento = millis();
    ligarTodosLEDs();
    Serial.println("✓ Movimento detectado! LEDs ligados");
  }
  
  // Se passou o tempo de espera, desligar os LEDs
  if (millis() - tempoUltimoMovimento > TEMPO_ESPERA && !modoManual) {
    desligarTodosLEDs();
  }
  
  // Pequeno delay para estabilidade
  delay(100);
}

// ===== FUNÇÕES =====

// Ligar todos os LEDs
void ligarTodosLEDs() {
  digitalWrite(pinLED1, HIGH);
  digitalWrite(pinLED2, HIGH);
  digitalWrite(pinLED3, HIGH);
}

// Desligar todos os LEDs
void desligarTodosLEDs() {
  digitalWrite(pinLED1, LOW);
  digitalWrite(pinLED2, LOW);
  digitalWrite(pinLED3, LOW);
  Serial.println("✗ LEDs desligados (timeout)");
}

// Verificar botão para modo manual
void verificarBotaoManual() {
  if (digitalRead(pinBotaoManual) == LOW) {
    delay(20);  // Debounce
    if (digitalRead(pinBotaoManual) == LOW) {
      modoManual = !modoManual;
      
      if (modoManual) {
        ligarTodosLEDs();
        Serial.println("⚙ Modo Manual: LIGADO");
      } else {
        desligarTodosLEDs();
        Serial.println("⚙ Modo Manual: DESLIGADO");
      }
      
      // Aguardar botão ser solto
      while (digitalRead(pinBotaoManual) == LOW) {
        delay(10);
      }
      delay(20);  // Debounce
    }
  }
}