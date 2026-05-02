#include <Arduino.h>
#include "config.h"
#include "leds_torre.h"
#include "dfplayer.h"
#include "mqtt_torre.h"
#include "jogo_torre.h"

// ==========================================================
// TORRE — Firmware da Grande Base
// Projeto: Comunidade Game - Inclusão Digital
//
// Hardware: NodeMCU ESP8266 v2
//           Fita WS2812B IP67 (30 LEDs em círculo)
//           2x Módulo Sensor LDR
//           DFPlayer Mini + caixa de som
//
// Descrição da Torre:
//   A Torre é o coração da equipe defensora.
//   Ela começa com 30 LEDs VERDES (HP máximo).
//   Cada tiro do Atacante que passar pelo Defensor
//   apaga 3 LEDs → a cor muda com a vida:
//     Verde  → HP alto (> 60%)
//     Amarelo → HP médio (31–60%)
//     Vermelho → HP baixo (≤ 30%) — perigo!
//   Quando todos os LEDs apagam → GAME OVER!
//   Se o Defensor cometer uma falta, a Torre recebe
//   DANO DUPLO via MQTT — 6 LEDs apagam de uma vez!
// ==========================================================

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("==============================================");
  Serial.println("   TORRE — A Grande Base          v1.0");
  Serial.println("   Comunidade Game - Inclusão Digital");
  Serial.println("==============================================");

  leds_iniciar();       // prepara a fita WS2812B de 30 LEDs
  dfplayer_iniciar();   // inicializa o DFPlayer Mini e o cartão SD
  mqtt_iniciar();       // conecta ao Wi-Fi e ao broker MQTT
  jogo_iniciar();       // configura os pinos LDR e o estado inicial

  Serial.println("[SETUP] Pronto! Aguardando START do Nexus...");
  Serial.println();
}

void loop() {
  // ── Passo 1: manter a conexão com o broker ────────────
  if (!mqtt_conectado()) {
    mqtt_reconectar();
  }
  mqtt_processar();   // processa mensagens recebidas (START e DANO_DUPLO)

  // ── Passo 2: verificar se o laser acertou a Torre ────
  //    Hit detection roda localmente — independente da rede
  jogo_verificarLaser();

  // ── Passo 3: após Game Over, não faz mais nada ───────
  //    O Nexus detecta hp=0 e encerra a partida no painel
}
