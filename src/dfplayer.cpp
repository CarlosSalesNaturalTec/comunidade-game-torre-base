#include "dfplayer.h"

// ── SoftwareSerial e objeto do DFPlayer ──────────────
//    RX do NodeMCU recebe o TX do DFPlayer (e vice-versa)
static SoftwareSerial   _serialDF(PINO_DFPLAYER_RX, PINO_DFPLAYER_TX);
static DFRobotDFPlayerMini _df;

// ─────────────────────────────────────────────────────
bool dfplayer_iniciar() {
  _serialDF.begin(9600);
  delay(1000);  // DFPlayer precisa de ~1s para inicializar após energizar

  if (!_df.begin(_serialDF)) {
    // Verifique: cartão SD inserido? Arquivos na pasta "01"?
    // Resistor de 1kΩ entre TX do DFPlayer e RX do NodeMCU?
    Serial.println("[DFPLAYER] ERRO: DFPlayer nao respondeu!");
    Serial.println("[DFPLAYER] → Verifique o cartao SD e as conexoes.");
    return false;
  }

  _df.volume(25);   // 0–30 (25 = volume alto para evento ao ar livre)
  Serial.println("[DFPLAYER] Pronto! Volume: 25/30");
  return true;
}

// ─────────────────────────────────────────────────────
// Todas as funções abaixo são FIRE-AND-FORGET:
// disparam o som e retornam imediatamente.
// O DFPlayer toca em paralelo — o loop() NÃO é bloqueado.
// ─────────────────────────────────────────────────────

void dfplayer_tocarInicio() {
  Serial.println("[DFPLAYER] Tocando: Inicio de batalha");
  _df.playFolder(1, SOM_INICIO);
}

void dfplayer_tocarImpacto() {
  Serial.println("[DFPLAYER] Tocando: Impacto");
  _df.playFolder(1, SOM_IMPACTO);
}

void dfplayer_tocarDanoDuplo() {
  Serial.println("[DFPLAYER] Tocando: Dano Duplo!");
  _df.playFolder(1, SOM_DANO_DUPLO);
}

void dfplayer_tocarGameOver() {
  Serial.println("[DFPLAYER] Tocando: Game Over");
  _df.playFolder(1, SOM_GAME_OVER);
}
