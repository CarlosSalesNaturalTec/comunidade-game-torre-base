#include "leds_torre.h"

static CRGB _fita[TOTAL_LEDS];

// ─────────────────────────────────────────────────────
// Escolhe a cor do anel de vida conforme o HP restante
//   HP > 60% → verde
//   HP 31–60% → amarelo
//   HP ≤ 30%  → vermelho
// ─────────────────────────────────────────────────────
static CRGB corParaHP(int hp) {
  int porcentagem = (hp * 100) / HP_MAXIMO;
  if (porcentagem > 60) return CRGB::Green;
  if (porcentagem > 30) return CRGB::Yellow;
  return CRGB::Red;
}

// ─────────────────────────────────────────────────────
void leds_iniciar() {
  FastLED.addLeds<WS2812B, PINO_LEDS, GRB>(_fita, TOTAL_LEDS);
  FastLED.setBrightness(BRILHO_LEDS);
  leds_apagar();
  Serial.println("[LEDS] Fita de 30 LEDs pronta!");
}

// Acende exatamente N LEDs com a cor proporcional ao HP
// Os LEDs apagados representam o dano já sofrido
void leds_mostrarHP(int hp) {
  if (hp < 0) hp = 0;
  CRGB cor = corParaHP(hp);

  for (int i = 0; i < TOTAL_LEDS; i++) {
    _fita[i] = (i < hp) ? cor : CRGB::Black;
  }
  FastLED.show();
}

// Flash branco rápido ao receber tiro direto
// Bloqueante por ~200ms — ok: debounce ativo garante pausa entre hits
void leds_flashImpacto() {
  fill_solid(_fita, TOTAL_LEDS, CRGB::White);
  FastLED.show();
  delay(120);
  leds_apagar();
  delay(80);
}

// Flash vermelho mais longo ao receber punição de DANO DUPLO
// Mais intenso para comunicar visualmente a severidade
void leds_flashDanoDuplo() {
  // Pisca 3 vezes vermelho para destacar a punição
  for (int pulso = 0; pulso < 3; pulso++) {
    fill_solid(_fita, TOTAL_LEDS, CRGB::Red);
    FastLED.show();
    delay(130);
    leds_apagar();
    delay(80);
  }
}

// Espiral apagando: cada LED apaga em sequência (dramático!)
// Chamado uma única vez quando o HP chega a 0
void leds_animarGameOver() {
  for (int i = TOTAL_LEDS - 1; i >= 0; i--) {
    _fita[i] = CRGB::Black;
    FastLED.show();
    delay(60);  // 60ms por LED × 30 LEDs = ~1.8s de animação total
  }
}

// Acende em sequência do primeiro ao último LED (efeito de "ligar")
// Chamado ao receber o START do Nexus
void leds_animarInicio() {
  // Fase 1: acende brancos em sequência
  for (int i = 0; i < TOTAL_LEDS; i++) {
    _fita[i] = CRGB::White;
    FastLED.show();
    delay(40);
  }
  delay(200);

  // Fase 2: transição para verde (HP máximo)
  leds_mostrarHP(HP_MAXIMO);
}

void leds_apagar() {
  fill_solid(_fita, TOTAL_LEDS, CRGB::Black);
  FastLED.show();
}
