#pragma once
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include "config.h"

// =====================================================
// MÓDULO DE LEDS — Fita WS2812B (30 LEDs circular)
//
// A fita forma um anel ao redor da Torre.
// Os LEDs representam a vida da Torre:
//   30 LEDs verdes  → Torre com HP máximo (100%)
//   15 LEDs amarelos → Torre no meio da vida (50%)
//    5 LEDs vermelhos → Torre quase destruída
//    0 LEDs acesos   → Torre destruída!
//
// A cor muda automaticamente conforme o HP cai.
// =====================================================

void leds_iniciar();

void leds_mostrarHP(int hp);    // atualiza o anel de vida
void leds_flashImpacto();        // piscada branca ao receber tiro normal
void leds_flashDanoDuplo();      // piscada vermelha ao receber punição
void leds_animarGameOver();      // espiral apagando ao zerar o HP
void leds_animarInicio();        // acende em sequência ao receber START
void leds_apagar();
