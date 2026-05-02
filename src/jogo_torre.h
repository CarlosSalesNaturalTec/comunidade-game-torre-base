#pragma once
#include <Arduino.h>

// =====================================================
// MÓDULO DE JOGO — Lógica da Torre
// =====================================================

// ── Estados possíveis da Torre ────────────────────────
enum EstadoTorre {
  JOGO_PAUSADO,    // aguardando START do Nexus
  TORRE_VIVA,      // HP > 0, recebendo e processando hits
  TORRE_DESTRUIDA  // HP = 0, Game Over
};

// ── Funções públicas (chamadas de main.cpp) ───────────
void jogo_iniciar();
void jogo_iniciarPartida();      // chamada pelo callback MQTT ao receber START

void jogo_verificarLaser();      // detecta hit local pelo LDR — chame em TODO loop()
void jogo_aplicarDanoDuplo();    // chamada pelo callback MQTT ao receber PUNICAO

// ── Getters ───────────────────────────────────────────
int          jogo_getHP();
EstadoTorre  jogo_getEstado();
