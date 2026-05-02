#include "jogo_torre.h"
#include "leds_torre.h"
#include "dfplayer.h"
#include "mqtt_torre.h"
#include "config.h"

// ── Variáveis internas de estado ──────────────────────
static EstadoTorre   _estado      = JOGO_PAUSADO;
static int           _hpTorre     = HP_MAXIMO;
static unsigned long _ultimoHitMs = 0;

// ─────────────────────────────────────────────────────
// FUNÇÕES INTERNAS (privadas)
// ─────────────────────────────────────────────────────

// Retorna true se qualquer LDR detectou o laser
static bool laserDetectado() {
  bool sensor1 = (digitalRead(PINO_LDR_1) == LDR_NIVEL_HIT);
  bool sensor2 = (digitalRead(PINO_LDR_2) == LDR_NIVEL_HIT);
  return sensor1 || sensor2;
}

// Retorna true se o intervalo mínimo entre dois hits foi respeitado
static bool debounceOk() {
  return (millis() - _ultimoHitMs) >= DEBOUNCE_HIT_MS;
}

// Aplica um valor de dano ao HP e trata as consequências
// É chamada tanto pelo dano simples (LDR) quanto pelo dano duplo (MQTT)
static void aplicarDano(int quantidade, bool ehDanoDuplo) {
  _hpTorre -= quantidade;
  if (_hpTorre < 0) _hpTorre = 0;

  // Animação e som diferentes para dano normal e dano duplo
  if (ehDanoDuplo) {
    leds_flashDanoDuplo();
    dfplayer_tocarDanoDuplo();
    Serial.print("[JOGO] DANO DUPLO! HP da Torre: ");
  } else {
    leds_flashImpacto();
    dfplayer_tocarImpacto();
    Serial.print("[JOGO] HIT! HP da Torre: ");
  }

  Serial.print(_hpTorre);
  Serial.print(" / ");
  Serial.println(HP_MAXIMO);

  leds_mostrarHP(_hpTorre);
  mqtt_publicarHP(_hpTorre);

  // Verificar se a Torre foi destruída
  if (_hpTorre <= 0) {
    _estado = TORRE_DESTRUIDA;
    Serial.println("[JOGO] *** TORRE DESTRUIDA! GAME OVER! ***");
    leds_animarGameOver();
    dfplayer_tocarGameOver();
    mqtt_publicarHP(0);
  }
}

// ─────────────────────────────────────────────────────
// FUNÇÕES PÚBLICAS
// ─────────────────────────────────────────────────────

void jogo_iniciar() {
  pinMode(PINO_LDR_1, INPUT);
  pinMode(PINO_LDR_2, INPUT);
  _estado   = JOGO_PAUSADO;
  _hpTorre  = HP_MAXIMO;
  leds_apagar();

  #ifdef MODO_TESTE
    Serial.println("[JOGO] *** MODO TESTE ATIVO — Torre pronta sem START! ***");
    Serial.println("[JOGO] Aponte o laser nos sensores para testar.");
    jogo_iniciarPartida();
  #else
    Serial.println("[JOGO] Torre inicializada. Aguardando START...");
  #endif
}

void jogo_iniciarPartida() {
  _hpTorre     = HP_MAXIMO;
  _estado      = TORRE_VIVA;
  _ultimoHitMs = 0;

  // Animação de início: branco → verde nos 30 LEDs
  leds_animarInicio();

  // Voz épica de início de batalha via DFPlayer
  dfplayer_tocarInicio();

  Serial.println("[JOGO] *** PARTIDA INICIADA! Torre com HP maximo. ***");
  mqtt_publicarHP(_hpTorre);
}

// Verifica hit local pelo LDR — independente da rede
// Chame em TODO loop()
void jogo_verificarLaser() {
  if (_estado != TORRE_VIVA) return;

  if (laserDetectado() && debounceOk()) {
    _ultimoHitMs = millis();

    Serial.println("[JOGO] Laser detectado no LDR da Torre!");
    aplicarDano(DANO_POR_TIRO, false);   // dano simples
  }
}

// Chamada pelo callback MQTT ao receber a punição do Defensor
// O dano duplo vem da rede, mas é aplicado localmente aqui
void jogo_aplicarDanoDuplo() {
  if (_estado != TORRE_VIVA) return;

  Serial.println("[JOGO] Punição recebida via MQTT → aplicando DANO DUPLO!");
  aplicarDano(DANO_POR_TIRO * DANO_DUPLO_FATOR, true);
}

int jogo_getHP() {
  return _hpTorre;
}

EstadoTorre jogo_getEstado() {
  return _estado;
}
